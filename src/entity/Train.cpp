#include "entity/Train.h"

#include "glm/geometric.hpp"

namespace Game9
{
Train::Train():
m_sprite(nullptr), m_position(4.0f, 4.0f), m_size(256.0f, 256.0f), m_velocity(0.0f, 0.0f), m_rotation(0.0f), m_segmentProgess(0.0f), m_currentSegment(0)
{}

Train::Train(std::shared_ptr<Texture2D> texture, glm::vec2 pos, glm::vec2 size, glm::vec2 velocity):
m_sprite(texture), m_position(pos), m_size(size), m_velocity(velocity), m_rotation(0.0f), m_segmentProgess(0.0f), m_currentSegment(0)
{}

Train::~Train()
{}

// Draws sprite using shader in SpriteRenderer object.
void Train::Draw(SpriteRenderer* renderer)
{
    renderer->DrawSprite(m_sprite, m_position, m_size, m_rotation);
}

// Moves the train on a defined path.
void Train::Move(float deltaTime)
{
    if (m_path.size() < 2 || m_currentSegment >= m_path.size() - 1)
    {
        return;
    }

    // Interpolation variable 't'.
    const auto& pointA{m_path[m_currentSegment]};
    const auto& pointB{m_path[m_currentSegment + 1]};

    float abLength{glm::distance(pointA, pointB)};
    // deltaTime * velocity is also something we did in OpenGLTest.
    float abTravel{glm::length(m_velocity) * deltaTime};

    // percent to travel.
    float progress{abTravel / abLength};
    m_segmentProgess += progress;

    if (m_segmentProgess >= 1)
    {
        m_segmentProgess = 0;
        ++m_currentSegment;

        if (m_currentSegment >= m_path.size() - 1)
        {
            m_position = m_path.back();
            return;
        }
    }

    // linear interpolation.
    if (m_currentSegment < m_path.size() - 1)
    {
        const auto& currPoint{m_path[m_currentSegment]};
        const auto& nextPoint{m_path[m_currentSegment + 1]};

        m_position = glm::mix(currPoint, nextPoint, m_segmentProgess);
    }
}

// Set points to which train should travel to.
void Train::SetPath(const std::vector<glm::vec2>& path)
{
    m_path = path;
}
}// namespace Game9
