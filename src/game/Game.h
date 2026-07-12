#ifndef GAME_H
#define GAME_H

#include <concepts>
#include <list>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "events/Events.h"
#include "core/world.h"
#include "core/window.h"

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
    std::shared_ptr<Window>  m_window;

    std::list<std::unique_ptr<World::WorldComponent>> m_layerStack;

public:

    Game(const ApplicationSpecification& specification = ApplicationSpecification());
    ~Game();

    // main game loop.
    void Run();

    void Render();
    void Update(float deltaTime);
    void RaiseEvent(Event::Event &event);

    // Share window specification with layers.
    std::shared_ptr<Window> GetWindow() noexcept;

    template<typename TLayer, typename ...Args>
    requires(std::derived_from<TLayer, World::WorldComponent>)
    void PushLayer(Args&&... args)
    {
        m_layerStack.push_back(std::make_unique<TLayer>(std::forward<Args>(args)...));
    }
};
}// namespace Core

#endif
