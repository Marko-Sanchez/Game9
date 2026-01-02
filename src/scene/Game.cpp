#include "scene/Game.h"

#include <print>
#include <cstdio>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

namespace Core
{
/*
 * Set GLFW to call this function in case of error.
 */
static void GLFWErrorCallback(int error, const char* description)
{
    std::println(stderr, "[GLFW Error {}]: {}", error, description);
}

Game::Game(const ApplicationSpecification& specification):
m_specification(specification)
{
    glfwSetErrorCallback(GLFWErrorCallback);
    if (!glfwInit())
    {
        std::println("Failed to intialize GLFW.");
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
        std::println(stderr, "Failed to Initialize GLEW need a valid OpenGL Context:\nError: {}", reinterpret_cast<const char *>(glewGetErrorString(err)));
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Game::~Game()
{
    m_window->Destroy();
    glfwTerminate();
}

void Game::Run()
{
    m_isRunning = true;

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
}

void Game::Update(float deltaTime)
{
    for (auto& layer: m_layerStack)
    {
        layer->OnUpdate(deltaTime);
    }
}

/*
 * Each layer handles its' own custom method of handling event, cascading until event is handled.
 *
 * @param
 * event: event (mousepressed, keypressed).
 */
void Game::RaiseEvent(Event &event)
{
    for (auto iter = m_layerStack.rbegin(); iter != m_layerStack.rend(); ++iter)
    {
        (*iter)->OnEvent(event);
        if (event.isHandled)
        {
            break;
        }
    }
}

/*
 * Used to provide access to Window management class when pushing layers onto stack.
 */
std::shared_ptr<Window> Game::GetWindow() noexcept
{
    return m_window;
}
}// namespace Core
