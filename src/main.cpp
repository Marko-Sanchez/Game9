#include <GL/glew.h>
#include <GL/gl.h>

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>

#include "scene/Game.h"

int main()
{
    using namespace Game9;

    constexpr int WINDOW_WIDTH{1024};
    constexpr int WINDOW_HEIGHT{1024};

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

    // TODO: setup icon here:?
    //
    //

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make Window Current Context, set swap interval to wait for 1 screen update before swapping buffers.
    glfwMakeContextCurrent(glfw_window.get());
    glfwSwapInterval(1);

    // OpenGL configuration.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (GLenum err = glewInit(); err != GLEW_OK)
    {
        std::cerr << "Failed to Initialize GLEW\nNeeds a valid OpenGL Context\n";
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return EXIT_FAILURE;
    }

    Game game9(glfw_window, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialize game (load shaders/textures).
    game9.Init();

    float lastFrame{};
    float deltaTime{};

    while (!glfwWindowShouldClose(glfw_window.get()))
    {
        // Calculate delta time and processes callbacks.
        float currentFrame{static_cast<float>(glfwGetTime())};
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input.
        game9.ProcessInput(deltaTime);

        // TODO: update game state.
        // game9.Update();

        // Render.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        game9.Render();

        // Swap Front buffer (currently being displayed) and back buffer (next to render).
        glfwSwapBuffers(glfw_window.get());
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
