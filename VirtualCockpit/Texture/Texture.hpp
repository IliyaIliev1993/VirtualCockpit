//
//  Texture.hpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 14.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <string>
#include <iostream>
#include "glad.h"
#include "glfw3.h"

class Texture
{
    
private:
    
    unsigned int m_Texture;
    int m_Width;
    int m_Height;
    int m_NumberOfChannels;
    
public:
    
    Texture();
    
    unsigned int GetTexture();
    int& GetWidth();
    int& GetHeight();
    int& GetNumberOfChannels();
    void LoadFromFile(std::string sPath);
    void Bind();
    void Unbind();
    
};

#endif /* Texture_hpp */
