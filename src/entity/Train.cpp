#include "entity/Train.h"

#include "glm/geometric.hpp"

namespace Game9
{
Train::Train():
m_direction(Direction::FORWARD), m_sprite(nullptr), m_position(4.0f, 4.0f), m_size(256.0f, 256.0f), m_velocity(0.0f, 0.0f), m_rotation(0.0f), m_segmentProgess(0.0f), m_currentSegment(0)
{}

Train::Train(std::shared_ptr<Core::util::Texture2D> texture, glm::vec2 pos, glm::vec2 size, glm::vec2 velocity):
m_direction(Direction::FORWARD), m_sprite(texture), m_position(pos), m_size(size), m_velocity(velocity), m_rotation(0.0f), m_segmentProgess(0.0f), m_currentSegment(0)
{}

Train::~Train()
{}

/*
 * Draws sprite using shader in SpriteRenderer object.
 */
void Train::Draw(std::shared_ptr<Core::model::SpriteRenderer> rend)
{
    rend->DrawSprite(m_sprite, m_position, m_size, m_rotation);
}

/*
* Moves the train along a defined path; once either ends are reached, changes direction.
*/
void Train::Travel(float deltaTime)
{
    // Interpolation variable 't'.
    const auto& pointA{m_path[m_currentSegment]};
    const auto& pointB{m_path[m_currentSegment + static_cast<int>(m_direction)]};

    float abLength{glm::distance(pointA, pointB)};
    // deltaTime * velocity is also something we did in OpenGLTest.
    float abTravel{glm::length(m_velocity) * deltaTime};

    // percent to travel.
    float progress{abTravel / abLength};
    m_segmentProgess += progress;

    // Current segmented completed.
    if (m_segmentProgess >= 1)
    {
        m_segmentProgess = 0;
        m_currentSegment += static_cast<int>(m_direction);

        // Reached the end, set to last point and turn around.
        if (m_currentSegment == m_path.size() - 1 || m_currentSegment == 0)
        {
            m_position = m_currentSegment == m_path.size() - 1 ? m_path.back() : m_path.front();
            m_direction = m_direction == Direction::FORWARD ? Direction::REVERSE : Direction::FORWARD;
            return;
        }
    }

    // linear interpolation.
    const auto& currPoint{m_path[m_currentSegment]};
    const auto& nextPoint{m_path[m_currentSegment + static_cast<int>(m_direction)]};

    m_position = glm::mix(currPoint, nextPoint, m_segmentProgess);
}

/*
* Set points to which train should travel to.
*/
void Train::SetPath(const std::vector<glm::vec2>& path)
{
    if (path.size() < 2) return;

    m_path = path;
}
}// namespace Game9
