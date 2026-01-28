#include "scene/BackgroundLayer.h"

#include <algorithm>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/matrix.hpp"
#include "ui/MouseEvents.h"
#include <GLFW/glfw3.h>
#include <cassert>

namespace Game9
{
/*
 * Create Shader, load textures, and set shader arguments.
 */
BackgroundLayer::BackgroundLayer(std::shared_ptr<Core::Window> window):
m_window(window)
{
    const std::string_view idName{"background"};

    const std::string_view vertexShader{"resources/shaders/background.vertex"};
    const std::string_view fragmentShader{"resources/shaders/background.fragment"};

    const std::string_view backgroundTexture{"resources/images/background.png"};

    auto shader = m_resourceManager.LoadShader(vertexShader, fragmentShader, idName);
    auto texture = m_resourceManager.LoadTexture(backgroundTexture, idName, 0);

    shader->Bind();
    shader->SetUniform1i("u_image", 0);

    m_background = std::make_unique<SceneHandler>(shader, m_window, texture);

    this->UpdateProjection();
}

void BackgroundLayer::OnRender()
{
    m_background->Draw();
}

void BackgroundLayer::OnEvent(Core::Event& event)
{
    Core::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e){return OnMouseButtonPressed(e);});
    dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e){return OnMouseMoved(e);});
    dispatcher.Dispatch<Core::MouseButtonReleasedEvent>([this](Core::MouseButtonReleasedEvent& e){return OnMouseButtonReleased(e);});
}

/*
 * Axis-Aligned Bounding Box. Checks if mouse position is within object bounds.
 *
 * @params
 * mousePosition: Position of mouse.
 * visibleMin: The viewable minimum of objects position (bottom-left of texture) and window minimum size {0, 0}.
 * visibleMax: The viewable maximum of objects position (bottom-left of texture + object size) and window max size (ex. {1024, 1024}).
 */
bool BackgroundLayer::mouseAABB(const glm::vec2& mousePosition, const glm::vec2& visibleMin, const glm::vec2& visibleMax)
{
    // completely off the map.
    if (visibleMin.x >= visibleMax.x || visibleMin.y >= visibleMax.y)
    {
        return false;
    }

    bool collisionX = visibleMin.x <= mousePosition.x &&
                      mousePosition.x <= visibleMax.x;
    bool collisionY = visibleMin.y <= mousePosition.y &&
                      mousePosition.y <= visibleMax.y;

    return collisionX && collisionY;
}

/*
 * On window resize, update projection.
 */
void BackgroundLayer::UpdateProjection()
{
    if (!m_background)
    {
        return;
    }

    const auto [width, height] = m_window->GetFrameBufferSize();

    m_background->SetProjection(glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f));
}

bool BackgroundLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{
    if (event.GetMouseButton() != GLFW_MOUSE_BUTTON_LEFT)
    {
        return false;
    }

    m_dragstate.dragging = true;
    m_dragstate.lastMousePosition = m_dragstate.cachedMousePosition;

    return true;
}

/*
 * Moves texture when mouse is dragged.
 *
 * @note:
 * Window / mouse coordinates (glfw) set top-left as the origin (0,0) point. While
 * opengl / object coordinates set the bottom-left as origin; Therefore, in-order to align
 * mouse y-axis with object coordinates we subtract it with window max height.
 */
bool BackgroundLayer::OnMouseMoved(Core::MouseMovedEvent& event)
{
    // caching mouse position avoids jumps in texture and allows continous dragging.
    auto [winWidth, winHeight] = m_window->GetFrameBufferSize();
    auto mousePosition = glm::vec2{static_cast<float>(event.GetX()), winHeight - static_cast<float>(event.GetY())};
    m_dragstate.cachedMousePosition = mousePosition;

    if (!m_dragstate.dragging)
    {
        return false;
    }


    auto objectPosition = m_background->GetPosition();
    auto size  = m_background->GetScale();

    auto delta = mousePosition - m_dragstate.lastMousePosition;
    objectPosition.x += delta.x * 0.9f;
    objectPosition.y += delta.y * 0.9f;

    objectPosition.x = std::clamp(objectPosition.x, winWidth - size.x, 0.0f);
    objectPosition.y = std::clamp(objectPosition.y, winHeight - size.y, 0.0f);

    // min-max range of texture.
    glm::vec2 objectMin = objectPosition;
    glm::vec2 objectMax = objectPosition + size;

    // clamp bounds to window size.
    glm::vec2 visibleMin =
    {
        std::max(objectMin.x, 0.0f),
        std::max(objectMin.y, 0.0f)
    };
    glm::vec2 visibleMax =
    {
        std::min(objectMax.x, winWidth),
        std::min(objectMax.y, winHeight)
    };

    if (mouseAABB(mousePosition, visibleMin, visibleMax))
    {
        m_background->SetPosition(objectPosition);
    }
    m_dragstate.lastMousePosition = mousePosition;

    return true;
}

bool BackgroundLayer::OnMouseButtonReleased(Core::MouseButtonReleasedEvent& event)
{
    if (event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
    {
        m_dragstate.dragging = false;
    }
    return true;
}
}
