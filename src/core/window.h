#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

#include "events/Events.h"

namespace Core
{

struct WindowSpecification
{
    unsigned int width{1024};
    unsigned int height{1024};
    std::string title;
    std::function<void(Event::Event&)> EventCallback;

    bool isResizeable;
};

/*
* Handles window context, and glfw callbacks.
*/
class Window
{
private:
    float m_zoomFactor;
    float m_frameDelta;

    GLFWwindow* m_handle = nullptr;
    WindowSpecification m_specification;

public:
    Window(const WindowSpecification& specification);
    ~Window();

    void Destroy();
    bool ShouldClose();

    void Update();
    void RaiseEvent(Event::Event &event);

    void Tick(float frameDelta) noexcept;

    float GetZoom() const noexcept;
    float GetWidth() const noexcept;
    float GetHeight() const noexcept;

    std::pair<float, float> GetFrameBufferSize() const;
    std::pair<double, double> GetMousePosition() const;

private:
    // callbacks.
    void SetWindowCallbacks();
    void ProcessMouseScrollCallback(double xPosIn, double yPosIn);
};
}// namespace Core

#endif
