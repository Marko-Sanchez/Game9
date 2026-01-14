#include "scene/SceneHandler.h"

namespace Game9
{
SceneHandler::SceneHandler(std::shared_ptr<Core::util::Shader> sceneshader, std::shared_ptr<Core::Window> window, std::shared_ptr<Core::util::Texture2D> texture):
m_window(window),
m_texture(texture)
{
    m_renderer = std::make_unique<Core::model::SpriteRenderer>(sceneshader);

    m_model.m_scale = glm::vec2(static_cast<float>(m_window->GetWidth() * 2), static_cast<float>(m_window->GetHeight() * 2));
}

void SceneHandler::SetPosition(const glm::vec2& position) noexcept
{
    m_model.m_position = position;
}

void SceneHandler::SetProjection(const glm::mat4& projection) noexcept
{
    m_renderer->UpdateProjection(projection);
}

glm::vec2 SceneHandler::GetPosition() const noexcept
{
    return m_model.m_position;
}

glm::vec2 SceneHandler::GetSize() const noexcept
{
    return m_model.m_scale;
}

void SceneHandler::Draw()
{
    m_renderer->DrawSprite(m_texture, m_model.m_position, m_model.m_scale, m_model.m_rotation);
}
}
