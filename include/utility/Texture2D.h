#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string_view>

class Texture2D
{
public:
    unsigned int m_ID;

    int m_width;
    int m_height;

    int m_internalFormat;// desired image format output.
    int m_imageFormat;   // format given by image.

    unsigned int m_wrapS;
    unsigned int m_wrapT;

    unsigned int m_filterMin;
    unsigned int m_filterMax;

    Texture2D();
    ~Texture2D();

    unsigned int GetID() const noexcept;

    void Bind() const;
    void UnBind() const;

    void GenerateTexture(const std::string_view& texturePath, int textureSlot = 0);
};

#endif
