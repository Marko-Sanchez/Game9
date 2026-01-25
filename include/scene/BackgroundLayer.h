#ifndef BACKGROUNDLAYER_H
#define BACKGROUNDLAYER_H

#include "scene/SceneHandler.h"
#include "ui/Event.h"
#include "ui/Layer.h"
#include "ui/MouseEvents.h"
#include "ui/window.h"
#include "utility/ResourceManager.h"

namespace Game9
{
struct DragState
{
    bool dragging{false};
    glm::vec2 grabOffset;
};
/*
* Draws texture displaying background image / scenery.
*/
class BackgroundLayer: public Core::Layer
{
private:
    Core::util::ResourceManager m_resourceManager;

    std::unique_ptr<Game9::SceneHandler> m_background;
    std::shared_ptr<Core::Window> m_window;
    DragState m_dragstate;

    bool mouseAABB(const glm::vec2& mousePosition, const glm::vec2& visibleMin, const glm::vec2& visibleMax);

public:
    BackgroundLayer(std::shared_ptr<Core::Window> window);

    virtual void OnRender() override;
    virtual void OnUpdate(float delta) override;
    virtual void OnEvent(Core::Event& event) override;

    bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);
    bool OnMouseMoved(Core::MouseMovedEvent& event);
    bool OnMouseButtonReleased(Core::MouseButtonReleasedEvent& event);
};
}
#endif
