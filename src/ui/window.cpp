#include "ui/window.h"

#include <GLFW/glfw3.h>
#include <array>
#include <filesystem>
#include <iostream>
#include <stb/stb_image.h>
#include <system_error>

namespace Game9
{

Window::Window(std::shared_ptr<GLFWwindow> window, int width, int height):
m_width(width),
m_height(height),
m_window(window)
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

    /* SetWindowIcon(); */
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

// Note:
// Due to Gnome terminal not having the option to display window-icons, this function
// is not tested and thus not called. Through some research glfwSetWindowIcon() may need
// to be called early in window context creation thus this code is replicated in main.cpp;
// Although, this function remains here until it's possible to test.
void Window::SetWindowIcon()
{
    const std::array<std::filesystem::path, 3> files =
    {
        "resources/images/winIcon64.png",
        "resources/images/winIcon32.png",
        "resources/images/winIcon16.png"
    };

    std::error_code ec;
    for (const auto& file: files)
    {
        if (!std::filesystem::exists(file, ec))
        {
            std::cerr << "Image does not exist: " << ec.message() << std::endl;
            return;
        }
    }

    GLFWimage icons[3];

    std::shared_ptr<unsigned char> data64(stbi_load(files[0].c_str(), &icons[0].width, &icons[0].height, 0, 4), stbi_image_free);
    std::shared_ptr<unsigned char> data32(stbi_load(files[1].c_str(), &icons[1].width, &icons[1].height, 0, 4), stbi_image_free);
    std::shared_ptr<unsigned char> data16(stbi_load(files[2].c_str(), &icons[2].width, &icons[2].height, 0, 4), stbi_image_free);

    icons[0].pixels = data64.get();
    icons[1].pixels = data32.get();
    icons[2].pixels = data16.get();

    if (!data64 || !data32 || !data16)
    {
        std::cerr << "Failed to load icon image." << std::endl;
        return;
    }

    glfwSetWindowIcon(m_window.get(), 3, icons);
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
