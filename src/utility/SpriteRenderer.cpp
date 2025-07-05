#include "utility/SpriteRenderer.h"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <GL/gl.h>

/* Constructor */
SpriteRenderer::SpriteRenderer(std::shared_ptr<Shader> shader):
m_shader(shader)
{
    initRenderData();
}

/*
 * Deletes Vertex Array Object.
 */
SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &m_vao);
}

/*
 * Draws the given texture using the intialized Shader. Allows for model position, size, and rotation movement.
 *
 * @params:
 *
 * texture: 2D texture to draw.
 * position: position to move texture to.
 * size: increase or decrease texture.
 * rotate: rotate texture.
 */
void SpriteRenderer::DrawSprite(std::shared_ptr<Texture2D> texture, glm::vec2 position, glm::vec2 size, float rotate)
{
    m_shader->Bind();

    glm::mat4 model{1.0f};

    // position.
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // rotation.
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));  // move quad origin from bottom-left to center.
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate.
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));// move origin back to bottom-left.

    // scale.
    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_shader->SetUniformMat4f("u_model", model);

    // TODO: Use texture slot from Texture2D.
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

/*
* Initializes our quad between (0,0) and (1,1). Creates VBO and VAO, uploads data to GPU.
* Uses one vertex attribute of 4 floats, to represent position on the x-y axis and
* texture coordinates uv.
*/
void SpriteRenderer::initRenderData()
{
    unsigned int VBO{};
    const float vertices[] =
    {
        // Texture coordinates are (0,0) at the bottom-left, (1,1) at top-right.
        // pos      // tex-cordinate
        0.0f, 1.0f, 0.0f, 1.0f, // top-left.
        0.0f, 0.0f, 0.0f, 0.0f, // bottom-left.
        1.0f, 0.0f, 1.0f, 0.0f, // bottom-right.

        0.0f, 1.0f, 0.0f, 1.0f, // top left.
        1.0f, 0.0f, 1.0f, 0.0f, // bottom-right.
        1.0f, 1.0f, 1.0f, 1.0f  // top-right.
    };

    // Bind.
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));

    // UnBind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
