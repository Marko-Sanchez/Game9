#ifndef LAYER_H
#define LAYER_H

#include <concepts>
#include <memory>
#include <utility>

namespace Core
{
/*
* Base class provides template for derived classes that will handle drawing different
* scenes / layers, such as: main menu, background, overlay, etc.
*/
class Layer
{
    private:
    void QueueTransition(std::unique_ptr<Layer> newLayer);

    public:
    virtual ~Layer() = default;

    virtual void OnEvent() {}
    virtual void OnUpdate(float delta) {}
    virtual void OnRender() {}

    template <std::derived_from<Layer> T, typename... Args>
    void TransitionTo(Args&&... args)
    {
        QueueTransition(std::unique_ptr<T>(std::forward<Args>(args)...));
    }
};
}// namespace Core

#endif
