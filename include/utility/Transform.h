#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/ext/vector_float2.hpp"
#include "glm/glm.hpp"

namespace Core::model
{
/*
 * Calculates the Model-View-Projection determining how model / object / sprite
 * is drawn.
 */
struct Transform
{
public:
    // Local space information.
    glm::vec2 position;
    glm::vec2 scale;

    float rotation;

    // Global space information.
    glm::mat4 parentMatrix;

    Transform();
    Transform(glm::mat4 globalmatrix, glm::vec2 pos, glm::vec2 scale, float rotation);

    glm::mat4 ComputeLocalModelMatrix();
    glm::mat4 ComputeParentModelMatrix();

    void SetParentMatrix(const glm::mat4& parentGlobalMatrix);
};
}// namespace Core::model
#endif
