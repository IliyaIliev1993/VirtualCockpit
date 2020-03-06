//
//  Texture.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 14.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{
    m_Texture = 0;
    m_Width = 0;
    m_Height = 0;
    m_NumberOfChannels = 0;
}

void Texture::LoadFromFile(std::string sPath)
{
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(sPath.c_str(), &m_Width, &m_Height, &m_NumberOfChannels, 4);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        std::cout << "Loading Texture: " << sPath.c_str() << " ..." << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture: " << sPath << std::endl;
    }
    
    stbi_image_free(data);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::GetTexture()
{
    return m_Texture;
}

int& Texture::GetWidth()
{
    return m_Width;
}

int& Texture::GetHeight()
{
    return m_Height;
}

int& Texture::GetNumberOfChannels()
{
    return m_NumberOfChannels;
}
