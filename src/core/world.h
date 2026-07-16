#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <atomic>
#include <vector>

#include <glm/glm.hpp>

#include "events/Events.h"

namespace World
{

using ComponentId = std::uint64_t;

inline ComponentId GenerateComponentId()
{
    static std::atomic<ComponentId> next {1};
    return next++;
}


/*
 * Shared interface for anything that lives in the world. Design Pattern: Composite.
 */
class WorldComponent
{
    public:

        using Id = ComponentId;

        explicit WorldComponent(Id id, std::string name):
            _id(id), _name(std::move(name)) {}

        virtual ~WorldComponent() = default;

        virtual void OnEvent(Event::Event&) {}
        virtual void OnUpdate(float deltaSeconds = 0) {}
        virtual void OnRender() const {}

        virtual void AddChildren(std::shared_ptr<WorldComponent>) {}
        virtual void RemoveChildren(Id) {}

        virtual const std::vector<std::shared_ptr<WorldComponent>>& GetChildren() const
        {
            static const std::vector<std::shared_ptr<WorldComponent>> empty;
            return empty;
        };

        virtual glm::vec3 GetPosition() const noexcept = 0;

        Id GetId() const                   {return _id;}
        const std::string& GetName() const {return _name;}

    protected:

        Id _id;
        std::string _name;
};
}// namespace World

#endif
