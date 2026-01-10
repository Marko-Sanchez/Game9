#ifndef TRAIN_H
#define TRAIN_H

#include "utility/SpriteRenderer.h"
#include "utility/Texture2D.h"
#include <glm/glm.hpp>

#include <memory>

namespace Game9
{
// Holds model data and reference to a texture for this given train.
struct Train
{
    enum class Direction: int
    {
        FORWARD = 1,
        REVERSE = -1
    };

    Direction m_direction;

    // Train Texture.
    std::shared_ptr<Core::util::Texture2D> m_texture;

    // Train Model data.
    glm::vec2 m_position;
    glm::vec2 m_size;
    glm::vec2 m_velocity;

    float m_rotation;

    // Current index along path, and progress to next index.
    float m_segmentProgess;
    int m_currentSegment;

    // Train path to follow using linear interpolation.
    std::vector<glm::vec2> m_path;

    Train();
    Train(std::shared_ptr<Core::util::Texture2D> texture, glm::vec2 pos, glm::vec2 size, glm::vec2 velocity);
    ~Train();

    virtual void Draw(std::shared_ptr<Core::model::SpriteRenderer> rend);
    virtual void Travel(float deltaTime);

    virtual void SetPath(const std::vector<glm::vec2>& path);
};
}// namespace Game9
#endif
