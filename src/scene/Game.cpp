#include "scene/Game.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float2.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "utility/ResourceManager.h"
#include "utility/SpriteRenderer.h"
#include <iostream>

namespace Core
{
/*
 * Set GLFW to call this function in case of error.
 */
static void GLFWErrorCallback(int error, const char* description)
{
    std::cerr << "[GLFW Error]: " << description << std::endl;
}

Game::Game(const ApplicationSpecification& specification):
m_specification(specification)
{
    glfwSetErrorCallback(GLFWErrorCallback);
    if (!glfwInit())
    {
        std::cerr << "Failed to intialize GLFW." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Set window specification and create the glfw window context.
    if (m_specification.windowspec.title.empty())
    {
        m_specification.windowspec.title = m_specification.name;
    }

    m_window = std::make_shared<Window>(m_specification.windowspec);
    m_window->Create();

    // OpenGL configuration.
    if (GLenum err = glewInit(); err != GLEW_OK)
    {
        std::cerr << "Failed to Initialize GLEW\nNeeds a valid OpenGL Context: Window::Create()?\n";
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Game::~Game()
{
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
    m_resourceManager.LoadShader("resources/shaders/background.vertex", "resources/shaders/background.fragment", "train");
    // = {
    // shader-path, name-enumvalue(use to retrieve)
    // }

    // Configure shaders:
    // bottom-left corner is (0,0) top-right is (width, height).

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_specification.windowspec.width), 0.0f, static_cast<float>(m_specification.windowspec.height), -1.0f, 1.0f);

    m_resourceManager.GetShader("train")->Bind();
    m_resourceManager.GetShader("train")->SetUniform1i("u_image", 1);
    m_resourceManager.GetShader("train")->SetUniformMat4f("u_projection", projection);

    m_trainHandler = std::make_unique<Game9::TrainHandler>();
    entityRenderer = std::make_unique<SpriteRenderer>(m_resourceManager.GetShader("train"));

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
        // this->ProcessInput(deltaTime);

        // TODO: update game state.
        this->Update(deltaTime);

        // Clear and Render.
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        this->Render();

        m_window->Update();
    }
}

void Game::Render()
{
    for (auto& layer: m_layerStack)
    {
        layer->OnRender();
    }

    m_trainHandler->Draw(entityRenderer);
}

void Game::Update(float deltaTime)
{
    for (auto& layer: m_layerStack)
    {
        layer->OnUpdate(deltaTime);
    }
    m_trainHandler->Update(deltaTime);
}

void Game::ProcessInput(float deltaTime)
{
    m_window->Tick(deltaTime);
}

std::shared_ptr<Window> Game::GetWindow()
{
    return m_window;
}
}// namespace Core
