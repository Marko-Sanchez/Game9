#ifndef TRAIN_LAYER_H
#define TRAIN_LAYER_H

#include <memory>

#include "events/WindowEvents.h"
#include "layers/Layer.h"

#include "managers/TrainHandler.h"
#include "managers/ResourceManager.h"

#include "core/window.h"

namespace Layer
{
/*
 * Draws trains together as a layer above background layer.
 */
class TrainLayer : public Layer::Layer
{
private:
    Manager::ResourceManager m_resourceManager;
    std::shared_ptr<Core::Window> m_window;
    std::unique_ptr<Manager::TrainHandler> m_trainHandler;

public:
    TrainLayer(std::shared_ptr<Core::Window> window);

    virtual void OnRender() override;
    virtual void OnEvent(Event::Event& event) override;
    virtual void OnUpdate(float delta) override;

    void UpdateProjection();

    bool OnWindowResized(Event::WindowResizedEvent& e);
};
}// namespace Layer
#endif
