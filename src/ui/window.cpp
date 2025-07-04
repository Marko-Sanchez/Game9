#include "ui/window.h"

#include <GLFW/glfw3.h>

namespace Game9
{

Window::Window(std::shared_ptr<GLFWwindow> window, int width, int height):
m_window(window),
m_width(width),
m_height(height)
{
    glfwSetWindowUserPointer(m_window.get(), this);

    glfwSetWindowSizeCallback(m_window.get(), [](GLFWwindow* window, int width, int height)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessWindowSizeCallback(width, height);
    });

    glfwSetKeyCallback(m_window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessKeyboardCallback(key, scancode, action, mods);
    });

    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessMouseCallback(xPosIn, yPosIn);
    });

    glfwSetScrollCallback(m_window.get(), [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessMouseScrollCallback(xPosIn, yPosIn);
    });
}

void Window::Tick(float frameDelta)
{
    m_frameDelta = frameDelta;
}

int Window::GetWidth() const
{
    return m_width;
}

int Window::GetHeight() const
{
    return m_height;
}

void Window::ProcessWindowSizeCallback(int width, int height)
{
    m_width  = width;
    m_height = height;
}

void Window::ProcessKeyboardCallback(int key, int scancode, int action, int mods)
{
    m_camera.ProcessKeyboardInput(key, m_frameDelta);
}

void Window::ProcessMouseCallback(double xPosIn, double yPosIn)
{
    float xpos{static_cast<float>(xPosIn)};
    float ypos{static_cast<float>(yPosIn)};

    if (m_firstMouseMovement)
    {
        m_lastMouseX = xpos;
        m_lastMouseY = ypos;
        m_firstMouseMovement = false;
    }

    float xoffset{xpos - m_lastMouseX};
    float yoffset{m_lastMouseY - ypos};

    m_lastMouseX = xpos;
    m_lastMouseY = ypos;
    m_camera.ProcessMouseMovement(xoffset, yoffset);
}

void Window::ProcessMouseScrollCallback(double xPosIn, double yPosIn)
{
    m_camera.ProcessMouseScroll(static_cast<float>(yPosIn));
}
}// namespace Game9
