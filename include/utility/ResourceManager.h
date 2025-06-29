#ifndef RESOURCEMANGER_H
#define RESOURCEMANGER_H

#include "utility/Shader.h"
#include "utility/Texture2D.h"

#include <functional>
#include <optional>
#include <string>
#include <map>

class ResourceManager
{
public:
    static std::map<std::string, Shader, std::less<>> m_shader;
    static std::map<std::string, Texture2D, std::less<>> m_textures;

    static std::optional<Shader> LoadShader(const std::string_view vertexPath, const std::string_view fragmentPath, const std::string_view shaderName);
    static std::optional<Shader> GetShader(const std::string_view shaderName);

    static std::optional<Texture2D> LoadTexture(const std::string_view texturePath, const std::string_view textureName);
    static std::optional<Texture2D> GetTexture(const std::string_view textureName);

    // TODO: function to remove un-used textures and shaders.
private:
    ResourceManager() {}
};
#endif
