#include "scene/Game.h"

#include "glm/ext/matrix_clip_space.hpp"

#include <GL/gl.h>

#include "utility/ResourceManager.h"

std::unique_ptr<SpriteRenderer> Renderer;

Game::Game(std::shared_ptr<GLFWwindow> window, int width, int height):
m_state(GAME_ACTIVE),
m_window(window, width, height)
{}

Game::~Game()
{
    Renderer.reset();
}

void Game::Init()
{
    // Load shaders.
    ResourceManager::LoadShader("resources/shaders/background.vertex", "resources/shaders/background.fragment", "background");
    // TODO: change shader to use same glCreateShader ID for different programs that use the same, shader.
    ResourceManager::LoadShader("resources/shaders/background.vertex", "resources/shaders/background.fragment", "train");

    // Configure shaders:
    // bottom-left corner is (0,0) top-right is (width, height).
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_window.GetWidth()), 0.0f, static_cast<float>(m_window.GetHeight()), -1.0f, 1.0f);

    ResourceManager::GetShader("background").value()->Bind();
    ResourceManager::GetShader("background").value()->SetUniform1i("u_image", 0);
    ResourceManager::GetShader("background").value()->SetUniformMat4f("u_projection", projection);

    ResourceManager::GetShader("train").value()->Bind();
    ResourceManager::GetShader("train").value()->SetUniform1i("u_image", 0);
    ResourceManager::GetShader("train").value()->SetUniformMat4f("u_projection", projection);

    // TODO: temp for testing. (static-renderer, etc.)
    Renderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("background").value());

    // Load Textures.
    ResourceManager::LoadTexture("resources/images/background.png", "background");
    ResourceManager::LoadTexture("resources/images/train.png", "train");
}

void Game::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexture("background").value(), glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(m_window.GetWidth()), static_cast<float>(m_window.GetHeight())));
    Renderer->DrawSprite(ResourceManager::GetTexture("train").value(),
                         glm::vec2(static_cast<float>(m_window.GetWidth()) / 2, static_cast<float>(m_window.GetHeight()) / 2),
                         glm::vec2(256, 256));
}

void Game::ProcessInput(float deltaTime)
{
    m_window.Tick(deltaTime);
}
