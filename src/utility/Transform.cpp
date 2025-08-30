#include "utility/Transform.h"
#include "glm/ext/matrix_transform.hpp"

namespace Game9
{
Transform::Transform()
{
    m_position = {0.0f, 0.0f};
    m_scale    = {1.0f, 1.0f};

    m_rotation = 0.0f;
    m_parentMatrix = {1.0f};
}

Transform::Transform(glm::mat4 parentMatrix, glm::vec2 pos, glm::vec2 scale, float rotation):
m_parentMatrix(parentMatrix), m_position(pos), m_scale(scale), m_rotation(rotation)
{}

glm::mat4 Transform::ComputeLocalModelMatrix()
{
    glm::mat4 model;

    // position.
    model = glm::translate(model, glm::vec3(m_position, 0.0f));

    // rotation.
    model = glm::translate(model, glm::vec3(0.5f * m_scale.x, 0.5f * m_scale.y, 0.0f));  // move quad origin from bottom-left to center.
    model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate.
    model = glm::translate(model, glm::vec3(-0.5f * m_scale.x, -0.5f * m_scale.y, 0.0f));// move origin back to bottom-left.

    // scale.
    model = glm::scale(model, glm::vec3(m_scale, 1.0f));

    return model;
}

glm::mat4 Transform::ComputeParentModelMatrix()
{
    return m_parentMatrix * this->ComputeLocalModelMatrix();
}

void Transform::SetParentMatrix(const glm::mat4& parentMatrix)
{
    m_parentMatrix = parentMatrix;
}
}
