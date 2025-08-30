#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/ext/vector_float2.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

namespace Game9
{
class Transform
{
public:
    // Local space information.
    glm::vec2 m_position;
    glm::vec2 m_scale;

    float m_rotation;

    // Global space information.
    glm::mat4 m_globalMatrix;

    Transform();
    Transform(glm::mat4 globalmatrix, glm::vec2 pos, glm::vec2 scale, float rotation);

    glm::mat4 ComputeLocalModelMatrix();
    glm::mat4 ComputeGlobalModelMatrix();

    void SetGlobalMatrix(const glm::mat4& parentGlobalMatrix);
};
}
#endif
