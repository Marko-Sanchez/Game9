#include "scene/Game.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float2.hpp"

#include <GL/gl.h>

#include "utility/ResourceManager.h"


Game::Game(std::shared_ptr<GLFWwindow> window, int width, int height):
m_state(GameState::GAME_ACTIVE),
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

    m_trainHandler = std::make_unique<Game9::TrainHandler>();

    // TODO: temp for testing. (static-renderer, etc.)
    sceneRenderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("background").value());
    entityRenderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("train").value());

    // Load Textures.
    ResourceManager::LoadTexture("resources/images/background.png", "background", 0);

    // TODO: Testing, hard coded paths.
    m_trainHandler->LoadPaths("placeholder value for testing");
}

void Game::Render()
{
    sceneRenderer->DrawSprite(ResourceManager::GetTexture("background").value(), glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(m_window.GetWidth()), static_cast<float>(m_window.GetHeight())));
    m_trainHandler->Draw(entityRenderer);
}

void Game::Update(float deltaTime)
{
    if (m_state == GameState::GAME_ACTIVE)
    {
        m_trainHandler->Update(deltaTime);
    }
}

void Game::ProcessInput(float deltaTime)
{
    m_window.Tick(deltaTime);
}
