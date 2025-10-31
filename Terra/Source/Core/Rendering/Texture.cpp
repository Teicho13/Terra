#include "Texture.h"

#include <stb_image.h>
#include <glad/glad.h>

Terra::Texture::Texture(const std::string& ImagePath)
    : m_ID(0), m_FilePath(ImagePath), m_Width(0), m_Height(0), m_Channels(0)
{
    stbi_set_flip_vertically_on_load(1);
    
    unsigned char* imageBytes = stbi_load(ImagePath.c_str(), &m_Width, &m_Height, &m_Channels, 4);
    
    //Create Texture object and bind ID
    glGenTextures(1,&m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    //Set texture parameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //assign texture to opengl texture object
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,m_Width,m_Height,0,GL_RGBA,GL_UNSIGNED_BYTE, imageBytes);
    
    //unbind object and free memory
    glBindTexture(GL_TEXTURE_2D, 0);
    if (imageBytes)
        stbi_image_free(imageBytes);
}

Terra::Texture::~Texture()
{
    glDeleteTextures(1,&m_ID);
}

void Terra::Texture::Bind(const unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D,m_ID);
}

void Terra::Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D,0);
}
