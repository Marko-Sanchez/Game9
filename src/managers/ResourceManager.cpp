#include "managers/ResourceManager.h"

#include <exception>
#include <cstdio>
#include <print>

#include <filesystem>
#include <string_view>

#include <stb_image.h>

namespace Manager
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
std::shared_ptr<Renderer::Shader> ResourceManager::LoadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::string_view shaderName)
{
    if (!std::filesystem::exists(vertexPath))
    {
        std::println(stderr, "Shader path '{}' not found.", vertexPath.string());
        return nullptr;
    }
    else if (!std::filesystem::exists(fragmentPath))
    {
        std::println(stderr, "Shader path '{}' not found.", fragmentPath.string());
        return nullptr;
    }
    else if (auto shaderIter = m_shader.find(shaderName); shaderIter != m_shader.end())
    {
        std::println(stderr, "Shader already exist with this name: {}", shaderName);
        return nullptr;
    }

    try
    {
        // Create Shader.
        auto shader = std::make_shared<Renderer::Shader>(vertexPath, fragmentPath);
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
std::shared_ptr<Renderer::Texture2D> ResourceManager::LoadTexture(const std::filesystem::path& texturePath, const std::string_view textureName, const int textureSlot)
{
    if (!std::filesystem::exists(texturePath))
    {
        std::println(stderr, "Texture path '{}' not found.", texturePath.string());
        return nullptr;
    }

    if (auto iter = m_textures.find(textureName); iter != m_textures.end())
    {
        std::println(stderr, "Texture already exist with this name: {}", textureName);
        return nullptr;
    }

    try
    {
        auto texture = std::make_shared<Renderer::Texture2D>(texturePath, textureSlot);
        m_textures.emplace(std::string{textureName}, texture);

        return texture;
    }
    catch (const std::exception& e)
    {
        std::println(stderr, "Failed to create texture '{}': {}", textureName, e.what());
        return nullptr;
    }
}

std::shared_ptr<Renderer::Shader> ResourceManager::GetShader(const std::string_view shaderName)
{
    if (auto iter = m_shader.find(shaderName); iter != m_shader.end())
    {
        return iter->second;
    }

    return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::GetTexture(const std::string_view textureName)
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
    if (!std::filesystem::exists(path))
    {
        std::println(stderr, "Image Path '{}' not found.", path);
        return nullptr;
    }

    return std::shared_ptr<unsigned char>(stbi_load(path.data(), &width, &height, 0, 4), stbi_image_free);
}

}// namespace Manager
