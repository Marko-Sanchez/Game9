#include <GL/glew.h>
#include <GL/gl.h>

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <memory>

#include "ui/window.h"
int main()
{
    using namespace Game9;

    constexpr int WINDOW_WIDTH{1080};
    constexpr int WINDOW_HEIGHT{1080};

    if (!glfwInit())
    {
        std::cerr << "Failed to Initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    std::shared_ptr<GLFWwindow> glfw_window(glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GAME 9", nullptr, nullptr), glfwDestroyWindow);
    if (!glfw_window.get())
    {
        std::cerr << "Window failed to Create" << std::endl;

        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make Window Current Context, set swap interval to wait for 1 screen update before swapping buffers.
    glfwMakeContextCurrent(glfw_window.get());
    glfwSwapInterval(1);

    if (GLenum err = glewInit(); err != GLEW_OK)
    {
        std::cerr << "Failed to Initialize GLEW\nNeeds a valid OpenGL Context\n";
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return EXIT_FAILURE;
    }

    Window window(glfw_window, WINDOW_WIDTH, WINDOW_HEIGHT);
    while (!glfwWindowShouldClose(glfw_window.get()))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // window.render();

        // Swap Front buffer (currently being displayed) and back buffer (next to render).
        glfwSwapBuffers(glfw_window.get());
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
