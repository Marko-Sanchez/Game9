#ifndef COMPOSITECOMPONENT_H
#define COMPOSITECOMPONENT_H

#include "world.h"

#include <memory>
#include <vector>

namespace World
{
/*
 * Base class for composites.
 */
class CompositeComponent: public WorldComponent
{
    protected:

        glm::vec3 _origin{0.0f, 0.0f, 0.0f};
        std::vector<std::shared_ptr<WorldComponent>> _children;

    public:

        // Inherit WorldComponent constructors.
        using WorldComponent::WorldComponent;

        void OnUpdate(float deltaSeconds) override
        {
            for (const auto& child: _children)
            {
                child->OnUpdate(deltaSeconds);
            }
        }

        void OnRender() const override
        {
            for (const auto& child: _children)
            {
                child->OnRender();
            }
        }

        void AddChildren(std::shared_ptr<WorldComponent> child) override
        {
            _children.push_back(std::move(child));
        }

        void RemoveChildren(Id childId) override
        {
            for (auto iter = _children.begin(); iter != _children.end(); ++iter)
            {
                if ((*iter)->GetId() == childId)
                {
                    _children.erase(iter);
                }
            }
        }

        glm::vec3 GetPosition() const override
        {
            return _origin;
        }
};
}// World

#endif
