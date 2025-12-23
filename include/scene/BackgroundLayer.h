#ifndef BACKGROUNDLAYER_H
#define BACKGROUNDLAYER_H

#include "scene/SceneHandler.h"
#include "ui/Layer.h"
#include "ui/window.h"
#include "utility/ResourceManager.h"

namespace Game9
{
/*
* Draws texture displaying background image / scenery.
*/
class BackgroundLayer: public Core::Layer
{
private:
    Core::util::ResourceManager m_resourceManager;

    std::unique_ptr<Game9::SceneHandler> m_background;
    std::shared_ptr<Core::Window> m_window;
public:
    BackgroundLayer(std::shared_ptr<Core::Window> window);
    virtual ~BackgroundLayer() override;

    virtual void OnRender() override;
    virtual void OnUpdate(float delta) override;
};
}
#endif
