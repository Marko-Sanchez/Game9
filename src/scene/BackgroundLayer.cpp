#include "scene/BackgroundLayer.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace Game9
{
/*
 * Create Shader, load textures, and set shader argurments.
 */
BackgroundLayer::BackgroundLayer(std::shared_ptr<Core::Window> window):
m_window(window)
{
    const std::string_view idName{"background"};

    const std::string_view vertexShader{"resources/shaders/background.vertex"};
    const std::string_view fragmentShader{"resources/shaders/background.fragment"};

    const std::string_view backgroundTexture{"resources/images/background.png"};

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_window->GetWidth()),
                                      0.0f, static_cast<float>(m_window->GetHeight()),
                                      -1.0f, 1.0f);

    m_resourceManager.LoadShader(vertexShader, fragmentShader, idName);
    m_resourceManager.GetShader(idName)->Bind();
    m_resourceManager.GetShader(idName)->SetUniform1i("u_image", 0);
    m_resourceManager.GetShader(idName)->SetUniformMat4f("u_projection", projection);

    m_background = std::make_unique<SceneHandler>(m_resourceManager.GetShader(idName),
                                                         m_window,
                                                         m_resourceManager.LoadTexture(backgroundTexture, idName, 0));
}

/*
 * Updates projection based on zoom parameters.
 */
void BackgroundLayer::OnRender()
{
    const float w{static_cast<float>(m_window->GetWidth())};
    const float h{static_cast<float>(m_window->GetHeight())};

    const float hw{(w * m_window->GetZoom()) * 0.5f};
    const float hh{(h * m_window->GetZoom()) * 0.5f};

    m_background->UpdateProjection(
    glm::ortho(w * 0.5f - hw, w * 0.5f + hw,
               h * 0.5f - hh, h * 0.5f + hh,
               -1.0f, 1.0f));

    m_background->Draw();
}

void BackgroundLayer::OnUpdate(float delta)
{

}
}
