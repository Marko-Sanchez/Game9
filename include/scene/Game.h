#ifndef GAME_H
#define GAME_H

#include "entity/TrainHandler.h"
#include "utility/SpriteRenderer.h"
#include "scene/SceneHandler.h"

#include "ui/window.h"
#include <GLFW/glfw3.h>

#include <memory>

namespace Core
{
struct ApplicationSpecification
{
    std::string name = "Application";
    WindowSpecification windowspec = WindowSpecification();
};

// Application.
class Game
{
private:
    ApplicationSpecification m_specification;
    std::shared_ptr<Window> m_window;
    bool m_isRunning = false;

    std::unique_ptr<Game9::SceneHandler> m_background;
    std::unique_ptr<Game9::TrainHandler> m_trainHandler;

    // Renderers.
    std::shared_ptr<SpriteRenderer> entityRenderer;

public:
    Game(const ApplicationSpecification& specification = ApplicationSpecification());
    ~Game();

    void Run();
    // intialize game state (load all shaders/textures).
    void Init();
    // Game loop.
    void Render();
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
};
}// namespace Core
#endif
