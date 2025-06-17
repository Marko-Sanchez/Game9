#include "scene/Camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Game9
{
Camera::Camera():
m_yawAngle(-90.0f), m_pitchAngle(0.0f), m_cameraSpeed(3.0f), m_mouseSensitivty(0.01f), m_fov(45.0f),
m_worldUp(std::move(glm::vec3(0.0f, 1.0f, 0.0f))),
m_cameraPosition(std::move(glm::vec3(0.0f, 0.0f, 5.0f)))
{
    UpdateCameraVectors();
}

/*
 * Calculates camera front (direction camera is facing) from pitch and yaw.
 * Followed by camera right and camera up.
 */
void Camera::UpdateCameraVectors()
{
    // Yaw Angles (left-right): (x: cos(yaw), z: sin(yaw))
    // Pitch Angles (up-down): (x: cos(pitch), y: sin(pitch))
    glm::vec3 front (
        cos(glm::radians(m_yawAngle)) * cos(glm::radians(m_pitchAngle)),
        sin(glm::radians(m_pitchAngle)),
        sin(glm::radians(m_yawAngle)) * cos(glm::radians(m_pitchAngle))
    );
    m_cameraFront = glm::normalize(front);

    m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_worldUp));
    m_cameraUp    = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
}

/*
 * Updates camera position from camera movement. Invoked from a function that handles
 * system keyboard input (glfwGetKey).
 *
 * @params
 * {CAMERA_MOVEMENT} cameramovement: camera movement.
 * {float} deltaTime: difference in time between last frame and current frame.
 */
void Camera::ProcessKeyboardInput(int key, float deltaTime)
{
    // helps balance slow (larger delta) and fast fps (smaller delta) computers.
    float movementSpeed{deltaTime * m_cameraSpeed};

    switch(key)
    {
        case GLFW_KEY_UP:
            m_cameraPosition += m_cameraFront * movementSpeed;
            break;
        case GLFW_KEY_DOWN:
            m_cameraPosition -= m_cameraFront * movementSpeed;
            break;
        case GLFW_KEY_RIGHT:
            m_cameraPosition += glm::normalize(m_cameraRight) * movementSpeed;
            break;
        case GLFW_KEY_LEFT:
            m_cameraPosition -= glm::normalize(m_cameraRight) * movementSpeed;
            break;
        default:
             break;
    }
}

/*
* Handles camera angles pitch (up/down) and yaw (left/right). Limits
* camera pitch to avoid issues with glm::lookAt at angles above 90 degrees and
* angles below -90 degrees. Invoked from mouse callback function.
*
* @params
* {float} xoffset: mouse x value offset from center of window.
* {float} yoffset: mouse y value offset from center of window.
*/
void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= m_mouseSensitivty;
    yoffset *= m_mouseSensitivty;

    m_yawAngle += xoffset;
    m_pitchAngle += yoffset;

    // Avoid glm::lookAt() flip that occurs at 90*.
    if (m_pitchAngle > 89.0f)
    {
        m_pitchAngle = 89.0f;
    }
    else if (m_pitchAngle < -89.0f)
    {
        m_pitchAngle = -89.0f;
    }
    UpdateCameraVectors();
}

/*
 * Uses the scroll wheel to handle glm::perspective fov or 'zoom'.
 */
void Camera::ProcessMouseScroll(float yoffset)
{
    m_fov -= yoffset;
    if (m_fov < 1.0f)
    {
        m_fov = 1.0f;
    }
    if (m_fov > 45.0f)
    {
        m_fov = 45.0f;
    }
}

/*
 * Returns value of Field of view. Used in glm:perspective().
 */
float Camera::GetZoom() const
{
    return m_fov;
}

/*
* Returns view matrix.
*/
glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
}

/*
* Returns camera position.
*/
glm::vec3 Camera::GetCameraPos() const
{
    return m_cameraPosition;
}
}// namespace Game9
