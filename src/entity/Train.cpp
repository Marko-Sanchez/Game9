#include "entity/Train.h"

#include "glm/geometric.hpp"

namespace Game9
{
Train::Train():
m_direction(Direction::FORWARD),
m_texture(nullptr),
m_position(0.0f, 0.0f), m_size(256.0f, 256.0f), m_velocity(0.0f, 0.0f), m_rotation(0.0f),
m_segmentProgress(0.0f), m_currentSegment(0)
{
    m_speed = glm::length(m_velocity);
}

Train::Train(std::shared_ptr<Core::util::Texture2D> texture, glm::vec2 size, glm::vec2 velocity):
m_direction(Direction::FORWARD),
m_texture(texture),
m_position(0.0f, 0.0f), m_size(size), m_velocity(velocity), m_rotation(0.0f),
m_segmentProgress(0.0f), m_currentSegment(0)
{
    m_speed = glm::length(m_velocity);
}

/*
* Moves the train along a defined path; once either ends are reached, changes direction.
*/
void Train::Travel(float deltaTime)
{
    if (m_path.size() < 2)
    {
        return;
    }

    // direction of travel.
    const int dir{static_cast<int>(m_direction)};

    // Interpolation variable 't'.
    const auto& pointA{m_path[m_currentSegment]};
    const auto& pointB{m_path[m_currentSegment + dir]};

    float abLength{glm::distance(pointA, pointB)};
    // deltaTime * velocity is also something we did in OpenGLTest.
    float abTravel{m_speed * deltaTime};

    // percent to travel.
    float progress{abTravel / abLength};
    m_segmentProgress += progress;

    // Current segmented completed.
    if (m_segmentProgress >= 1.0f)
    {
        m_segmentProgress -= 1.0f;// perserve remainder, suppose: 1.1f - 1.0f
        m_currentSegment += dir;

        // Reached the end, set to last point and turn around.
        bool atStart{m_currentSegment == 0};
        bool atEnd{m_currentSegment == m_path.size() - 1};

        if (atStart || atEnd)
        {
            m_position = (m_currentSegment == m_path.size() - 1) ? m_path.back() : m_path.front();
            m_direction = (m_direction == Direction::FORWARD) ? Direction::REVERSE : Direction::FORWARD;
            return;
        }
    }

    // linear interpolation.
    const auto& currPoint{m_path[m_currentSegment]};
    const auto& nextPoint{m_path[m_currentSegment + static_cast<int>(m_direction)]};

    m_position = glm::mix(currPoint, nextPoint, m_segmentProgress);
}

/*
* Set path of travel and resets progress variables to defaults.
*
* @params
* path: array of coordinates train will follow.
*/
void Train::SetPath(const std::vector<glm::vec2>& path)
{
    if (path.size() < 2)
    {
        return;
    }

    m_path = path;
    m_currentSegment = 0;
    m_segmentProgress = 0;
    m_direction = Direction::FORWARD;
    m_position = m_path.front();
}
}// namespace Game9
