#include "scene/BackgroundLayer.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/matrix.hpp"
#include "ui/MouseEvents.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdlib>

#include <print>
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

    m_background->SetProjection(
    glm::ortho(w * 0.5f - hw, w * 0.5f + hw,
               h * 0.5f - hh, h * 0.5f + hh,
               -1.0f, 1.0f));

    m_background->Draw();
}

void BackgroundLayer::OnUpdate(float delta)
{

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

bool BackgroundLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{
    if (event.GetMouseButton() != GLFW_MOUSE_BUTTON_LEFT)
    {
        return false;
    }
    m_dragstate.dragging = true;

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
    if (!m_dragstate.dragging)
    {
        return false;
    }

    auto winWidth  = m_window->GetWidth();
    auto winHeight = m_window->GetHeight();
    auto objectPosition   = m_background->GetPosition();
    auto size      = m_background->GetSize();

    auto mouseX = static_cast<float>(event.GetX());
    auto mouseY = winHeight - static_cast<float>(event.GetY());

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

    if (mouseAABB(glm::vec2{mouseX, mouseY}, visibleMin, visibleMax))
    {
        // TODO: temporary sets texture to the top-right.
        std::println("In AABB");
        m_background->SetPosition({512.0f, 512.0f});
    }

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
