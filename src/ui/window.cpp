#include "ui/window.h"

#include <GLFW/glfw3.h>

#include <array>
#include <cassert>
#include <filesystem>
#include <print>
#include <cstdio>
#include <stb/stb_image.h>
#include <system_error>

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
void Window::RaiseEvent(Event &event)
{
    if (m_specification.EventCallback)
        m_specification.EventCallback(event);
}

void Window::Tick(float frameDelta)
{
    m_frameDelta = frameDelta;
}

WindowSpecification Window::GetWindowSpecification() const
{
    return m_specification;
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
            std::println(stderr, "Image does not exist: {}", ec.message());
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
        std::println(stderr, "Failed to load icon image.");
        return;
    }

    glfwSetWindowIcon(m_handle, 3, icons);
}

void Window::SetWindowCallbacks()
{
    glfwSetWindowUserPointer(m_handle, this);

    glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* window, int width, int height)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessWindowSizeCallback(width, height);
        // SomeEvent event(width, height);
        // reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->RaiseEvent(event);
    });

    glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessKeyboardCallback(key, scancode, action, mods);
    });

    glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int button, int action, int mods)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessMousePressCallback(button, action, mods);
    });

    glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessMousePosCallback(xPosIn, yPosIn);
    });

    glfwSetScrollCallback(m_handle, [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->ProcessMouseScrollCallback(xPosIn, yPosIn);
    });
}

void Window::ProcessWindowSizeCallback(int width, int height)
{
    m_specification.width  = width;
    m_specification.height = height;
}

void Window::ProcessKeyboardCallback(int key, int scancode, int action, int mods)
{
}

void Window::ProcessMousePressCallback(int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        std::println("Mouse button pressed: x {}, y: {}", m_lastMouseX, m_lastMouseY);
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
    m_lastMouseY = m_specification.height - ypos;
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
