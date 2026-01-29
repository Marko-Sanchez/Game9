#ifndef TRAIN_LAYER_H
#define TRAIN_LAYER_H

#include <memory>

#include "ui/Layer.h"
#include "entity/TrainHandler.h"
#include "ui/WindowEvents.h"
#include "utility/ResourceManager.h"
#include "ui/window.h"

namespace Game9
{
/*
 * Draws trains together as a layer above background layer.
 */
class TrainLayer : public Core::Layer
{
private:
    Core::util::ResourceManager m_resourceManager;
    std::shared_ptr<Core::Window> m_window;
    std::unique_ptr<Game9::TrainHandler> m_trainHandler;

public:
    TrainLayer(std::shared_ptr<Core::Window> window);

    virtual void OnRender() override;
    virtual void OnEvent(Core::Event& event) override;
    virtual void OnUpdate(float delta) override;

    void UpdateProjection();

    bool OnWindowResized(Core::WindowResizedEvent& e);
};
}// namespace Game9
#endif
