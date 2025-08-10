#ifndef GAME_H
#define GAME_H

#include "entity/Train.h"
#include "utility/SpriteRenderer.h"

#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

#include "ui/window.h"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
private:
    GameState m_state;
    Game9::Window m_window;

    std::vector<Game9::Train> m_trains;

public:
    Game(std::shared_ptr<GLFWwindow> window, int width, int height);
    ~Game();
    // intialize game state (load all shaders/textures).
    void Init();
    // Game loop.
    void Render();
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
};
#endif
