#include "scene/SceneHandler.h"

namespace Game9
{
SceneHandler::SceneHandler(std::shared_ptr<Core::util::Shader> shader, std::shared_ptr<Core::Window> window, std::shared_ptr<Core::util::Texture2D> texture):
m_renderer(std::make_unique<Core::model::SpriteRenderer>(shader)),
m_window(window),
m_texture(texture)
{
    assert(m_renderer);
    assert(m_window);
    assert(m_texture);

    const auto [width, height] = m_window->GetFrameBufferSize();
    m_model.m_scale = glm::vec2(width * 2, height * 2);
}

void SceneHandler::SetPosition(const glm::vec2& position) noexcept
{
    m_model.m_position = position;
}

void SceneHandler::SetProjection(const glm::mat4& projection)
{
    assert(m_renderer && "Renderer must exist before setting projection.");
    m_renderer->UpdateProjection(projection);
}

glm::vec2 SceneHandler::GetPosition() const noexcept
{
    return m_model.m_position;
}

glm::vec2 SceneHandler::GetScale() const noexcept
{
    return m_model.m_scale;
}

void SceneHandler::Draw()
{
    assert(m_renderer && m_texture);
    m_renderer->DrawSprite(
            m_texture,
            m_model.m_position,
            m_model.m_scale,
            m_model.m_rotation
            );
}
}// namespace Game9
