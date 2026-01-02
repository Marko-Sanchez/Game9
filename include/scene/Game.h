#ifndef GAME_H
#define GAME_H

#include <concepts>
#include <list>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ui/Event.h"
#include "ui/Layer.h"
#include "ui/window.h"

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

    std::list<std::unique_ptr<Layer>> m_layerStack;

public:
    Game(const ApplicationSpecification& specification = ApplicationSpecification());
    ~Game();

    // main game loop.
    void Run();

    void Render();
    void Update(float deltaTime);
    void RaiseEvent(Event &event);

    // Share window specification with layers.
    std::shared_ptr<Window> GetWindow() noexcept;

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
