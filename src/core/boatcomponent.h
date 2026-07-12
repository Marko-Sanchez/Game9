#ifndef BOATCOMPONENT_H
#define BOATCOMPONENT_H

#include "core/world.h"

#include <glm/ext/vector_float3.hpp>

namespace World
{
enum class BoatType
{
    USER,
    FRIENDLY,
    NEUTRAL,
    ENEMY,
    UNKNOWN
};

class BoatComponent: public WorldComponent
{
    protected:

        glm::vec3 _position {0.0f, 0.0f, 0.0f};
        BoatType _boatType  {BoatType::UNKNOWN};

    public:

        struct Vertex
        {
            glm::vec2 position;
            glm::vec2 texcoords;
        };

        using WorldComponent::WorldComponent;

        virtual ~BoatComponent() = default;

        BoatComponent(const BoatComponent&)            = delete;
        BoatComponent& operator=(const BoatComponent&) = delete;
        BoatComponent(BoatComponent&&)                 = delete;
        BoatComponent& operator=(BoatComponent&&)      = delete;

        virtual void OnEvent(Event::Event&) override       {}
        virtual void OnUpdate(float deltaSeconds) override {}
        virtual void OnRender() const override             {}

        virtual glm::vec3 GetPosition() const override {return _position;}
        virtual void SetPosition(const glm::vec3& pos) {_position = pos;}
        virtual BoatType GetType() const               {return _boatType;}
};
}// namespace World

#endif
