#ifndef BACKGROUNDLAYER_H
#define BACKGROUNDLAYER_H

#include "events/Events.h"
#include "events/MouseEvents.h"

#include "layers/Layer.h"

#include "managers/SceneHandler.h"
#include "managers/ResourceManager.h"

#include "core/window.h"

namespace Layer
{
struct DragState
{
    bool dragging{false};
    glm::vec2 lastMousePosition;
    glm::vec2 cachedMousePosition;
};
/*
* Draws texture displaying background image / scenery.
*/
class BackgroundLayer: public Layer::Layer
{
private:
    Manager::ResourceManager m_resourceManager;

    std::unique_ptr<Manager::SceneHandler> m_background;
    std::shared_ptr<Core::Window> m_window;
    DragState m_dragstate;

    bool mouseAABB(const glm::vec2& mousePosition, const glm::vec2& visibleMin, const glm::vec2& visibleMax);
    void UpdateProjection();

public:
    BackgroundLayer(std::shared_ptr<Core::Window> window);

    virtual void OnRender() override;
    virtual void OnEvent(Event::Event& event) override;

    bool OnMouseButtonPressed(Event::MouseButtonPressedEvent& event);
    bool OnMouseMoved(Event::MouseMovedEvent& event);
    bool OnMouseButtonReleased(Event::MouseButtonReleasedEvent& event);
};
}// namespace Layer
#endif
