#ifndef BOATCOMPONENT_H
#define BOATCOMPONENT_H

#include "core/world.h"

#include <glm/trigonometric.hpp>
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
        float _scale        {1.0f};

        float _rotation     {0.0f}; // direction.
        float _speed        {0.0f}; // current speed.
        float _acceleration {0.0f}; // current accel.

        float _rotationSpeed {glm::radians(90.0f)};
        float _maxSpeed      {25.0f};
        float _accelRate     {5.0f};
        float _drag          {0.98f}; // slow down speed.

        float _rotationInput {0.0f}; // user is currently pressing down on key.
        float _accelInput    {0.0f};

        BoatType _boatType  {BoatType::UNKNOWN};

    public:

        struct Vertex
        {
            glm::vec2 position;
            glm::vec2 texcoords;
        };

        using WorldComponent::WorldComponent;
        BoatComponent(Id id, std::string name, glm::vec3 position, BoatType boatType):
            WorldComponent::WorldComponent(id, name),
            _position(position),
            _boatType(boatType)
        {}

        virtual ~BoatComponent() = default;

        BoatComponent(const BoatComponent&)            = delete;
        BoatComponent& operator=(const BoatComponent&) = delete;
        BoatComponent(BoatComponent&&)                 = delete;
        BoatComponent& operator=(BoatComponent&&)      = delete;

        virtual void OnEvent(Event::Event&) override       {}
        virtual void OnUpdate(float deltaSeconds) override {}
        virtual void OnRender() const override             {}

        virtual void SetAccelerationInput(float dir) {_accelInput = dir;};
        virtual void SetRotationInput(float dir)     {_rotationInput = dir;};

        virtual glm::vec2 GetSize() const noexcept = 0;
        virtual glm::vec4 GetAABB() const noexcept = 0;
        virtual glm::vec3 GetPosition() const noexcept override {return _position;}
        virtual void SetPosition(const glm::vec3& pos) {_position = pos;}
        virtual BoatType GetType() const               {return _boatType;}
};
}// namespace World

#endif
