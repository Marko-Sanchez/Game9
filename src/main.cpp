#include <GL/gl.h>
#include <cstdlib>
#include <iostream>
#include <memory>

#include <GLFW/glfw3.h>

int main()
{
    std::cout << "Hello World" << std::endl;

    if (!glfwInit())
    {
        std::cerr << "Failed to Initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    std::shared_ptr<GLFWwindow> window(glfwCreateWindow(1080, 1080, "GAME 9", nullptr, nullptr), glfwDestroyWindow);
    if (!window.get())
    {
        std::cerr << "Window failed to Create" << std::endl;

        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Make Window Current Context, set swap interval to wait for 1 screen update before swapping buffers.
    glfwMakeContextCurrent(window.get());
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window.get()))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Swap Front buffer (currently being displayed) and back buffer (next to render).
        glfwSwapBuffers(window.get());

        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
