#ifndef SCENE_HANDLER
#define SCENE_HANDLER

#include <memory>

#include <glm/fwd.hpp>

#include "utility/Transform.h"

#include "renderer/SpriteRenderer.h"
#include "renderer/Texture2D.h"

#include "core/window.h"

namespace Manager
{
/*
* Handles drawing of the background / map. Handles drawing large images like the background
* menu and such; unlike what other handlers that specialize in drawing the trains, etc.
*/
class SceneHandler
{
private:
    std::unique_ptr<Renderer::SpriteRenderer> m_renderer;
    std::shared_ptr<Core::Window> m_window;
    std::shared_ptr<Renderer::Texture2D> m_texture;

    Utility::Transform m_model;

public:
    SceneHandler(std::shared_ptr<Renderer::Shader> sceneshader, std::shared_ptr<Core::Window> window, std::shared_ptr<Renderer::Texture2D> texture);

    void SetPosition(const glm::vec2& position) noexcept;
    void SetProjection(const glm::mat4& projection);

    glm::vec2 GetPosition() const noexcept;
    glm::vec2 GetScale() const noexcept;

    void Draw();
};
}// namespace Manager
#endif
