//
//  Font.hpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 23.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#ifndef Font_hpp
#define Font_hpp

#include <iostream>
#include <string>
#include <map>
#include <freetype2/ft2build.h>
#include <freetype2/freetype/freetype.h>
#include "glad.h"
#include "glfw3.h"
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "type_ptr.hpp"
#include "Shader.hpp"

class Font
{
    
private:
    
    const unsigned int SCR_WIDTH = 1440;
    const unsigned int SCR_HEIGHT = 900;
    
    GLuint VertexArrayObject;
    GLuint VertexBufferObject;
    
    float m_fRed = 1.0f;
    float m_fGreen = 1.0f;
    float m_fBlue = 1.0f;
    float m_fAlpha = 1.0f;
    
    std::string m_sFontIDMemory;
    
    glm::mat4 m_projection;
    
    struct Character
    {
        GLuint     TextureID;  // ID handle of the glyph texture
        glm::ivec2 Size;       // Size of glyph
        glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
        GLuint     Advance;    // Offset to advance to next glyph
    };

    std::map<GLchar, Character> Characters;
    
public:
    
    Font();
    void SetFont(std::string sFontID, unsigned short usHeight);
    void SetColor(float fR = 1.0f, float fG = 1.0f, float fB = 1.0f, float fAlpha = 1.0f);
    void DrawText(std::string sText, GLfloat fX, GLfloat fY, GLfloat fScale, Shader& shaderToUse);
    
};

#endif /* Font_hpp */
