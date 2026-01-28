#include "utility/Transform.h"
#include "glm/ext/matrix_transform.hpp"

namespace Core::model
{
Transform::Transform()
{
    position = {0.0f, 0.0f};
    scale    = {1.0f, 1.0f};

    rotation = 0.0f;
    parentMatrix = {1.0f};
}

Transform::Transform(glm::mat4 parentMatrix, glm::vec2 pos, glm::vec2 scale, float rotation):
parentMatrix(parentMatrix), position(pos), scale(scale), rotation(rotation)
{}

glm::mat4 Transform::ComputeLocalModelMatrix()
{
    glm::mat4 model;

    // position.
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // rotation.
    model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));  // move quad origin from bottom-left to center.
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate.
    model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));// move origin back to bottom-left.

    // scale.
    model = glm::scale(model, glm::vec3(scale, 1.0f));

    return model;
}

glm::mat4 Transform::ComputeParentModelMatrix()
{
    return parentMatrix * this->ComputeLocalModelMatrix();
}

void Transform::SetParentMatrix(const glm::mat4& parentMatrix)
{
    this->parentMatrix = parentMatrix;
}
}// namespace Core::model
