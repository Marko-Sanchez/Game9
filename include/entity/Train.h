#ifndef TRAIN_H
#define TRAIN_H

#include "utility/SpriteRenderer.h"
#include "utility/Texture2D.h"
#include <glm/glm.hpp>

namespace Game9
{
// Default class for basic trains, holds texture and model data.
class Train
{
private:
    glm::vec2 m_position;
    glm::vec2 m_size;
    glm::vec2 m_velocity;

    float m_rotation;

    Texture2D m_sprite;
public:
    Train();
    Train(Texture2D texture, glm::vec2 pos, glm::vec2 size, glm::vec2 velocity);

    virtual void Draw(SpriteRenderer &renderer);
};
}
#endif
