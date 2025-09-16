#include "scene/SceneHandler.h"

namespace Game9
{
SceneHandler::SceneHandler(SpriteRenderer renderer, std::shared_ptr<Window> window, std::shared_ptr<Texture2D> texture):
m_renderer(renderer),
m_window(window),
m_texture(texture)
{
    m_model.m_scale = glm::vec2(static_cast<float>(m_window->GetWidth() * 2), static_cast<float>(m_window->GetHeight() * 2));
}

void SceneHandler::Draw()
{
    m_renderer.DrawSprite(m_texture, m_model.m_position, m_model.m_scale, m_model.m_rotation);
}

void SceneHandler::UpdateModel(const Transform& model)
{
    m_model = model;
}

void SceneHandler::UpdateProjection(const glm::mat4 projection)
{
    m_renderer.UpdateProjection(projection);
}
}
