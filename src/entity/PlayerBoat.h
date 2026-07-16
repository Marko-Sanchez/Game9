#ifndef PLAYERBOAT_H
#define PLAYERBOAT_H

#include "core/boatcomponent.h"

#include "events/Events.h"
#include "events/KeyEvents.h"

#include "renderer/Shader.h"
#include "renderer/Texture2D.h"

namespace Entity
{
class PlayerBoat: public World::BoatComponent
{
    private:

        GLuint m_VAO;
        GLuint m_VBO;
        GLuint m_EBO;

        glm::mat4 m_model;
        glm::mat4 m_view;
        glm::mat4 m_projection;

        Renderer::Shader    m_shader;
        Renderer::Texture2D m_texture;

        bool OnKeyPressed(const Event::KeyPressedEvent& event);
        bool OnKeyReleased(const Event::KeyReleasedEvent& event);

    public:

        PlayerBoat(const std::string& playerName, const glm::vec3& position);
        ~PlayerBoat();

        void OnEvent(Event::Event&) override;
        void OnUpdate(float deltaSeconds) override;
        void OnRender() const override;

        glm::vec2 GetSize() const noexcept override;
        glm::vec4 GetAABB() const noexcept override;
};
}// namespace Entity

#endif
