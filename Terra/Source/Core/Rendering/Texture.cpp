#include "Texture.h"

#include <iostream>
#include <stb_image.h>
#include <glad/glad.h>

Terra::Texture::Texture(const std::filesystem::path& ImagePath)
{
    CreateTexture(ImagePath);
}

Terra::Texture::~Texture()
{
    glDeleteTextures(1,&m_ID);
}

void Terra::Texture::CreateTexture(const std::filesystem::path& ImagePath)
{
    int ImgWidth, ImgHeight, numColCh;
    unsigned char* bytes = stbi_load(ImagePath.string().c_str(), &ImgWidth, &ImgHeight, &numColCh, 0);

    if (bytes == nullptr)
    {
        std::cout << "Failed to load image \n";
        return;
    }
    
    //Create Texture object and bind ID
    glGenTextures(1,&m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    //Set texture parameter
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //assign texture to opengl texture object
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ImgWidth,ImgHeight,0,GL_RGB,GL_UNSIGNED_BYTE,bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //unbind object and free memory
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(bytes);
}

void Terra::Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D,m_ID);
}

void Terra::Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D,0);
}
