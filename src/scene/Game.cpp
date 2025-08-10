#include "scene/Game.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float2.hpp"

#include <GL/gl.h>

#include "utility/ResourceManager.h"

// Issue uses the same shader for background and train but still works.
// Multiple SpriteRenderers for different things like background and entitys ?
std::unique_ptr<SpriteRenderer> sceneRenderer;
std::shared_ptr<SpriteRenderer> entityRenderer;

Game::Game(std::shared_ptr<GLFWwindow> window, int width, int height):
m_state(GAME_ACTIVE),
m_window(window, width, height)
{}

Game::~Game()
{
    sceneRenderer.reset();
    entityRenderer.reset();
}

/*
* Loads Shaders and Textures, sets the intial values for shader uniforms.
*/
void Game::Init()
{
    // Load shaders.
    ResourceManager::LoadShader("resources/shaders/background.vertex", "resources/shaders/background.fragment", "background");
    ResourceManager::LoadShader("resources/shaders/background.vertex", "resources/shaders/background.fragment", "train");

    // Configure shaders:
    // bottom-left corner is (0,0) top-right is (width, height).
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_window.GetWidth()), 0.0f, static_cast<float>(m_window.GetHeight()), -1.0f, 1.0f);

    ResourceManager::GetShader("background").value()->Bind();
    ResourceManager::GetShader("background").value()->SetUniform1i("u_image", 0);
    ResourceManager::GetShader("background").value()->SetUniformMat4f("u_projection", projection);

    ResourceManager::GetShader("train").value()->Bind();
    ResourceManager::GetShader("train").value()->SetUniform1i("u_image", 1);
    ResourceManager::GetShader("train").value()->SetUniformMat4f("u_projection", projection);

    // TODO: temp for testing. (static-renderer, etc.)
    sceneRenderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("background").value());
    entityRenderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("train").value());

    // Load Textures.
    ResourceManager::LoadTexture("resources/images/background.png", "background", 0);
    ResourceManager::LoadTexture("resources/images/train.png", "train", 1);

    // Temp: in the future setting path may require to be copied from a file, and other way of adding trains to data structure.
    m_trains.emplace_back(ResourceManager::GetTexture("train").value(), glm::vec2(m_window.GetWidth() / 2, m_window.GetHeight() /2), glm::vec2(64, 64), glm::vec2(50.0f, 50.0f));
    m_trains.back().SetPath({glm::vec2(m_window.GetWidth() / 2, m_window.GetHeight() /2), glm::vec2{m_window.GetWidth(), m_window.GetHeight()}, glm::vec2{0,0}});
}

void Game::Render()
{
    sceneRenderer->DrawSprite(ResourceManager::GetTexture("background").value(), glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(m_window.GetWidth()), static_cast<float>(m_window.GetHeight())));

    for (auto& train: m_trains)
    {
        train.Draw(entityRenderer.get());
    }
}

void Game::Update(float deltaTime)
{
    if (m_state == GAME_ACTIVE)
    {
        for(auto& train: m_trains)
        {
            train.Move(deltaTime);
        }
    }
}

void Game::ProcessInput(float deltaTime)
{
    m_window.Tick(deltaTime);
}
