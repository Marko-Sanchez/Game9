#ifndef GAME_H
#define GAME_H

#include "utility/SpriteRenderer.h"

#include <GLFW/glfw3.h>

#include <memory>

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
private:
    int m_width;
    int m_height;

    GameState m_state;

    std::shared_ptr<GLFWwindow> m_window;

public:
    Game(std::shared_ptr<GLFWwindow> window, int width, int height);
    ~Game();
    // intialize game state (load all shaders/textures).
    void Init();
    // Game loop.
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();
};
#endif
