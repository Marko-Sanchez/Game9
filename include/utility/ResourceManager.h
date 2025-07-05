#ifndef RESOURCEMANGER_H
#define RESOURCEMANGER_H

#include "utility/Shader.h"
#include "utility/Texture2D.h"

#include <memory>
#include <functional>
#include <optional>
#include <string>
#include <map>

class ResourceManager
{
private:
    using shared_shader = std::shared_ptr<Shader>;
    using shared_texture = std::shared_ptr<Texture2D>;

public:
    static std::map<std::string, shared_shader, std::less<>> m_shader;
    static std::map<std::string, shared_texture, std::less<>> m_textures;

    static std::optional<shared_shader> LoadShader(const std::string_view vertexPath, const std::string_view fragmentPath, const std::string_view shaderName);
    static std::optional<shared_shader> GetShader(const std::string_view shaderName);

    static std::optional<shared_texture> LoadTexture(const std::string_view texturePath, const std::string_view textureName);
    static std::optional<shared_texture> GetTexture(const std::string_view textureName);

    // TODO: function to remove un-used textures and shaders.
private:
    ResourceManager() {}
};
#endif
