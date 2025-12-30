#ifndef SCENE_HANDLER
#define SCENE_HANDLER

#include <memory>

#include "glm/fwd.hpp"
#include "utility/Transform.h"
#include "utility/SpriteRenderer.h"
#include "utility/Texture2D.h"

#include "ui/window.h"

namespace Game9
{
/*
* Handles drawing of the background / map. TODO: handles drawing large images like the background
* menu and such. unlike what other handlers that specialize in drawing the trains, etc.
*/
class SceneHandler
{
private:
    // math unknown yet.
    float m_offSetWidth;
    float m_offSetHeight;

    // Initial: Model position, Model size, parent model = projection ?
    Core::model::Transform m_model;

    std::unique_ptr<Core::model::SpriteRenderer> m_renderer;

    std::shared_ptr<Core::Window> m_window;
    std::shared_ptr<Texture2D> m_texture;

public:
    SceneHandler(std::shared_ptr<Shader> sceneshader, std::shared_ptr<Core::Window> window, std::shared_ptr<Texture2D> texture);

    void Draw();
    void UpdateModel(const Core::model::Transform& model);
    void UpdateProjection(const glm::mat4 projection);
};
}
#endif
