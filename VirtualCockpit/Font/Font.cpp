//
//  Font.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 23.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include "Font.hpp"

Font::Font()
{
    m_sFontIDMemory = "";
}

void Font::SetFont(std::string sFontID, unsigned short usHeight)
{
    if(m_sFontIDMemory == sFontID)
    {
        return;
    }
    
    m_projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);
    
    FT_Library ft_library;
    FT_Face ft_font;
    
    if(FT_Init_FreeType(&ft_library))
    {
        std::cout << "Failed to initialize FT_LIBRARY" << std::endl;
    }
    
    if(FT_New_Face(ft_library, sFontID.c_str(), 0, &ft_font))
    {
        std::cout << "Failed to set FONT: " << sFontID << std::endl;
    }
    
    FT_Set_Pixel_Sizes(ft_font, 0, usHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    if(FT_Load_Char(ft_font, 'X', FT_LOAD_RENDER))
    {
        std::cout << "Failed to load Glyph" << std::endl;
    }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
      
    for (GLubyte chars = 0; chars < 128; chars++)
    {
        if (FT_Load_Char(ft_font, chars, FT_LOAD_RENDER))
        {
            std::cout << "Failed to load Glyph" << std::endl;
            continue;
        }
        
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RED,
            ft_font->glyph->bitmap.width,
            ft_font->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            ft_font->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character =
        {
            texture,
            glm::ivec2(ft_font->glyph->bitmap.width, ft_font->glyph->bitmap.rows),
            glm::ivec2(ft_font->glyph->bitmap_left, ft_font->glyph->bitmap_top),
            static_cast<GLuint>(ft_font->glyph->advance.x)
        };
        
        Characters.insert(std::pair<GLchar, Character>(chars, character));
    }
    
    m_sFontIDMemory = sFontID;
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    FT_Done_Face(ft_font);
    FT_Done_FreeType(ft_library);
    
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VertexArrayObject);
    glGenBuffers(1, &VertexBufferObject);
    glBindVertexArray(VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    std::cout << "Loading Font: " << sFontID << " ..." << std::endl;
    
}

void Font::SetColor(float fR, float fG, float fB, float fAlpha)
{
    m_fRed = fR;
    m_fGreen = fG;
    m_fBlue = fB;
    m_fAlpha = fAlpha;
}

void Font::DrawText(std::string sText, GLfloat fX, GLfloat fY, GLfloat fScale, Shader& shaderToUse)
{
    shaderToUse.use();
    shaderToUse.setMat4("projection", m_projection);
    shaderToUse.setVec4("textColor", glm::vec4(m_fRed, m_fGreen, m_fBlue, m_fAlpha));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VertexArrayObject);
    
    std::string::const_iterator it;
    
    for(it = sText.begin(); it != sText.end(); it++)
    {
        Character ch = Characters[*it];

        GLfloat xpos = fX + ch.Bearing.x * fScale;
        GLfloat ypos = fY - (ch.Size.y - ch.Bearing.y) * fScale;

        GLfloat w = ch.Size.x * fScale;
        GLfloat h = ch.Size.y * fScale;
        
        GLfloat vertices[6][4] =
        {
            { xpos,     ypos - h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos - h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos - h,   1.0, 0.0 }
        };
        
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        fX += (ch.Advance >> 6) * fScale;
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
