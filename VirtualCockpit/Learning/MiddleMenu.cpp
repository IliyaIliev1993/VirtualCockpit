//
//  MiddleMenu.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 1.03.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include "MiddleMenu.hpp"
const unsigned short g_usTimerKeyIn = 0;
const unsigned short g_usTimerDisappearLogo = 1;
const unsigned short g_usTimerKeyOut = 2;

MiddleMenu::MiddleMenu()
{
    
}

bool MiddleMenu::LoadResources()
{
    indicatorRPMObject.LoadResources();
    m_textureLogo.LoadFromFile("Resources/audi.png");
    m_textureLine.LoadFromFile("Resources/line.png");
    m_textureSquare.LoadFromFile("Resources/square.png");
    
    return true;
}

void MiddleMenu::Draw(Shader &shaderTexture, Shader &shaderFont)
{
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, m_fAlphaSquare);
    gRenderer.DrawPictureScaled(m_textureSquare, 352.0f, 574.5f, 0.0f, 0.9f, shaderTexture);
    // Logo
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, m_fAlphaStart);
    gRenderer.DrawPictureInAspectRatio(m_textureLogo, 595.0f, 520.0f, 0.0f, shaderTexture);
    
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    gRenderer.DrawPictureScaled(m_textureLine, 0.5f, 835.0f, 0.0f, 1.8f, shaderTexture);
    
    //Indicatores RPM and KMH
    indicatorRPMObject.Draw(shaderTexture, shaderFont);
}

void MiddleMenu::Process()
{
    std::function<void()>callbackKeyIn = [&]
    {
        m_fAlphaStart = sineKeyIn.easeIn(m_timerStart.GetTimes(), 0.0f, 1.0f, 50);
        if(m_fAlphaStart >= 1.0f)
        {
            m_timerStart.Stop();
            m_eState = eKeyIn;
            indicatorRPMObject.StartTest();
            m_timerDisappearLogo.Start(g_usTimerDisappearLogo, 15);
            
        }
    };
    
    std::function<void()>callbackDisappearLogo = [&]
    {
        m_fAlphaStart = m_fAlphaStart - sineKeyIn.easeIn(m_timerDisappearLogo.GetTimes(), 0.0f, 1.0f, 1000);
        if(m_fAlphaStart <= 0.0f)
        {
            m_fAlphaStart = 0.0f;
            m_fAlphaSquare = sineKeyIn.easeIn(m_timerDisappearLogo.GetTimes(), 0.0f, 0.7f, 200);
            
            if(m_fAlphaSquare >= 0.7f)
            {
                m_timerDisappearLogo.Stop();
            }
        }
    };
    
    std::function<void()>callbackKeyOut = [&]
    {
        m_fAlphaSquare = m_fAlphaSquare - sineKeyIn.easeIn(m_timerStop.GetTimes(), 0.0f, 1.0f, 200);
        if(m_fAlphaSquare <= 0.0f)
        {
            m_fAlphaStart = sineKeyIn.easeIn(m_timerStop.GetTimes(), 0.0f, 1.0f, 250);
            if(m_fAlphaStart <= 0.0f)
            {
                m_timerStop.Stop();
                m_eState = eKeyOut;
                indicatorRPMObject.SetStateOfEngine(eOff);
            }
        }
    };
    
    m_timerStart.Tick(callbackKeyIn);
    m_timerDisappearLogo.Tick(callbackDisappearLogo);
    m_timerStop.Tick(callbackKeyOut);
}

void MiddleMenu::KeyIn()
{
    if(!m_timerStart.IsStarted() && m_eState == eKeyOut)
    {
        m_timerStart.Start(g_usTimerKeyIn, 15);
    }
}

void MiddleMenu::KeyOut()
{
    if(!m_timerStop.IsStarted() &&
       m_eState == eKeyIn &&
       indicatorRPMObject.GetStateOfEngine() == eReadyForStart)
    {
        m_timerStop.Start(g_usTimerKeyOut, 15);
    }
    
}

void MiddleMenu::HandleEvent(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !bPressedNow)
    {
        KeyIn();
        KeyOut();
        bPressedNow = true;
        return;
    }
    
    bPressedNow = false;
}
