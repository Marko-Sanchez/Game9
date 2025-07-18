#include "utility/ResourceManager.h"

#include <stb/stb_image.h>

#include <iostream>

#include <filesystem>
#include <string_view>
#include <system_error>

// Instatiate static variables
std::map<std::string, ResourceManager::shared_shader, std::less<>> ResourceManager::m_shader;
std::map<std::string, ResourceManager::shared_texture, std::less<>> ResourceManager::m_textures;

std::optional<ResourceManager::shared_shader> ResourceManager::LoadShader(const std::string_view vertexPath, const std::string_view fragmentPath, const std::string_view shaderName)
{
    std::error_code ec;
    if (!std::filesystem::exists(vertexPath, ec) || !std::filesystem::exists(fragmentPath, ec))
    {
        std::cerr << "Shader path not found:\n" << ec.message() << std::endl;
        return {};
    }

    if (auto iter = m_shader.find(shaderName); iter == m_shader.end())
    {
        std::string name{shaderName};
        auto shader = std::make_shared<Shader>();
        shader->LoadShader(vertexPath, fragmentPath);
        m_shader[name] = shader;

        return m_shader[name];
    }

    std::cerr << "Shader already exist with this name: " << shaderName << std::endl;
    return {};
}

// TODO: figure out pathing.
std::optional<ResourceManager::shared_texture> ResourceManager::LoadTexture(const std::string_view texturePath, const std::string_view textureName)
{
    std::error_code ec;
    if (!std::filesystem::exists(texturePath, ec))
    {
        std::cerr << "Texture path not found:\n" << ec.message() << std::endl;
        return {};
    }

    if (auto iter = m_textures.find(textureName); iter == m_textures.end())
    {
        std::string name{textureName};
        auto texture = std::make_shared<Texture2D>();
        texture->GenerateTexture(texturePath);
        m_textures[name] = texture;

        return m_textures[name];
    }

    std::cerr << "Texture already exist with this name: " << textureName << std::endl;
    return {};
}

std::optional<ResourceManager::shared_shader> ResourceManager::GetShader(const std::string_view shaderName)
{
    if (auto iter = m_shader.find(shaderName); iter != m_shader.end())
    {
        return iter->second;
    }

    return {};
}

std::optional<ResourceManager::shared_texture> ResourceManager::GetTexture(const std::string_view textureName)
{
    if (auto iter = m_textures.find(textureName); iter != m_textures.end())
    {
        return iter->second;
    }

    return {};
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
