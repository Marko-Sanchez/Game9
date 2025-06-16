#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <memory>

namespace Game9
{
/*
* Handles window context, glfw callbacks, and renders the game.
*/
class Window
{
private:
    int m_width;
    int m_height;

    std::shared_ptr<GLFWwindow> m_window;

    // callbacks.
    void ProcessKeyboardCallback(int key, int scancode, int action, int mods);
    void ProcessMouseCallback(double xPosIn, double yPosIn);
    void ProcessMouseScrollCallback(double xPosIn, double yPosIn);
    void ProcessWindowSizeCallback(int width, int height);

public:
    Window(std::shared_ptr<GLFWwindow> window, int width, int height);

    void Render();

    int GetWidth() const;
    int GetHeight() const;
};
}// namespace Game9

#endif
