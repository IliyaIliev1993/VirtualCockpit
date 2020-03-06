//
//  Renderer.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 15.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include "Renderer.hpp"

Renderer::Renderer()
{
    m_Window = nullptr;
}

bool Renderer::InitSystem()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif
    
    m_Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "VirtualCockpit", NULL, NULL);
    glfwMakeContextCurrent(m_Window);
    
    if (m_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    CreateBuffers();
    
    return true;
}

void Renderer::CreateBuffers()
{
    float vertices[] = {
        // positions          // colors           // texture coords
       1440.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
       1440.0f, 900.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
         0.0f,  900.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
         0.0f,  0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    glGenVertexArrays(1, &VertexArrayObject);
    glGenBuffers(1, &VertexBufferObject);
    glGenBuffers(1, &ElementBufferObject);

    glBindVertexArray(VertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);
    
}

void Renderer::DrawPicture(Texture& textureToDraw, float fX, float fY, float fZ, Shader& shaderToUse)
{
    textureToDraw.Bind();
    shaderToUse.use();
    
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = glm::mat4(1.0f);
    
    model = glm::translate(glm::mat4(1.0f), glm::vec3(fX, fY, fZ));
    mvp = projection * view * model;
    shaderToUse.setMat4("mvp", mvp);
    shaderToUse.setVec4("setColor", m_fRed, m_fGreen, m_fBlue, m_fAlpha);
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
}

void Renderer::DrawSquare(float fX, float fY, float fZ, float fWidth, float fHeigh, Shader &shaderToUse)
{
    shaderToUse.use();
    
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = glm::mat4(1.0f);
    
    float fScaleX = 1.0f / ((float)SCR_WIDTH / fWidth);
    float fScaleY = 1.0f / ((float)SCR_HEIGHT / fHeigh);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(fX, fY, fZ));
    model = glm::scale(model, glm::vec3(fScaleX, fScaleY, 0.0f));
    mvp = projection * view * model;
    shaderToUse.setMat4("mvp", mvp);
    shaderToUse.setVec4("setColor", m_fRed, m_fGreen, m_fBlue, m_fAlpha);
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPictureInAspectRatio(Texture &textureToDraw, float fX, float fY, float fZ, Shader &shaderToUse)
{
    textureToDraw.Bind();
    shaderToUse.use();
    
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = glm::mat4(1.0f);
    
    float fScaleX = 1.0f / ((float)SCR_WIDTH / (float)textureToDraw.GetWidth());
    float fScaleY = 1.0f / ((float)SCR_HEIGHT / (float)textureToDraw.GetHeight());
    model = glm::translate(glm::mat4(1.0f), glm::vec3(fX, fY, fZ));
    model = glm::scale(model, glm::vec3(fScaleX, fScaleY, 0.0f));
    mvp = projection * view * model;
    shaderToUse.setMat4("mvp", mvp);
    shaderToUse.setVec4("setColor", m_fRed, m_fGreen, m_fBlue, m_fAlpha);
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPictureScaled(Texture &textureToDraw, float fX, float fY, float fZ, float fScale, Shader &shaderToUse)
{
    textureToDraw.Bind();
    shaderToUse.use();
    
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = glm::mat4(1.0f);
    
    float fScaleX = fScale / ((float)SCR_WIDTH / (float)textureToDraw.GetWidth());
    float fScaleY = fScale / ((float)SCR_HEIGHT / (float)textureToDraw.GetHeight());
    model = glm::translate(glm::mat4(1.0f), glm::vec3(fX, fY, fZ));
    model = glm::scale(model, glm::vec3(fScaleX, fScaleY, 0.0f));
    mvp = projection * view * model;
    shaderToUse.setMat4("mvp", mvp);
    shaderToUse.setVec4("setColor", m_fRed, m_fGreen, m_fBlue, m_fAlpha);
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPictureRotated(Texture &textureToDraw, float fX, float fY, float fZ, float fAngle, float fRadius, float fPivotX, float fPivotY, float fPivotZ, Shader &shaderToUse )
{
    textureToDraw.Bind();
    shaderToUse.use();
    
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = glm::mat4(1.0f);
    
    //        // IMPORATANT ! //
    //        /* The matrix are ridden in back forward !
    //         First need to translate the object to the 0, 0, of the window
    //         Second need to scale (if needed)
    //         Third rotate the object
    //         Fourth translate the object to the given center */
    
    float fScaleX = 1.0f / ((float)SCR_WIDTH / (float)textureToDraw.GetWidth());
    float fScaleY = 1.0f / ((float)SCR_HEIGHT / (float)textureToDraw.GetHeight());
    model = glm::translate(glm::mat4(1.0f), glm::vec3(fX + fPivotX, fY + fPivotY, fZ + fPivotZ));
    model = glm::rotate(model, glm::radians(fAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(fScaleX, fScaleY, 0.0f));
    model = glm::translate(model, glm::vec3(-(float)SCR_WIDTH / 2, -((float)SCR_HEIGHT / 2) - fRadius, 0.0f));
    mvp = projection * view * model;
    shaderToUse.setMat4("mvp", mvp);
    shaderToUse.setVec4("setColor", m_fRed, m_fGreen, m_fBlue, m_fAlpha);
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::SetColor(float fR, float fG, float fB, float fAlpha)
{
    m_fRed = fR;
    m_fGreen = fG;
    m_fBlue = fB;
    m_fAlpha = fAlpha;
}

void Renderer::EnableBlend()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::DisableBlend()
{
    glDisable(GL_BLEND);
}

void Renderer::ClearColor()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

GLFWwindow* Renderer::GetWindow()
{
    return m_Window;
}

const unsigned int Renderer::GetWindowWidth()
{
    return SCR_WIDTH;
}

const unsigned int Renderer::GetWindowHeight()
{
    return SCR_HEIGHT;
}

Renderer gRenderer;
