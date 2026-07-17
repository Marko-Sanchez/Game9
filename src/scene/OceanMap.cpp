#include "OceanMap.h"

#include <cmath>
#include <cstddef>
#include <filesystem>
#include <array>
#include <vector>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "entity/PlayerBoat.h"
#include "renderer/Texture2D.h"


namespace OceanMap
{
namespace
{
    const std::filesystem::path k_VertShader  {"resources/shaders/oceanmap.vert"};
    const std::filesystem::path k_FragShader  {"resources/shaders/oceanmap.frag"};
    const std::filesystem::path k_TexturePath {"resources/images/oceantile.png"};

    constexpr int k_TextureIndex {0};

    constexpr std::array<OceanMapComposite::Vertex, 6> k_QuadVertices =
    {{
         {{-64.0f, 64.0f},  {0.0f, 1.0f}},// top-left
         {{-64.0f, -64.0f}, {0.0f, 0.0f}},// bot-left
         {{64.0f, -64.0f},  {1.0f, 0.0f}},// bot-right
         {{64.0f, 64.0f},   {1.0f, 1.0f}},// top-right
    }};

    constexpr std::array<GLuint, 6> k_IndexBuffer =
    {
        0, 1, 2, 2, 3, 0
    };
}// anonymous namespace

OceanMapComposite::OceanMapComposite(std::string name):
    World::CompositeComponent(0, std::move(name)),
    m_shader(k_VertShader, k_FragShader),
    m_texture(k_TexturePath, k_TextureIndex)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(OceanMapComposite::Vertex) * k_QuadVertices.size(), k_QuadVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, positions)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcoords)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * k_IndexBuffer.size(), k_IndexBuffer.data(), GL_STATIC_DRAW);

    GenerateTranslations();

    glGenBuffers(1, &m_instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Instances) * m_translations.size(), m_translations.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Instances), reinterpret_cast<void*>(offsetof(Instances, translation)));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    auto model = glm::mat4(1.0f);
    auto projection = glm::ortho(0.0f, 1024.0f, 0.0f, 1024.0f, -.1f, 100.0f);
    auto view = glm::lookAt
        (
         glm::vec3(0.0f, 0.0f, 50.0f),
         glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3(0.0f, 1.0f, 0.0f)
        );

    m_shader.Bind();
    m_texture.Bind();

    m_shader.SetUniformMat4f("u_Model", model);
    m_shader.SetUniformMat4f("u_Projection", projection);
    m_shader.SetUniformMat4f("u_View", view);
    m_shader.SetUniform1i("u_Texture", k_TextureIndex);

    m_shader.UnBind();

    // Add boat children.
    World::CompositeComponent::AddChildren(std::make_shared<Entity::PlayerBoat>("thechurchofbob", glm::vec3(512.0f, 512.0f, 0.0f)));
}

OceanMapComposite::~OceanMapComposite()
{
    glDeleteBuffers(1, &m_instanceVBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void OceanMapComposite::OnEvent(Event::Event& event)
{
    World::CompositeComponent::OnEvent(event);
}

void OceanMapComposite::OnUpdate(float deltaSeconds)
{
    World::CompositeComponent::OnUpdate(deltaSeconds);
}

void OceanMapComposite::OnRender() const
{
    m_shader.Bind();
    glBindVertexArray(m_VAO);
    glDrawElementsInstanced(GL_TRIANGLES, k_IndexBuffer.size(), GL_UNSIGNED_INT, nullptr, m_translations.size());
    glBindVertexArray(0);
    m_shader.UnBind();

    World::CompositeComponent::OnRender();
}

void OceanMapComposite::GenerateTranslations()
{
    const float windowHeight {1024.0f};
    const float windowWidth {1024.0f};

    const float quadSize {128.0f};

    const size_t total {static_cast<size_t>(std::ceil(windowWidth / quadSize))};
    m_translations.reserve(total * total);

    for (int y {0}; y < windowHeight; y+= quadSize)
    {
        for (int x {0}; x < windowWidth; x+=quadSize)
        {
            m_translations.emplace_back(Instances{{x + quadSize * 0.5f, y + quadSize *  0.5f}});
        }
    }
}

}// namespace OceanMap
