#include "core/window.h"

#include <cassert>
#include <print>
#include <cstdio>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "events/WindowEvents.h"
#include "events/MouseEvents.h"
#include "events/KeyEvents.h"

namespace Core
{

/*
 * Create window and set events.
 */
Window::Window(const WindowSpecification& specification):
m_zoomFactor(1.0f),
m_specification(specification)
{
    m_handle = glfwCreateWindow(m_specification.width, m_specification.height, m_specification.title.c_str(), nullptr, nullptr);
    if (!m_handle)
    {
        assert(false && "Window failed to create.");
    }

    // Necessary to occur after creating Window; Any other version might not work with current code.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make Window Current Context, set swap interval to wait for 1 screen update before swapping buffers.
    glfwMakeContextCurrent(m_handle);
    glfwSwapInterval(1);

    // explicitly set viewport, default set to dimensions of window.
    const auto [width, height] = this->GetFrameBufferSize();
    glViewport(0.0f, 0.0f, width, height);

    this->SetWindowCallbacks();
}

Window::~Window()
{
    Destroy();
}

void Window::Destroy()
{
    if (m_handle)
        glfwDestroyWindow(m_handle);

    m_handle = nullptr;
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_handle) != 0;
}

/*
 * Swap Front buffer (currently being displayed) and back buffer (next to render).
 */
void Window::Update()
{
    glfwSwapBuffers(m_handle);
}

/*
 * When an event is invoked, call Game::RaiseEvent() to allow each layer to handle event.
 */
void Window::RaiseEvent(Event::Event &event)
{
    if (m_specification.EventCallback)
        m_specification.EventCallback(event);
}

void Window::Tick(float frameDelta) noexcept
{
    m_frameDelta = frameDelta;
}

float Window::GetZoom() const noexcept
{
    return m_zoomFactor;
}

float Window::GetWidth() const noexcept
{
    return static_cast<float>(m_specification.width);
}

float Window::GetHeight() const noexcept
{
    return static_cast<float>(m_specification.height);
}

std::pair<float, float> Window::GetFrameBufferSize() const
{
    int width, height;
    glfwGetFramebufferSize(m_handle, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

std::pair<double, double> Window::GetMousePosition() const
{
    double xpos, ypos;
    glfwGetCursorPos(m_handle, &xpos, &ypos);
    return {xpos, ypos};
}

void Window::SetWindowCallbacks()
{
    glfwSetWindowUserPointer(m_handle, this);

    glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* window, int width, int height)
    {
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        win->m_specification.width = width;
        win->m_specification.height = height;

        Event::WindowResizedEvent event(width, height);
        win->RaiseEvent(event);
    });

    glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* window)
    {
        Event::WindowClosedEvent event;
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->RaiseEvent(event);
    });

    glfwSetScrollCallback(m_handle, [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        Event::MousedScrolledEvent event(xPosIn, yPosIn);
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->RaiseEvent(event);
    });

    glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int button, int action, int mods)
    {
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        switch (action)
        {
            case GLFW_PRESS:
            {
                Event::MouseButtonPressedEvent event(button);
                win->RaiseEvent(event);
                break;
            }
            case GLFW_RELEASE:
            {
                Event::MouseButtonReleasedEvent event(button);
                win->RaiseEvent(event);
                break;
            }
        }
    });

    glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        switch (action)
        {
            case GLFW_PRESS:
            case GLFW_REPEAT:
            {
                Event::KeyPressedEvent event(key, action == GLFW_REPEAT);
                win->RaiseEvent(event);
                break;
            }
            case GLFW_RELEASE:
            {
                Event::KeyReleasedEvent event(key);
                win->RaiseEvent(event);
                break;
            }
        }
    });

    glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        Event::MouseMovedEvent event(xPosIn, yPosIn);
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->RaiseEvent(event);
    });

}

void Window::ProcessMouseScrollCallback(double xPosIn, double yPosIn)
{
    m_zoomFactor += yPosIn * 0.1f;
    if (m_zoomFactor > 1.5f)
    {
        m_zoomFactor = 1.5f;
    }
    else if (m_zoomFactor < 0.5f)
    {
        m_zoomFactor = 0.5f;
    }

    std::println("zoom: {}, delta: {} ", m_zoomFactor, yPosIn * 0.1f);
}
}// namespace Core
