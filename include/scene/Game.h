#ifndef GAME_H
#define GAME_H

#include "entity/TrainHandler.h"
#include "ui/Layer.h"
#include "utility/SpriteRenderer.h"
#include "scene/SceneHandler.h"

#include "ui/window.h"
#include <GLFW/glfw3.h>

#include <concepts>
#include <list>
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

    // temporary will be moved into class layer, so that each layer handles it's on resources.
    util::ResourceManager m_resourceManager;

    std::unique_ptr<Game9::TrainHandler> m_trainHandler;

    // Renderers.
    std::shared_ptr<SpriteRenderer> entityRenderer;

    std::list<std::unique_ptr<Layer>> m_layerStack;

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

    // Share window specification with layers.
    std::shared_ptr<Window> GetWindow();

    template<typename TLayer, typename ...Args>
    requires(std::derived_from<TLayer, Layer>)
    void PushLayer(Args&&... args)
    {
        m_layerStack.push_back(std::make_unique<TLayer>(std::forward<Args>(args)...));
    }

    friend class Layer;
};
}// namespace Core
#endif
