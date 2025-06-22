#ifndef RESOURCEMANGER_H
#define RESOURCEMANGER_H
#include "utility/Shader.h"
#include "utility/Texture2D.h"

#include <string>
#include <map>

class ResourceManager
{
public:
    static std::map<std::string, Shader> m_shader;
    static std::map<std::string, Texture2D> m_textures;

    static Shader LoadShader(const std::string_view vertexPath, const std::string_view fragmentPath, const std::string_view shaderName);
    static Shader GetShader(const std::string_view shaderName);

    static Texture2D LoadTexture(const std::string_view texturePath, const std::string_view textureName);
    static Texture2D GetTexture(const std::string_view textureName);

    // TODO: function to remove un-used textures and shaders.
private:
    ResourceManager() {}
};
#endif
