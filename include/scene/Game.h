#ifndef GAME_H
#define GAME_H

#include "entity/TrainHandler.h"
#include "utility/SpriteRenderer.h"

#include <GLFW/glfw3.h>

#include "ui/window.h"

#include <memory>

enum class GameState
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

    std::unique_ptr<Game9::TrainHandler> m_trainHandler;

    // Renderers.
    std::unique_ptr<SpriteRenderer> sceneRenderer;
    std::shared_ptr<SpriteRenderer> entityRenderer;

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
