#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string_view>

/*
* Generates a 2D Texture using third-party vendor STBI to load
* image data.
*/
class Texture2D
{
public:
    unsigned int m_ID;
    int m_textureSlot;

    int m_width;
    int m_height;

    // Describes how the texture will be stored in the GPU.
    // stbi_load will return the number of channels in the image
    // if desired_channels (last value) is 0.
    int m_internalFormat;
    // Defines data being passed to GPU.
    int m_imageFormat;

    unsigned int m_wrapS;
    unsigned int m_wrapT;

    unsigned int m_filterMin;
    unsigned int m_filterMax;

    Texture2D();
    ~Texture2D();

    unsigned int GetID() const noexcept;
    int GetTextureSlot() const noexcept;

    void Bind() const;
    void UnBind() const;

    void GenerateTexture(const std::string_view& texturePath, int textureSlot = 0);
};

#endif
