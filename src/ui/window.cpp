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

    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessMouseCallback(xPosIn, yPosIn);
    });

    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessMouseScrollCallback(xPosIn, yPosIn);
    });

    glfwSetKeyCallback(m_window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessKeyboardCallback(key, scancode, action, mods);
    });

    glfwSetWindowSizeCallback(m_window.get(), [](GLFWwindow* window, int width, int height)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessWindowSizeCallback(width, height);
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

}// namespace Game9
