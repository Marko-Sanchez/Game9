#ifndef RESOURCEMANGER_H
#define RESOURCEMANGER_H

#include "utility/Shader.h"
#include "utility/Texture2D.h"

#include <GL/glew.h>

#include <memory>
#include <functional>
#include <optional>
#include <string>
#include <map>

class ResourceManager
{
private:
    using shared_shader  = std::shared_ptr<Shader>;
    using shared_texture = std::shared_ptr<Texture2D>;
    using shared_image   = std::shared_ptr<unsigned char>;

    // RAII: Holds compiled shader file ID.
    struct ShaderFileHandler
    {
        unsigned int ID;

        // explicit prevents implicit conversion of uid to a ShaderFileHandler.
        // ex. SomeFunc(1) -> SomeFunc(ShaderFileHandler).
        explicit ShaderFileHandler(unsigned int id):ID(id){}
        ~ShaderFileHandler()
        {
            if (ID) glDeleteShader(ID);
        }

        // Move Contructor:
        ShaderFileHandler(ShaderFileHandler&& other) noexcept:
        ID(other.ID)
        {
            other.ID = 0;
        }
        // Move Assignment: Delete own ID and copy other's ID.
        ShaderFileHandler& operator=(ShaderFileHandler&& other) noexcept
        {
            if (this != &other)
            {
                if (ID) glDeleteShader(ID);
                ID = other.ID;
                other.ID = 0;
            }
            return *this;
        }
        // Copy Contructor:
        ShaderFileHandler(const ShaderFileHandler&) = delete;
        // Copy Assignment:
        ShaderFileHandler& operator=(const ShaderFileHandler&) = delete;
    };

public:
    // std::string_view file path, ShaderFileHandler is from compileshader.
    inline static std::map<std::string_view, ShaderFileHandler, std::less<>> m_files;

    inline static std::map<std::string, shared_shader, std::less<>> m_shader;
    inline static std::map<std::string, shared_texture, std::less<>> m_textures;

    static std::optional<shared_shader> LoadShader(const std::string_view vertexPath, const std::string_view fragmentPath, const std::string_view shaderName);
    static std::optional<shared_shader> GetShader(const std::string_view shaderName);

    static std::optional<shared_texture> LoadTexture(const std::string_view texturePath, const std::string_view textureName, const int textureSlot);
    static std::optional<shared_texture> GetTexture(const std::string_view textureName);

    static std::optional<shared_image> LoadImage(const std::string_view path, int& width, int& height);

    // TODO: function to remove un-used textures and shaders.
private:
    ResourceManager() {}

    static std::string ParseShaderFile(const std::string_view& filepath);
    static unsigned int CompileShader(unsigned int type, const std::string& source);

};
#endif
