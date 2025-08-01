#include "utility/Texture2D.h"

#include <GL/gl.h>
#include <stb/stb_image.h>

#include <iostream>
#include <memory>

/* Constructor */
Texture2D::Texture2D():
m_ID(0), m_imageFormat(GL_RGBA), m_wrapS(GL_CLAMP_TO_EDGE), m_wrapT(GL_CLAMP_TO_EDGE), m_filterMin(GL_LINEAR), m_filterMax(GL_LINEAR)
{}

/* Destructor */
Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_ID);
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

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

/*
* Generates a 2D texture from the given path.
*
* @params:
* texturePath: path to 2D texture.
* textureSlot: active texture slot that will affect subsequent texture state calls.
*/
void Texture2D::GenerateTexture(const std::string_view& texturePath, int textureSlot)
{
    m_textureSlot = textureSlot;

    stbi_set_flip_vertically_on_load(1);

    // stbi_load will return the number of channels in the image if desired_channels (last value) is 0.
    std::unique_ptr<unsigned char[], decltype(stbi_image_free)*>
    buffer(stbi_load(std::string(texturePath).c_str(), &m_width, &m_height, &m_internalFormat, 0), stbi_image_free);

    if (buffer.get())
    {
        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_imageFormat, GL_UNSIGNED_BYTE, buffer.get());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax);

        UnBind();
    }
    else
    {
        std::cerr << "Failed to load Texture\n" << stbi_failure_reason() << std::endl;
    }
}
