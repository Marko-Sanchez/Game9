#include "utility/ResourceManager.h"

#include <iostream>

#include <filesystem>
#include <system_error>

// Instatiate static variables
std::map<std::string, Shader, std::less<>> ResourceManager::m_shader;
std::map<std::string, Texture2D, std::less<>> ResourceManager::m_textures;

std::optional<Shader> ResourceManager::LoadShader(const std::string_view vertexPath, const std::string_view fragmentPath, const std::string_view shaderName)
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
        m_shader[name] = Shader();
        m_shader[name].LoadShader(vertexPath, fragmentPath);

        return m_shader[name];
    }

    std::cerr << "Shader already exist with this name: " << shaderName << std::endl;
    return {};
}

// TODO: figure out pathing.
std::optional<Texture2D> ResourceManager::LoadTexture(const std::string_view texturePath, const std::string_view textureName)
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
        m_textures[name] = Texture2D();
        m_textures[name].GenerateTexture(texturePath);

        return m_textures[name];
    }

    std::cerr << "Texture already exist with this name: " << textureName << std::endl;
    return {};
}

std::optional<Shader> ResourceManager::GetShader(const std::string_view shaderName)
{
    if (auto iter = m_shader.find(shaderName); iter != m_shader.end())
    {
        return iter->second;
    }

    return {};
}

std::optional<Texture2D> ResourceManager::GetTexture(const std::string_view textureName)
{
    if (auto iter = m_textures.find(textureName); iter != m_textures.end())
    {
        return iter->second;
    }

    return {};
}
