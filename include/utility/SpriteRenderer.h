#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <GL/glew.h>

#include <memory>

#include "glm/fwd.hpp"
#include "utility/Shader.h"
#include "utility/Texture2D.h"
#include "utility/Transform.h"

namespace Core::model
{
/*
* Manages a Sprite using a given shader and allows modification to its' model matrix.
* Creates a default Quad when constructed and draws a 2D texture.
*/
class SpriteRenderer
{
private:
    std::shared_ptr<Shader> m_shader;
    unsigned int m_vao;

public:
    SpriteRenderer(std::shared_ptr<Shader> shader);
    ~SpriteRenderer();

    void DrawSprite(std::shared_ptr<Texture2D> texture, glm::vec2 position = glm::vec2(0.0f), glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f);
    void DrawSprite(std::shared_ptr<Texture2D> texture, Transform transform);

    void UpdateProjection(const glm::mat4& projection);

private:
    void initRenderData();
    void Draw();
};
}// namespace Core::model
#endif
