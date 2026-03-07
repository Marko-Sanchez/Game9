#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <memory>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <GL/glew.h>

#include "utility/Transform.h"

#include "renderer/Shader.h"
#include "renderer/Texture2D.h"

namespace Renderer
{
struct Vertex
{
    glm::vec2 position;
    glm::vec2 uvs;
};
/*
* Manages a Sprite using a given shader and allows modification to its' model matrix.
* Creates a default Quad when constructed and draws a 2D texture.
*/
class SpriteRenderer
{
private:
    std::shared_ptr<Renderer::Shader> m_shader;
    unsigned int m_vao;
    unsigned int m_vbo;

public:
    SpriteRenderer(std::shared_ptr<Renderer::Shader> shader);
    ~SpriteRenderer();

    void DrawSprite(std::shared_ptr<Renderer::Texture2D> texture, glm::vec2 position = glm::vec2(0.0f), glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f);
    void DrawSprite(std::shared_ptr<Renderer::Texture2D> texture, Utility::Transform transform);

    void UpdateProjection(const glm::mat4& projection);

private:
    void Draw();
};
}// namespace Renderer
#endif
