#include "utility/Shader.h"

#include <GL/glew.h>

#include <climits>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ostream>
#include <string>
#include <system_error>

Shader::Shader():
m_programID(0)
{}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
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
 * Parse shader file, attaches vertex and fragment shader; Links them together.
 */
void Shader::LoadShader(const std::string_view& vertexPath, const std::string_view& fragmentPath)
{
    m_vertexshader = ParseShaderFile(vertexPath);
    m_fragmentshader = ParseShaderFile(fragmentPath);

    CreateShader();
}

/*
 * Read contents of file into a std::string variable.
 */
std::string Shader::ParseShaderFile(const std::string_view& filepath)
{
    std::error_code ec;
    if (!std::filesystem::exists(filepath, ec))
    {
        std::cerr << "File " << filepath << " does not exits...\n" <<
            ec.message() << std::endl;
        return "";
    }

    std::ifstream ifs(filepath.data());
    if (!ifs.is_open())
    {
        std::cerr << "Failed to Open file: " << filepath << std::endl;
        return "";
    }

    std::string line;
    std::string contents;

    if (const auto size = std::filesystem::file_size(filepath, ec); !ec)
        contents.reserve(size);

    while (getline(ifs, line))
    {
        contents.append(line + '\n');
    }

    return contents;
}

/*
 * Attach vertex and fragment to m_programID.
 */
void Shader::CreateShader()
{
    m_programID = glCreateProgram();
    if (m_programID == 0)
        std::cerr << "Error occured creating program" << std::endl;

    unsigned int vs{CompileShader(GL_VERTEX_SHADER, m_vertexshader)};
    unsigned int fs{CompileShader(GL_FRAGMENT_SHADER, m_fragmentshader)};

    glAttachShader(m_programID, vs);
    glAttachShader(m_programID, fs);
    glLinkProgram(m_programID);
    glValidateProgram(m_programID);

    int linkresult, validateresult;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linkresult);
    glGetProgramiv(m_programID, GL_VALIDATE_STATUS, &validateresult);// debugging, heavy cost.
    if (linkresult == GL_FALSE || validateresult == GL_FALSE)
    {
        int loglength;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &loglength);

        char *message = (char*) alloca(loglength * sizeof(char));
        glGetProgramInfoLog(m_programID, loglength, &loglength, message);

        std::cerr << "Program failed to link/validate:\n\n"
                  << message << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

/*
 * Compiles shader out of raw file contents.
 *
 * @param:
 * unsigned int {type}: type of shader; vertex or fragment.
 * const std::string {source}: file contents.
 */
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id{glCreateShader(type)};
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // check if shader compiled successfully, else print log error.
    int result{};
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char *message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        /* std::string message; */
        /* message.reserve(length); */
        /* glGetShaderInfoLog(id, length, &length, message.data()); */

        std::cerr << "Failed to Compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader\n" << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
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
