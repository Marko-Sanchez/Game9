#ifndef RESOURCEMANGER_H
#define RESOURCEMANGER_H

#include "utility/Shader.h"
#include "utility/Texture2D.h"

#include <GL/glew.h>

#include <memory>
#include <functional>
#include <filesystem>
#include <string>
#include <map>

namespace Core::util
{
// Loads and unloads resources. Keeps track of object ID's for shaders and textures.
class ResourceManager
{
private:

    // RAII: Holds compiled shader file ID.
    struct ShaderFileHandler
    {
        unsigned int ID;

        explicit ShaderFileHandler(unsigned int id);
        ~ShaderFileHandler();

        ShaderFileHandler(ShaderFileHandler&& other) noexcept;
        ShaderFileHandler& operator=(ShaderFileHandler&& other) noexcept;

        ShaderFileHandler(const ShaderFileHandler&) = delete;
        ShaderFileHandler& operator=(const ShaderFileHandler&) = delete;
    };

    // std::string_view file path, ShaderFileHandler is from compileshader.
    std::map<std::string, ShaderFileHandler, std::less<>> m_files;

    // shader object should be transformed to open function instead of class functions ??
    std::map<std::string, std::shared_ptr<Shader>, std::less<>> m_shader;
    std::map<std::string, std::shared_ptr<Texture2D>, std::less<>> m_textures;

public:
    std::shared_ptr<Shader> LoadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::string_view shaderName);
    std::shared_ptr<Shader> GetShader(const std::string_view shaderName);

    std::shared_ptr<Texture2D> LoadTexture(const std::filesystem::path& texturePath, const std::string_view textureName, const int textureSlot);
    std::shared_ptr<Texture2D> GetTexture(const std::string_view textureName);

    std::shared_ptr<unsigned char> LoadImage(const std::string_view path, int& width, int& height);

    // TODO: function to remove un-used textures and shaders.
private:

    std::string ParseShaderFile(const std::filesystem::path& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);

};
}// namespace Core::util
#endif
