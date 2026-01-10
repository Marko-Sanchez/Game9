#include "scene/TrainLayer.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace Game9
{
TrainLayer::TrainLayer(std::shared_ptr<Core::Window> window):
m_window(window)
{
    const std::string_view idName{"train"};

    const std::string_view vertexShader{"resources/shaders/background.vertex"};
    const std::string_view fragmentShader{"resources/shaders/background.fragment"};

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_window->GetWindowSpecification().width),
                                      0.0f, static_cast<float>(m_window->GetWindowSpecification().height),
                                      -1.0f, 1.0f);

    m_resourceManager.LoadShader(vertexShader,fragmentShader, idName);
    m_resourceManager.GetShader(idName)->Bind();
    m_resourceManager.GetShader(idName)->SetUniform1i("u_image", 1);// Todo: hard-coded
    m_resourceManager.GetShader(idName)->SetUniformMat4f("u_projection", projection);

    // TrainHandler loads train textures and paths; EntityRenderer draws the trains.
    m_trainHandler = std::make_unique<Game9::TrainHandler>(m_resourceManager.GetShader("train"));
    m_trainHandler->LoadPaths();
}

TrainLayer::~TrainLayer()
{}

void TrainLayer::OnRender()
{
    m_trainHandler->Draw();
}

void TrainLayer::OnUpdate(float delta)
{
    m_trainHandler->Update(delta);
}
}
