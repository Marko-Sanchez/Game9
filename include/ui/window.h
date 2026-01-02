#ifndef WINDOW_H
#define WINDOW_H

#include "ui/Event.h"
#include <GLFW/glfw3.h>
#include <string>

namespace Core
{

struct WindowSpecification
{
    unsigned int width{1024};
    unsigned int height{1024};
    std::string title;
    std::function<void(Event&)> EventCallback;

    bool isResizeable;
};

/*
* Handles window context, and glfw callbacks.
*/
class Window
{
private:
    float m_zoomFactor;

    float m_lastMouseX;
    float m_lastMouseY;
    float m_frameDelta;

    GLFWwindow* m_handle = nullptr;
    bool m_firstMouseMovement;

    WindowSpecification m_specification;

public:
    Window(const WindowSpecification& specification);
    ~Window();

    void Destroy();
    bool ShouldClose();

    void Update();
    void RaiseEvent(Event &event);

    void Tick(float frameDelta);

    WindowSpecification GetWindowSpecification() const;
    float GetZoom() const;

private:
    void SetWindowIcon();

    void SetWindowCallbacks();
    // callbacks.
    void ProcessWindowSizeCallback(int width, int height);
    void ProcessKeyboardCallback(int key, int scancode, int action, int mods);
    void ProcessMousePressCallback(int button, int action, int mods);
    void ProcessMousePosCallback(double xPosIn, double yPosIn);
    void ProcessMouseScrollCallback(double xPosIn, double yPosIn);
};
}// namespace Core

#endif
