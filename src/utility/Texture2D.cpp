#include "utility/Texture2D.h"

#include <print>
#include <stb/stb_image.h>

#include <memory>
#include <cassert>
#include <stdexcept>

namespace Core::util
{
/*
* Generates a 2D texture from the given path and sets active texture slot.
*
* @params:
* texturePath: path to 2D texture.
* textureSlot: active texture slot that will affect subsequent texture state calls.
*/
Texture2D::Texture2D(const std::filesystem::path& texturePath, int textureSlot, TextureParams params):
m_ID(0),
m_textureSlot(textureSlot),
m_texParams(params)
{
    assert(m_textureSlot < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);

    stbi_set_flip_vertically_on_load(1);

    // stbi_load will return the number of channels in the image if desired_channels (last value) is 0.
    int channels{0}, imageWidth{0}, imageHeight{0};
    std::unique_ptr<unsigned char[], decltype(stbi_image_free)*>
    buffer(stbi_load(texturePath.c_str(), &imageWidth, &imageHeight, &channels, 0), stbi_image_free);

    GLint internalFormat{0}; // gpu storage.
    GLenum dataFormat{0};   // incoming data.

    switch (channels)
    {
        case STBI_grey:
            dataFormat = internalFormat = GL_RED;
            break;
        case STBI_grey_alpha:
            dataFormat = internalFormat = GL_RG;
            break;
        case STBI_rgb:
            dataFormat = GL_RGB;
            internalFormat = GL_RGB8;
            break;
        case STBI_rgb_alpha:
            dataFormat = GL_RGBA;
            internalFormat = GL_RGBA8;
        break;
    }

    if (buffer)
    {
        glGenTextures(1, &m_ID);
        this->Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageWidth, imageHeight, 0, dataFormat, GL_UNSIGNED_BYTE, buffer.get());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_texParams.wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_texParams.wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_texParams.minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_texParams.magFilter);

        this->UnBind();
    }
    else
    {
        throw std::runtime_error(std::string{"Failed to load Texture: "} + std::string{stbi_failure_reason()});
    }
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
    m_ID = other.m_ID;
    other.m_ID = 0;
}

Texture2D::~Texture2D()
{
    if (m_ID)
    {
        glDeleteTextures(1, &m_ID);
    }
}

/*
* Returns generated texture ID. If function returns 0, texture has not been created or error occured.
*/
unsigned int Texture2D::GetID() const noexcept
{
    return m_ID;
}

/* Returns texture slot. */
int Texture2D::GetTextureSlot() const noexcept
{
    return m_textureSlot;
}

/* Sets active texture and binds. */
void Texture2D::Bind() const
{
    glActiveTexture(GL_TEXTURE0 + m_textureSlot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
}// namespace Core::util
