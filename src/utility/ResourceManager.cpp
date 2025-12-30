#include "utility/ResourceManager.h"

#include <exception>
#include <stb/stb_image.h>

#include <print>
#include <cstdio>

#include <fstream>
#include <filesystem>
#include <string_view>
#include <system_error>

namespace Core::util
{

/**************** ShaderFileHandler ************************/

// explicit prevents implicit conversion of uid to a ShaderFileHandler.
// ex. SomeFunc(1) -> SomeFunc(ShaderFileHandler).
ResourceManager::ShaderFileHandler::ShaderFileHandler(unsigned int id):
ID(id)
{}

ResourceManager::ShaderFileHandler::~ShaderFileHandler()
{
    if (ID) glDeleteShader(ID);
}

// Move constructor.
ResourceManager::ShaderFileHandler::ShaderFileHandler(ShaderFileHandler&& other) noexcept:
ID(other.ID)
{
    other.ID = 0;
}

// Move Assignment: Delete own ID and copy other's ID.
ResourceManager::ShaderFileHandler& ResourceManager::ShaderFileHandler::operator=(ShaderFileHandler&& other) noexcept
{
    if (this != &other)
    {
        if (ID) glDeleteShader(ID);
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

/**************** ShaderFileHandler ************************/

/*
 * Parses and compiles vertex and fragment shader files. If shader path already exist,
 * create a new shader and attach the already created shader id to a new program.
*/
std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string_view vertexPath, const std::string_view fragmentPath, const std::string_view shaderName)
{
    std::error_code ec;
    if (!std::filesystem::exists(vertexPath, ec) || !std::filesystem::exists(fragmentPath, ec))
    {
        std::println(stderr, "Shader path not found: {}", ec.message());
        return nullptr;
    }
    else if (auto shaderIter = m_shader.find(shaderName); shaderIter != m_shader.end())
    {
        std::println(stderr, "Shader already exist with this name: {}", shaderName);
        return nullptr;
    }

    auto &v = m_files.try_emplace(
            std::string{vertexPath},
            CompileShader(GL_VERTEX_SHADER, ParseShaderFile(vertexPath))
            ).first->second;

    auto &f = m_files.try_emplace(
            std::string{fragmentPath},
            CompileShader(GL_FRAGMENT_SHADER, ParseShaderFile(fragmentPath))
            ).first->second;

    try
    {
        // Create Shader.
        auto shader = std::make_shared<Shader>(v.ID, f.ID);
        m_shader.emplace(std::string{shaderName}, shader);

        return shader;
    }
    catch (const std::exception& e)
    {
        std::println(stderr, "Failed to create shader '{}': {}", shaderName, e.what());
        return nullptr;
    }
}

// should resource manager simply handle the loading of textures and loading of shaders->enum.

// TODO: Figure file paths, in the case were user does not call executable from source directory.
// whatever object handles the texture should deal with the texture-name
std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string_view texturePath, const std::string_view textureName, const int textureSlot)
{
    std::error_code ec;
    if (!std::filesystem::exists(texturePath, ec))
    {
        std::println(stderr, "Texture path not found: {}", ec.message());
        return nullptr;
    }

    if (auto iter = m_textures.find(textureName); iter != m_textures.end())
    {
        std::println(stderr, "Texture already exist with this name: {}", textureName);
        return nullptr;
    }

    auto texture = std::make_shared<Texture2D>();
    texture->GenerateTexture(texturePath, textureSlot);
    m_textures.emplace(std::string{textureName}, texture);

    return texture;
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string_view shaderName)
{
    if (auto iter = m_shader.find(shaderName); iter != m_shader.end())
    {
        return iter->second;
    }

    return nullptr;
}

std::shared_ptr<Texture2D> ResourceManager::GetTexture(const std::string_view textureName)
{
    if (auto iter = m_textures.find(textureName); iter != m_textures.end())
    {
        return iter->second;
    }

    return nullptr;
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
std::shared_ptr<unsigned char> ResourceManager::LoadImage(const std::string_view path, int& width, int& height)
{
    std::error_code ec;
    if (!std::filesystem::exists(path, ec))
    {
        std::println(stderr, "Path not found: {}", ec.message());
        return nullptr;
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
        std::println(stderr,  "Failed to Open file: {}", filepath);
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

        std::println(stderr, "Failed to compile {} shader: {}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);

        glDeleteShader(id);
        return 0;
    }

    return id;
}
}// namespace Core::util
