#include "utility/ResourceManager.h"


#include <stb/stb_image.h>

#include <iostream>

#include <fstream>
#include <filesystem>
#include <string_view>
#include <system_error>

/*
 * Parses and compiles vertex and fragment shader files. If shader path already exist,
 * create a new shader and attach the already created shader id to a new program.
*/
std::optional<ResourceManager::shared_shader> ResourceManager::LoadShader(const std::string_view vertexPath, const std::string_view fragmentPath, const std::string_view shaderName)
{
    std::error_code ec;
    if (!std::filesystem::exists(vertexPath, ec) || !std::filesystem::exists(fragmentPath, ec))
    {
        std::cerr << "Shader path not found:\n" << ec.message() << std::endl;
        return std::nullopt;
    }
    else if (auto shaderIter = m_shader.find(shaderName); shaderIter != m_shader.end())
    {
        std::cerr << "Shader already exist with this name: " << shaderName << std::endl;
        return std::nullopt;
    }

    auto vertexIter   = m_files.find(vertexPath);
    auto fragmentIter = m_files.find(fragmentPath);

    if (vertexIter ==  m_files.end())
    {
        std::string contents{ParseShaderFile(vertexPath)};
        vertexIter = m_files.emplace(vertexPath, CompileShader(GL_VERTEX_SHADER, contents)).first;
    }
    if (fragmentIter == m_files.end())
    {
        std::string contents{ParseShaderFile(fragmentPath)};
        fragmentIter = m_files.emplace(fragmentPath, CompileShader(GL_FRAGMENT_SHADER, contents)).first;
    }

    auto vID = vertexIter->second.ID;
    auto fID = fragmentIter->second.ID;

    // Create Shader.
    auto shader = std::make_shared<Shader>(vID, fID);
    shader->CreateShader();

    std::string name{shaderName};
    m_shader.emplace(name, shader);

    return m_shader[name];
}

// TODO: Figure file paths, in the case were user does not call executable from source directory.
std::optional<ResourceManager::shared_texture> ResourceManager::LoadTexture(const std::string_view texturePath, const std::string_view textureName, const int textureSlot)
{
    std::error_code ec;
    if (!std::filesystem::exists(texturePath, ec))
    {
        std::cerr << "Texture path not found:\n" << ec.message() << std::endl;
        return std::nullopt;
    }

    if (auto iter = m_textures.find(textureName); iter != m_textures.end())
    {
        std::cerr << "Texture already exist with this name: " << textureName << std::endl;
        return std::nullopt;
    }

    std::string name{textureName};
    auto texture = std::make_shared<Texture2D>();
    texture->GenerateTexture(texturePath, textureSlot);
    m_textures[name] = texture;

    return m_textures[name];
}

std::optional<ResourceManager::shared_shader> ResourceManager::GetShader(const std::string_view shaderName)
{
    if (auto iter = m_shader.find(shaderName); iter != m_shader.end())
    {
        return iter->second;
    }

    return std::nullopt;
}

std::optional<ResourceManager::shared_texture> ResourceManager::GetTexture(const std::string_view textureName)
{
    if (auto iter = m_textures.find(textureName); iter != m_textures.end())
    {
        return iter->second;
    }

    return std::nullopt;
}

/*
* Loads an rgba image and updates passed the references width and height, Returns an optional
* containing shared_ptr<unsigned char> with custom deleter 'stbi_image_free.'
*
* @params:
* path: path to image.
* width: reference to width, will be updated by stbi_load.
* height: reference to height, will be updated by stbi_load.
*/
std::optional<ResourceManager::shared_image> ResourceManager::LoadImage(const std::string_view path, int& width, int& height)
{
    std::error_code ec;
    if (!std::filesystem::exists(path, ec))
    {
        std::cerr << "Path not found:\n" << ec.message() << std::endl;
        return std::nullopt;
    }

    return std::shared_ptr<unsigned char>(stbi_load(path.data(), &width, &height, 0, 4), stbi_image_free);
}

/*
* Reads contents from file and returns as a string.
*/
std::string ResourceManager::ParseShaderFile(const std::string_view& filepath)
{
    std::ifstream ifs(filepath.data());
    if (!ifs.is_open())
    {
        std::cerr << "Failed to Open file: " << filepath << std::endl;
        return "";
    }

    std::string line;
    std::string contents;

    std::error_code ec;
    if (const auto size{std::filesystem::file_size(filepath, ec)}; !ec)
        contents.reserve(size);

    while (getline(ifs, line))
    {
        contents.append(line + '\n');
    }

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
unsigned int ResourceManager::CompileShader(unsigned int type, const std::string& source)
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

        std::string message;
        message.reserve(length);
        glGetShaderInfoLog(id, length, &length, message.data());

        std::cerr << "Failed to Compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader\n" << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}
