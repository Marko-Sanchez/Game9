#ifndef WINDOW_H
#define WINDOW_H

#include "scene/Camera.h"
#include <GLFW/glfw3.h>
#include <memory>

namespace Game9
{
/*
* Handles window context, and glfw callbacks.
*/
class Window
{
private:
    int m_width;
    int m_height;

    float m_lastMouseX;
    float m_lastMouseY;
    float m_frameDelta;

    Camera m_camera;
    bool m_firstMouseMovement;

    std::shared_ptr<GLFWwindow> m_window;

public:
    Window(std::shared_ptr<GLFWwindow> window, int width, int height);

    void Tick(float frameDelta);

    int GetWidth() const;
    int GetHeight() const;

private:
    void SetWindowIcon();

    // callbacks.
    void ProcessWindowSizeCallback(int width, int height);
    void ProcessKeyboardCallback(int key, int scancode, int action, int mods);
    void ProcessMouseCallback(double xPosIn, double yPosIn);
    void ProcessMouseScrollCallback(double xPosIn, double yPosIn);
};
}// namespace Game9

#endif
