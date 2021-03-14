//
//  Renderer.hpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 15.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <iostream>
#include <string>
#include "glad.h"
#include "glfw3.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../Texture/Texture.hpp"
#include "shader.hpp"

class Renderer
{
private:
    
    GLFWwindow* m_Window = nullptr;
    const unsigned int SCR_WIDTH = 1440;
    const unsigned int SCR_HEIGHT = 900;
    unsigned int VertexBufferObject;
    unsigned int VertexArrayObject;
    unsigned int ElementBufferObject;
    glm::mat4 projection;
    
    float m_fRed = 1.0f;
    float m_fGreen = 1.0f;
    float m_fBlue = 1.0f;
    float m_fAlpha = 1.0f;
    
public:
    
    Renderer();
    bool InitSystem();
    
    GLFWwindow* GetWindow();
    const unsigned int GetWindowWidth();
    const unsigned int GetWindowHeight();
    void EnableBlend();
    void DisableBlend();
    void ClearColor();
    void CreateBuffers();
    void SetColor(float fR = 1.0f, float fG = 1.0f, float fB = 1.0f, float fAlpha = 1.0f);
    
    void DrawPicture(Texture& textureToDraw, float fX, float fY, float fZ, Shader& shaderToUse);
    void DrawPictureInAspectRatio(Texture& textureToDraw, float fX, float fY, float fZ, Shader& shaderToUse);
    void DrawPictureRotated(Texture& textureToDraw, float fX, float fY, float fZ, float fAngle,
                            float fRadius, float fPivotX, float fPivotY, float fPivotZ, Shader& shaderToUse);
    void DrawPictureScaled(Texture& textureToDraw, float fX, float fY, float fZ, float fScale, Shader& shaderToUse);
    void DrawSquare(float fX, float fY, float fZ, float fWidth, float fHeigh, Shader& shaderToUse);
    
};

extern Renderer gRenderer;

#endif /* Renderer_hpp */
