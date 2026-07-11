#ifndef OCEANMAP_H
#define OCEANMAP_H

#include "core/compositecomponent.h"
#include "renderer/Shader.h"
#include "renderer/Texture2D.h"

namespace OceanMap
{
class OceanMapComposite final: public World::CompositeComponent
{
public:

    struct Vertex
    {
        glm::vec2 positions;
        glm::vec2 texcoords;
    };

    struct Instances
    {
        glm::vec2 translation;
    };

private:

    Renderer::Shader    m_shader;
    Renderer::Texture2D m_texture;

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_instanceVBO;

    std::vector<Instances> m_translations;

    void GenerateTranslations();

public:

    OceanMapComposite(std::string name = "OceanMap");
    ~OceanMapComposite();

    OceanMapComposite(const OceanMapComposite&)            = delete;
    OceanMapComposite(OceanMapComposite&&) noexcept        = delete;
    OceanMapComposite& operator=(const OceanMapComposite&) = delete;
    OceanMapComposite& operator=(OceanMapComposite&&)      = delete;

    void OnEvent(Event::Event& event) override;
    void OnUpdate(float deltaSeconds) override;
    void OnRender() const override;
};
}// namespace OceanMap

#endif
