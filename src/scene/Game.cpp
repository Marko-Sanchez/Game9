#include "scene/Game.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float2.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "utility/ResourceManager.h"
#include <iostream>

namespace Core
{
static void GLFWErrorCallback(int error, const char* description)
{
    std::cerr << "[GLFW Error]: " << description << std::endl;
}

Game::Game(const ApplicationSpecification& specification):
m_specification(specification)
{
    glfwSetErrorCallback(GLFWErrorCallback);
    glfwInit();

    if (m_specification.windowspec.title.empty())
    {
        m_specification.windowspec.title = m_specification.name;
    }

    m_window = std::make_shared<Window>(m_specification.windowspec);
    m_window->Create();

    // OpenGL configuration.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (GLenum err = glewInit(); err != GLEW_OK)
    {
        std::cerr << "Failed to Initialize GLEW\nNeeds a valid OpenGL Context: Window::Create()?\n";
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
}

Game::~Game()
{
    sceneRenderer.reset();
    entityRenderer.reset();

    m_window->Destroy();
    glfwTerminate();
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

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_specification.windowspec.width), 0.0f, static_cast<float>(m_specification.windowspec.height), -1.0f, 1.0f);

    ResourceManager::GetShader("background").value()->Bind();
    ResourceManager::GetShader("background").value()->SetUniform1i("u_image", 0);
    ResourceManager::GetShader("background").value()->SetUniformMat4f("u_projection", projection);

    ResourceManager::GetShader("train").value()->Bind();
    ResourceManager::GetShader("train").value()->SetUniform1i("u_image", 1);
    ResourceManager::GetShader("train").value()->SetUniformMat4f("u_projection", projection);

    m_background = std::make_unique<Game9::SceneHandler>(ResourceManager::GetShader("background").value(),
                                                         m_window,
                                                         ResourceManager::LoadTexture("resources/images/background.png", "background", 0).value());
    m_trainHandler = std::make_unique<Game9::TrainHandler>();
    entityRenderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("train").value());

    // loads all game assest.
    m_trainHandler->LoadPaths();
}

void Game::Run()
{
    m_isRunning = true;

    this->Init();

    float lastFrame{};
    float deltaTime{};
    while (m_isRunning)
    {
        if (m_window->ShouldClose())
        {
            m_isRunning = false;
            break;
        }

        // Calculate delta time and processes callbacks.
        float currentFrame{static_cast<float>(glfwGetTime())};
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input.
        /* this->ProcessInput(deltaTime); */

        // TODO: update game state.
        this->Update(deltaTime);

        // Render.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        this->Render();

        m_window->Update();
    }
}

/*
* TODO: Handling the world loader.
* 1. Similiar to trainHandler but for worlds.
* 2. Should be able to handle multiple worlds: LA, SD
*
* 3. go to Window to manage mouse or arrow key callbacks to, to be able to move space around.
*/
void Game::Render()
{
    const float w{static_cast<float>(m_specification.windowspec.width)};
    const float h{static_cast<float>(m_specification.windowspec.height)};

    const float hw{(w * m_window->GetZoom()) * 0.5f};
    const float hh{(h * m_window->GetZoom()) * 0.5f};

    m_background->UpdateProjection(
    glm::ortho(w * 0.5f - hw, w * 0.5f + hw,
               h * 0.5f - hh, h * 0.5f + hh,
               -1.0f, 1.0f));

    m_background->Draw();
    m_trainHandler->Draw(entityRenderer);
}

void Game::Update(float deltaTime)
{
    m_trainHandler->Update(deltaTime);
}

void Game::ProcessInput(float deltaTime)
{
    m_window->Tick(deltaTime);
}
}// namespace Core
