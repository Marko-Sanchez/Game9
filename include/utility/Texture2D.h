#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <GL/gl.h>
#include <filesystem>

namespace Core::util
{

struct TextureParams
{
    GLint wrapS{GL_CLAMP_TO_EDGE};
    GLint wrapT{GL_CLAMP_TO_EDGE};
    GLint minFilter{GL_LINEAR};
    GLint magFilter{GL_LINEAR_MIPMAP_LINEAR};

    bool generateMipmaps{true};
};
/*
* Generates a 2D Texture using third-party vendor STBI to load
* image data.
*/
class Texture2D
{
private:
    GLuint m_ID;
    GLint m_textureSlot;

    TextureParams m_texParams;

public:
    Texture2D(const std::filesystem::path& texturePath, int textureSlot = 0, TextureParams params = {});
    Texture2D(Texture2D&& other) noexcept;
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    ~Texture2D();

    GLuint GetID() const noexcept;
    GLint GetTextureSlot() const noexcept;

    void Bind() const;
    void UnBind() const;
};
}
#endif
