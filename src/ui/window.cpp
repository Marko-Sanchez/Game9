#include "ui/window.h"

#include <GLFW/glfw3.h>
#include <array>
#include <filesystem>
#include <iostream>
#include <format>
#include <stb/stb_image.h>
#include <system_error>

namespace Game9
{

Window::Window(std::shared_ptr<GLFWwindow> window, int width, int height):
m_width(width),
m_height(height),
m_window(window),
m_zoomFactor(1.0f)
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

    glfwSetMouseButtonCallback(m_window.get(), [](GLFWwindow* window, int button, int action, int mods)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessMousePressCallback(button, action, mods);
    });

    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessMousePosCallback(xPosIn, yPosIn);
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

float Window::GetZoom() const
{
    return m_zoomFactor;
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
}

void Window::ProcessMousePressCallback(int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        std::cout << std::format("Mouse button pressed: x {}, y: {}\n", m_lastMouseX, m_lastMouseY);
    }
}

/*
 * Called whenever mouse is moved in current context window.
 * Top-left is (0,0), bottom-left is (0, 1024).
 *
 * Note: when drawing object models and handling object movement,
 * bottom-left is considered (0, 0). Hence, we subtract current y position
 * with window height.
 */
void Window::ProcessMousePosCallback(double xPosIn, double yPosIn)
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

    // Save current mouse position.
    m_lastMouseX = xpos;
    m_lastMouseY = m_height - ypos;
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

    std::cout << std::format("zoom: {}, delta: {} ", m_zoomFactor, yPosIn * 0.1f) << std::endl;
}
}// namespace Game9
