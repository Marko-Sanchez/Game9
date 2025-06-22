#include "utility/ResourceManager.h"
#include "utility/Texture2D.h"

#include <iostream>

#include <filesystem>
#include <system_error>

Shader ResourceManager::LoadShader(const std::string_view vertexPath, const std::string_view fragmentPath, const std::string_view shaderName)
{
    std::error_code ec;
    if (!std::filesystem::exists(vertexPath, ec) || !std::filesystem::exists(fragmentPath, ec))
    {
        std::cerr << "Shader path not found:\n" << ec.message() << std::endl;
        return Shader();
    }
    if (m_shader.find(shaderName.data()) == m_shader.end())
    {
        std::cerr << "Shader already exist with this name: " << shaderName << std::endl;
        return Shader();
    }

    m_shader[shaderName.data()] = Shader();
    m_shader[shaderName.data()].LoadShader(vertexPath, fragmentPath);

    return m_shader[shaderName.data()];
}

// TODO: figure out pathing.
Texture2D ResourceManager::LoadTexture(const std::string_view texturePath, const std::string_view textureName)
{
    std::error_code ec;
    if (!std::filesystem::exists(texturePath, ec))
    {
        std::cerr << "Texture file does not exist:\n" << ec.message() << std::endl;
        return Texture2D();
    }
    if (m_textures.find(textureName.data()) == m_textures.end())
    {
        std::cerr << "Texture already exist with this name: " << textureName << std::endl;
        return Texture2D();
    }

    m_textures[textureName.data()] = Texture2D();
    m_textures[textureName.data()].GenerateTexture(texturePath);

    return m_textures[textureName.data()];
}

Shader ResourceManager::GetShader(const std::string_view shaderName)
{
    return m_shader[shaderName.data()];
}

Texture2D ResourceManager::GetTexture(const std::string_view textureName)
{
    return m_textures[textureName.data()];
}
