#ifndef SCENE_HANDLER
#define SCENE_HANDLER

#include "glm/ext/vector_float2.hpp"
#include "glm/fwd.hpp"
#include "utility/Transform.h"
#include "utility/SpriteRenderer.h"
#include "utility/Texture2D.h"

#include "ui/window.h"

namespace Game9
{
/*
* Handles drawing of the background / map.
*/
class SceneHandler
{
private:
    // math unknown yet.
    float m_offSetWidth;
    float m_offSetHeight;

    // Initial: Model position, Model size, parent model = projection ?
    Transform m_model;

    SpriteRenderer m_renderer;

    std::shared_ptr<Window> m_window;
    std::shared_ptr<Texture2D> m_texture;

public:
    SceneHandler(SpriteRenderer renderer, std::shared_ptr<Window> window, std::shared_ptr<Texture2D> texture);

    void Draw();
    void UpdateModel(const Transform& model);
    void UpdateProjection(const glm::mat4 projection);
};
}
#endif
