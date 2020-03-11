//
//  main.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 10.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include <iostream>
#include "glad.h"
#include "glfw3.h"
#include "TimerProcess.hpp"
#include "shader.hpp"
#include "Texture.hpp"
#include "GLFW/glm/glm.hpp"
#include "GLFW/glm/gtc/matrix_transform.hpp"
#include "GLFW/glm/gtc/type_ptr.hpp"
#include "Renderer/Renderer.hpp"
#include "Font/Font.hpp"
#include "Sine.h"

#include "MiddleMenu.hpp"
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

float globalX = 0.0f;
float globalY = 0.0f;

MiddleMenu middleMenuObject;

int main()
{
    
    gRenderer.InitSystem();
    glfwSetFramebufferSizeCallback(gRenderer.GetWindow(), framebuffer_size_callback);
    
    Shader shaderTexture("Shaders/vertex_shader.vs", "Shaders/fragment_shader.fs");
    Shader shaderText("Shaders/vertex_shader_font.vs", "Shaders/fragment_shader_font.fs");
    Shader shaderLines("Shaders/vertex_shader_line.vs", "Shaders/fragment_shader_line.fs");
    
    gRenderer.EnableBlend();
    middleMenuObject.LoadResources();
    
    Texture textureObject;
    textureObject.LoadFromFile("Resources/bgr.png");
    
    Texture textureTest;
    textureTest.LoadFromFile("Resources/battery.png");
    
    Font globalFont;
    Font fontOff;
    Font fontRMP;
    
    globalFont.SetFont("Fonts/ExtendedBold.ttf", 40);
    fontOff.SetFont("Fonts/Bold.ttf", 40);
    fontRMP.SetFont("Fonts/Normal.ttf", 40);
    
    
    while (!glfwWindowShouldClose(gRenderer.GetWindow()))
    {
        gTimerProcess.Process();
        middleMenuObject.Process();
        middleMenuObject.GetIndicatorRPMObject().Process();
        middleMenuObject.GetBoardComputerObject().Process();
        
        processInput(gRenderer.GetWindow());
        gRenderer.ClearColor();
        gRenderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

        //Background
        gRenderer.DrawPicture(textureObject, 0.0f, 0.0f, 0.0f, shaderTexture);
        
        //MainDraw
        middleMenuObject.Draw(shaderTexture, shaderText, shaderLines);
        
        //TEXT COORDS
        std::string textCoordsGlobal = "X: " + std::to_string(globalX) + " Y: " + std::to_string(globalY);
        globalFont.DrawText(textCoordsGlobal, 350, 40, 1.0f, shaderText);
        
        std::string textCoords = " RPM: " + std::to_string(middleMenuObject.GetIndicatorRPMObject().GetRPM());
        std::string textCoordsKMH = "KMH: " + std::to_string(middleMenuObject.GetIndicatorRPMObject().GetKMHObject().GetKMH());

        globalFont.DrawText(textCoords, 0, 100, 1.0f, shaderText);
        globalFont.DrawText(textCoordsKMH, 950, 100, 1.0f, shaderText);
        
        gRenderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

        glfwSwapBuffers(gRenderer.GetWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    middleMenuObject.HandleEvent(window);
    middleMenuObject.GetIndicatorRPMObject().HandleEvent(window);
    middleMenuObject.GetBoardComputerObject().HandleEvent(window);
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        globalY -= 0.5;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        globalY += 0.5;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        globalX += 0.5;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        globalX -= 0.5;
    }
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
