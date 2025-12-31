#include "utility/Texture2D.h"

#include <GL/gl.h>
#include <stb/stb_image.h>

#include <memory>
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
Texture2D::Texture2D(const std::filesystem::path& texturePath, int textureSlot):
m_ID(0), m_textureSlot(textureSlot), m_imageFormat(GL_RGBA), m_wrapS(GL_CLAMP_TO_EDGE), m_wrapT(GL_CLAMP_TO_EDGE), m_filterMin(GL_LINEAR), m_filterMax(GL_LINEAR)
{
    stbi_set_flip_vertically_on_load(1);

    // stbi_load will return the number of channels in the image if desired_channels (last value) is 0.
    std::unique_ptr<unsigned char[], decltype(stbi_image_free)*>
    buffer(stbi_load(texturePath.c_str(), &m_width, &m_height, &m_internalFormat, 0), stbi_image_free);

    if (buffer.get())
    {
        glGenTextures(1, &m_ID);
        this->Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_imageFormat, GL_UNSIGNED_BYTE, buffer.get());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax);

        this->UnBind();
    }
    else
    {
        std::runtime_error(std::string{"Failed to load Texture: "} + stbi_failure_reason());
    }
}

/* Destructor */
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
