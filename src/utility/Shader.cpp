#include "utility/Shader.h"

#include <GL/glew.h>
#include <print>
#include <cstdio>
#include <stdexcept>
#include <string>

namespace Core::util
{
/*
 * Attach vertex and fragment to m_programID. Shader deletion is handled by
 * ShaderFileHandler in ResourceManager class.
 */
Shader::Shader(unsigned int vertexID, unsigned int fragmentID):
m_vertexID(vertexID),
m_fragmentID(fragmentID)
{
    if (m_programID = glCreateProgram(); !m_programID)
    {
        throw std::runtime_error("glCreateProgram failed.");
    }

    glAttachShader(m_programID, m_vertexID);
    glAttachShader(m_programID, m_fragmentID);
    glLinkProgram(m_programID);

    glValidateProgram(m_programID);

    int linkStatus;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        char log[1024] = {};
        int loglength{};

        glGetProgramInfoLog(m_programID, 1024, &loglength, log);

        throw std::runtime_error(std::string{"Program failed to link/validate: "} + log);
    }
}

Shader::~Shader()
{
    if (m_programID != 0)
    {
        glDeleteProgram(m_programID);
    }
}

/*
 * Returns Shader program ID. If function returns 0, shader has no been created or error occured.
 */
unsigned int Shader::GetID() const noexcept
{
    return m_programID;
}

/*
 * Returns pair of shader file IDs.
 */
std::pair<unsigned int, unsigned int> Shader::GetFileIDs() const noexcept
{
    // Copy Elison: Creation of a class object from source object can be ommited.
    return {m_vertexID, m_fragmentID};
}

void Shader::Bind() const
{
   glUseProgram(m_programID);
}

void Shader::UnBind() const
{
    glUseProgram(0);
}

// looks up uniform in cache, if not found does a expensive glGetUniformLocation() look up.
int Shader::GetUniformLocation(const std::string& name)
{
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end())
        return it->second;

    int uLocation{glGetUniformLocation(m_programID, name.c_str())};
    if (uLocation != -1)
        m_uniformLocationCache[name] = uLocation;

    return uLocation;
}

// glUniform1i and glUniform1iv are the only two functions that may be used to load uniform variables defined as sampler types.
void Shader::SetUniform1i(const std::string& name, int value)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std:println(stderr, "SetUniform1i: Variable {} Location Not Found", name);

    glUniform1i(var_location, value);
}

void Shader::SetUniform1iv(const std::string& name, int count, int* value)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::println(stderr, "SetUniform1iv: Variable {} Location Not Found", name);

    glUniform1iv(var_location, count, value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::println(stderr, "SetUniform1f: Variable {} Location Not Found", name);

    glUniform1f(var_location, value);
}

/*
* Upload three contigous floats in an array. Takes a {count} amount of glm::vec3, float [3],
* float[3*x].
*/
void Shader::SetUniform3fv(const std::string& name, const int count, const GLfloat* value)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::println(stderr, "SetUniform3fv: Variable {} Location Not Found", name);

    glUniform3fv(var_location, count, value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::println(stderr, "SetUniform4f: Variable {} Location Not Found", name);

    glUniform4f(var_location, v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::println(stderr, "SetUniformMat4f: Variable {} Location Not Found", name);

    glUniformMatrix4fv(var_location, 1, GL_FALSE, &matrix[0][0]);
}
}// namespace Core::util
