#include "scene/SceneHandler.h"

namespace Game9
{
SceneHandler::SceneHandler(std::shared_ptr<Shader> sceneshader, std::shared_ptr<Core::Window> window, std::shared_ptr<Texture2D> texture):
m_window(window),
m_texture(texture)
{
    m_renderer = std::make_unique<SpriteRenderer>(sceneshader);

    auto windowspec = m_window->GetWindowSpecification();
    m_model.m_scale = glm::vec2(static_cast<float>(windowspec.width * 2), static_cast<float>(windowspec.height * 2));
}

void SceneHandler::Draw()
{
    m_renderer->DrawSprite(m_texture, m_model.m_position, m_model.m_scale, m_model.m_rotation);
}

void SceneHandler::UpdateModel(const Transform& model)
{
    m_model = model;
}

void SceneHandler::UpdateProjection(const glm::mat4 projection)
{
    m_renderer->UpdateProjection(projection);
}
}
