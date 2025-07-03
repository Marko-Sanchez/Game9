#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <GL/glew.h>

#include "utility/Shader.h"
#include "utility/Texture2D.h"

class SpriteRenderer
{
private:
    Shader m_shader;
    unsigned int m_vao;

public:
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();

    void DrawSprite(Texture2D &texture, glm::vec2 position = glm::vec2(0.0f), glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 10.0f,
                    glm::vec3 color = glm::vec3(1.0f));

private:
    void initRenderData();
};
#endif
