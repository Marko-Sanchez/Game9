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

    // TODO:
}

void Window::ProcessKeyboardCallback(int key, int scancode, int action, int mods)
{

    // TODO:
}

void Window::ProcessMouseCallback(double xPosIn, double yPosIn)
{

    // TODO:
}

void Window::ProcessMouseScrollCallback(double xPosIn, double yPosIn)
{

    // TODO:
}
}// namespace Game9
