#include "utility/Shader.h"

#include <GL/glew.h>
#include <climits>
#include <iostream>
#include <string>

Shader::Shader(unsigned int vertexID, unsigned int fragmentID):
m_vertexID(vertexID),
m_fragmentID(fragmentID)
{
    // TODO: how to properly handle an error here.
    if (m_programID = glCreateProgram(); !m_programID)
    {
        std::cerr << "Error occured creating program" << std::endl;
    }
}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
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

/*
 * Attach vertex and fragment to m_programID. Shader deletion is handled by
 * ShaderFileHandler in ResourceManager class.
 */
void Shader::CreateShader()
{
    glAttachShader(m_programID, m_vertexID);
    glAttachShader(m_programID, m_fragmentID);
    glLinkProgram(m_programID);
    glValidateProgram(m_programID);

    int linkresult, validateresult;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linkresult);
    glGetProgramiv(m_programID, GL_VALIDATE_STATUS, &validateresult);// debugging, heavy cost.
    if (linkresult == GL_FALSE || validateresult == GL_FALSE)
    {
        int loglength;
        std::string message;

        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &loglength);

        message.reserve(loglength);
        glGetShaderInfoLog(m_programID, loglength, &loglength, message.data());

        std::cerr << "Program failed to link/validate:\n\n"
                  << message << std::endl;
    }
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
        std::cerr << "SetUniform1i: Variable "<< name <<" Location Not Found" << std::endl;

    glUniform1i(var_location, value);
}

void Shader::SetUniform1iv(const std::string& name, int count, int* value)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::cerr << "SetUniform1iv: Variable "<< name <<" Location Not Found" << std::endl;

    glUniform1iv(var_location, count, value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::cerr << "SetUniform1f: Variable "<< name <<" Location Not Found" << std::endl;

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
        std::cerr << "SetUniform3fv: Variable "<< name <<" Location Not Found" << std::endl;

    glUniform3fv(var_location, count, value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::cerr << "SetUniform4f: Variable "<< name <<" Location Not Found" << std::endl;

    glUniform4f(var_location, v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::cerr << "SetUniformMat4f: Variable "<< name <<" Location Not Found" << std::endl;

    glUniformMatrix4fv(var_location, 1, GL_FALSE, &matrix[0][0]);
}
