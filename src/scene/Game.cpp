#include "scene/Game.h"

#include "glm/ext/matrix_clip_space.hpp"

#include "ui/window.h"
#include "utility/ResourceManager.h"

SpriteRenderer *Renderer;

Game::Game(std::shared_ptr<GLFWwindow> window, int width, int height):
m_state(GAME_ACTIVE),
m_window(window, width, height)
{}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    // Load shaders.
    ResourceManager::LoadShader("resources/Shaders/background.vertex", "resources/Shaders/background.fragment", "background");

    // Configure shaders.
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_window.GetWidth()), static_cast<float>(m_window.GetHeight()), 0.0f, 1.0f, 1.0f);
    ResourceManager::GetShader("background")->Bind();
    ResourceManager::GetShader("background")->SetUniform1i("u_image", 0);
    ResourceManager::GetShader("background")->SetUniformMat4f("u_projection", projection);

    // TODO: temp for testing. (static-renderer, etc.)
    Renderer = new SpriteRenderer(ResourceManager::GetShader("background").value());

    // Load Textures.
    ResourceManager::LoadTexture("resources/images/background.png", "map");
}

void Game::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexture("map").value());
}

void Game::ProcessInput(float deltaTime)
{
    m_window.Tick(deltaTime);
}
