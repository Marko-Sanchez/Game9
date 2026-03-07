#include "layers/TrainLayer.h"

#include "glm/ext/matrix_clip_space.hpp"

namespace Layer
{
TrainLayer::TrainLayer(std::shared_ptr<Core::Window> window):
m_window(window)
{
    const std::string_view idName{"train"};

    const std::string_view vertexShader{"resources/shaders/background.vertex"};
    const std::string_view fragmentShader{"resources/shaders/background.fragment"};

    auto shader = m_resourceManager.LoadShader(vertexShader, fragmentShader, idName);
    shader->Bind();
    shader->SetUniform1i("u_image", 1);

    m_trainHandler = std::make_unique<Manager::TrainHandler>(shader);

    m_trainHandler->LoadPaths();

    this->UpdateProjection();
}

void TrainLayer::OnRender()
{
    m_trainHandler->Draw();
}

void TrainLayer::OnEvent(Event::Event& event)
{
    Event::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Event::WindowResizedEvent>([this](Event::WindowResizedEvent& e){return OnWindowResized(e);});
}

void TrainLayer::OnUpdate(float delta)
{
    m_trainHandler->Update(delta);
}

void TrainLayer::UpdateProjection()
{
    if (!m_trainHandler)
    {
        return;
    }
    const auto [width, height] = m_window->GetFrameBufferSize();
    m_trainHandler->UpdateProjection(glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f));
}

bool TrainLayer::OnWindowResized(Event::WindowResizedEvent& event)
{
    this->UpdateProjection();
    return false;
}
}// namespace Layer
