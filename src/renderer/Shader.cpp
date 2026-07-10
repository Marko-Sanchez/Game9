#include "renderer/Shader.h"

#include <print>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <fstream>

#include <GL/glew.h>

namespace Renderer
{
/*
 * Parse and Compile Shader.
 */
Shader::Shader(const std::filesystem::path& vertex, const std::filesystem::path& fragment)
{
    auto vertexID   = CompileShader(GL_VERTEX_SHADER, ParseShaderFile(vertex));
    auto fragmentID = CompileShader(GL_FRAGMENT_SHADER, ParseShaderFile(fragment));

    if (m_programID = glCreateProgram(); !m_programID)
    {
        throw std::runtime_error("glCreateProgram failed.");
    }

    glAttachShader(m_programID, vertexID);
    glAttachShader(m_programID, fragmentID);
    glLinkProgram(m_programID);

    glValidateProgram(m_programID);

    int linkStatus;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        int loglength;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &loglength);

        std::string message(loglength, '\0');
        glGetProgramInfoLog(m_programID, 1024, &loglength, message.data());

        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);

        throw std::runtime_error(std::string{"Program failed to link/validate: "} + message);
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

Shader::~Shader()
{
    if (m_programID != 0)
    {
        glDeleteProgram(m_programID);
    }
}

Shader::Shader(Shader&& other) noexcept
{
    if (m_programID != 0)
    {
        glDeleteProgram(m_programID);
    }

    m_programID            = other.m_programID;
    m_uniformLocationCache = std::move(other.m_uniformLocationCache);

    other.m_programID = 0;
    other.m_uniformLocationCache.clear();
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        if (m_programID != 0)
        {
            glDeleteProgram(m_programID);
        }

        m_programID            = other.m_programID;
        m_uniformLocationCache = std::move(other.m_uniformLocationCache);

        other.m_programID = 0;
        other.m_uniformLocationCache.clear();
    }

    return *this;
}

/*
* Reads contents from file and returns as a string.
*/
std::string Shader::ParseShaderFile(const std::filesystem::path& filepath)
{
    std::ifstream ifs(filepath, std::ios_base::in | std::ios_base::binary);
    if (!ifs)
    {
        throw std::ios_base::failure(std::format("Failed to open file for reading: {}", filepath.string()));
    }

    std::string contents;

    std::error_code ec;
    if (const auto size = std::filesystem::file_size(filepath, ec); !ec)
    {
        contents.reserve(static_cast<size_t>(size));
    }

    std::array<char, 1024> buffer;
    while (ifs.read(buffer.data(), buffer.size()))
    {
        contents.append(buffer.data(), buffer.size());
    }
    contents.append(buffer.data(), ifs.gcount());

    return contents;
}

/*
 * Sets the shader source code to an ID: glShaderSource(),
 * then compiles the source code: glCompileShader().
 *
 * @param:
 * unsigned int {type}: type of shader; vertex or fragment.
 * const std::string {source}: file contents.
 */
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    auto id {glCreateShader(type)};

    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // check if shader compiled successfully, else print log error.
    int result {};
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        std::string message;
        message.reserve(length);
        glGetShaderInfoLog(id, length, &length, message.data());

        glDeleteShader(id);

        throw std::runtime_error(std::format("Failed to compile {} shader: {}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message));
    }

    return id;
}

/*
 * Returns Shader program ID. If function returns 0, shader has no been created or error occured.
 */
unsigned int Shader::GetID() const noexcept
{
    return m_programID;
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
}// namespace Renderer
