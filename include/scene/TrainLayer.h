#ifndef TRAIN_LAYER_H
#define TRAIN_LAYER_H

#include "ui/Layer.h"
#include "entity/TrainHandler.h"
#include "utility/ResourceManager.h"
#include "ui/window.h"

#include <memory>

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
    std::shared_ptr<Core::model::SpriteRenderer> m_entityRenderer;

public:
    TrainLayer(std::shared_ptr<Core::Window> window);

    virtual ~TrainLayer() override;
    virtual void OnRender() override;
    virtual void OnUpdate(float delta) override;

};
}// namespace Game9
#endif
