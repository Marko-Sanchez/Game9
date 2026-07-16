#include "PlayerBoat.h"
#include "core/world.h"
#include "events/Events.h"
#include "events/KeyEvents.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GLFW/glfw3.h>

#include <filesystem>

namespace Entity
{
namespace
{
    const std::filesystem::path k_VertexShader   {"resources/shaders/boat.vert"};
    const std::filesystem::path k_FragmentShader {"resources/shaders/boat.frag"};
    const std::filesystem::path k_TexturePath    {"resources/images/world/PNG/Boat1_water_animation_color1/Boat1_water_frame1.png"};

    // TODO
    constexpr int k_TextureIndex {1};
    constexpr float k_ForwardOffset {-glm::half_pi<float>()};
    constexpr float k_QuadHalfWidth  {64.0f};
    constexpr float k_QuadHalfHeight {64.0f};

    constexpr std::array<World::BoatComponent::Vertex, 4> k_QuadVertices =
    {{
         {{-k_QuadHalfWidth, k_QuadHalfHeight},  {0.0f, 1.0f}},// top-left
         {{-k_QuadHalfWidth, -k_QuadHalfHeight}, {0.0f, 0.0f}},// bot-left
         {{k_QuadHalfWidth, -k_QuadHalfHeight},  {1.0f, 0.0f}},// bot-right
         {{k_QuadHalfWidth, k_QuadHalfHeight},   {1.0f, 1.0f}},// top-right
    }};

    constexpr std::array<GLuint, 6> k_IndexBuffer =
    {
        0, 1, 2, 2, 3, 0
    };

}// anonymous namespace

PlayerBoat::PlayerBoat(const std::string& playerName, const glm::vec3& position):
    World::BoatComponent(World::GenerateComponentId(), playerName, position, World::BoatType::USER),
    m_shader(k_VertexShader, k_FragmentShader),
    m_texture(k_TexturePath, k_TextureIndex)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(World::BoatComponent::Vertex) * k_QuadVertices.size(), k_QuadVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(World::BoatComponent::Vertex), reinterpret_cast<void*>(offsetof(World::BoatComponent::Vertex, position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(World::BoatComponent::Vertex), reinterpret_cast<void*>(offsetof(World::BoatComponent::Vertex, texcoords)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * k_IndexBuffer.size(), k_IndexBuffer.data(), GL_STATIC_DRAW);

    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, _position);
    // m_model = glm::scale(m_model, glm::vec3(_scale, _scale, _scale));
    m_projection = glm::ortho(0.0f, 1024.0f, 0.0f, 1024.0f, -0.1f, 100.0f);
    m_view = glm::lookAt
        (
         glm::vec3(0.0f, 0.0f, 50.0f),
         glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3(0.0f, 1.0f, 0.0f)
        );

    m_shader.Bind();
    m_texture.Bind();

    m_shader.SetUniformMat4f("u_Model", m_model);
    m_shader.SetUniformMat4f("u_Projection", m_projection);
    m_shader.SetUniformMat4f("u_View", m_view);
    m_shader.SetUniform1i("u_Texture", k_TextureIndex);

    m_texture.UnBind();
    m_shader.UnBind();
}

PlayerBoat::~PlayerBoat()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void PlayerBoat::OnEvent(Event::Event& event)
{
    Event::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Event::KeyPressedEvent>([this](Event::KeyPressedEvent& e){return OnKeyPressed(e);});
    dispatcher.Dispatch<Event::KeyReleasedEvent>([this](Event::KeyReleasedEvent& e){return OnKeyReleased(e);});
}

void PlayerBoat::OnUpdate(float deltaSeconds)
{
    _rotation += _rotationInput * _rotationSpeed * deltaSeconds;

    _speed += _accelInput * _accelRate * deltaSeconds;
    _speed = glm::clamp(_speed, -_maxSpeed * 0.5f, _maxSpeed);

    if (_accelInput == 0.0f)
    {
        _speed *= _drag;
        if (glm::abs(_speed) < 0.01f)
        {
            _speed = 0.0f;
        }
    }

    auto angle = _rotation + k_ForwardOffset;
    glm::vec3 forward (std::cos(angle), std::sin(angle), 0.0f);
    _position += forward * _speed * deltaSeconds;

    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, _position);
    // m_model = glm::scale(m_model, glm::vec3(_scale, _scale, _scale));
    m_model = glm::rotate(m_model, _rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}

void PlayerBoat::OnRender() const
{
    m_shader.Bind();
    m_shader.SetUniformMat4f("u_Model", m_model);

    m_texture.Bind();
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, k_IndexBuffer.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    m_texture.UnBind();

    m_shader.UnBind();
}

glm::vec2 PlayerBoat::GetSize() const noexcept
{
    return glm::vec2 {k_QuadHalfWidth * 2.0f, k_QuadHalfHeight * 2.0f} * _scale;
}

glm::vec4 PlayerBoat::GetAABB() const noexcept
{
    auto halfSize = GetSize() * 0.5f;

    std::array<glm::vec2, 4> corners =
    {{
        {-halfSize.x, halfSize.y},
        {-halfSize.x, -halfSize.y},
        {halfSize.x, -halfSize.y},
        {halfSize.x, halfSize.y},
    }};

    const float cos {std::cos(_rotation)};
    const float sin {std::sin(_rotation)};

    glm::vec2 minB {std::numeric_limits<float>::max()};
    glm::vec2 maxB {-std::numeric_limits<float>::max()};

    for (auto& corner: corners)
    {
        glm::vec2 rotated
        {
            corner.x * cos - corner.y * sin,
            corner.x * sin + corner.y * cos
        };
        auto world = glm::vec2(_position) + rotated;

        minB = glm::min(minB, world);
        maxB = glm::max(maxB, world);
    }

    return glm::vec4
    {
        minB.x, // left.
        minB.y, // bottom
        maxB.x, // right
        maxB.y  // top
    };
}

bool PlayerBoat::OnKeyPressed(const Event::KeyPressedEvent& e)
{
    if (e.m_keyCode == GLFW_KEY_UP)
    {
        SetAccelerationInput(1.0f);
    }
    else if (e.m_keyCode == GLFW_KEY_DOWN)
    {
        SetAccelerationInput(-0.5f);
    }
    else if (e.m_keyCode == GLFW_KEY_LEFT)
    {
        SetRotationInput(-1.0f);
    }
    else if (e.m_keyCode == GLFW_KEY_RIGHT)
    {
        SetRotationInput(1.0f);
    }

    return true;
}

bool PlayerBoat::OnKeyReleased(const Event::KeyReleasedEvent& e)
{
    if (e.m_keyCode == GLFW_KEY_UP || e.m_keyCode == GLFW_KEY_DOWN)
    {
        SetAccelerationInput(0.0f);
    }
    else if (e.m_keyCode == GLFW_KEY_LEFT || e.m_keyCode == GLFW_KEY_RIGHT)
    {
        SetRotationInput(0.0f);
    }

    return true;
}

}// namespace Entity
