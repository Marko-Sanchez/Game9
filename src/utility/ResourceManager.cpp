#include "utility/ResourceManager.h"

#include <exception>
#include <stb/stb_image.h>

#include <print>
#include <cstdio>

#include <fstream>
#include <filesystem>
#include <stdexcept>
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
std::shared_ptr<Shader> ResourceManager::LoadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::string_view shaderName)
{
    if (std::error_code ec; !std::filesystem::exists(vertexPath, ec) || !std::filesystem::exists(fragmentPath, ec))
    {
        std::println(stderr, "Shader path not found: {}", ec.message());
        return nullptr;
    }
    else if (auto shaderIter = m_shader.find(shaderName); shaderIter != m_shader.end())
    {
        std::println(stderr, "Shader already exist with this name: {}", shaderName);
        return nullptr;
    }

    try
    {
        auto &v = m_files.try_emplace(
                vertexPath.string(),
                CompileShader(GL_VERTEX_SHADER, ParseShaderFile(vertexPath))
                ).first->second;

        auto &f = m_files.try_emplace(
                fragmentPath.string(),
                CompileShader(GL_FRAGMENT_SHADER, ParseShaderFile(fragmentPath))
                ).first->second;

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

/*
 * Creates a texture allowing shader to read texture data.
 *
 * @params:
 * texturePath: absolute path to 2D texture (in this case .png file).
 * textureName: unique name to allow identifying texture.
 * textureSlot: int value representing which slot texture unit it to be made active for drawing / manipulating.
 */
std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::filesystem::path& texturePath, const std::string_view textureName, const int textureSlot)
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

    try
    {
        auto texture = std::make_shared<Texture2D>(texturePath, textureSlot);
        m_textures.emplace(std::string{textureName}, texture);

        return texture;
    }
    catch (const std::exception& e)
    {
        std::println(stderr, "Failed to create texture '{}': {}", textureName, e.what());
        return nullptr;
    }
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
std::string ResourceManager::ParseShaderFile(const std::filesystem::path& filepath)
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

        glDeleteShader(id);

        throw std::runtime_error(std::format("Failed to compile {} shader: {}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message));
    }

    return id;
}
}// namespace Core::util
