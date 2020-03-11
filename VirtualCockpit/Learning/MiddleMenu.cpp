//
//  MiddleMenu.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 1.03.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include "MiddleMenu.hpp"
const unsigned short g_usSizeOfFonts = 40;
const unsigned short g_usTimerGlobal = 999;
const unsigned short g_usTimerKeyIn = 0;
const unsigned short g_usTimerDisappearLogo = 1;
const unsigned short g_usTimerKeyOut = 2;

MiddleMenu::MiddleMenu()
{
    
}

bool MiddleMenu::LoadResources()
{
    indicatorRPMObject.LoadResources();
    bcObject.LoadResources();
    m_textureLogo.LoadFromFile("Resources/audi.png");
    m_textureLine.LoadFromFile("Resources/line.png");
    m_textureLineSmall.LoadFromFile("Resources/line_small.png");
    m_textureSquare.LoadFromFile("Resources/square.png");
    m_textureLogoPetrol.LoadFromFile("Resources/fuel.png");
    m_textureLogoWater.LoadFromFile("Resources/water.png");
    m_fontNormal.SetFont("Fonts/Normal.ttf", g_usSizeOfFonts);
    m_fontBold.SetFont("Fonts/Bold.ttf", g_usSizeOfFonts);
    m_fontExtendedBold.SetFont("Fonts/ExtendedBold.ttf", g_usSizeOfFonts);
    
    return true;
}

void MiddleMenu::Draw(Shader &shaderTexture, Shader &shaderFont, Shader &shaderLines)
{
    //Up Line
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, m_fAlphaSquare);
    gRenderer.DrawPictureScaled(m_textureLineSmall, 399.5f, 472.5f, 0.0f, 0.8f, shaderTexture);
    //Square
    gRenderer.SetColor(0.0f, 0.0f, 0.0f, m_fAlphaSquare);
    gRenderer.DrawSquare(354.5f, 475.0f, 0.0f, 720.0f, 359.5f, shaderLines);
    // Logo
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, m_fAlphaStart);
    gRenderer.DrawPictureInAspectRatio(m_textureLogo, 595.0f, 520.0f, 0.0f, shaderTexture);
    
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    gRenderer.DrawPictureScaled(m_textureLine, 0.5f, 835.0f, 0.0f, 1.8f, shaderTexture);
    
    //BoardComputer
    bcObject.Draw(shaderTexture, shaderFont, shaderLines, m_fAlphaSquare);
    
    //Indicatores RPM and KMH
    indicatorRPMObject.Draw(shaderTexture, shaderFont);
    
    //Petrol Meter
    m_fontBold.SetColor(1.0f,0.0f,0.0f,1.0f);
    m_fontBold.DrawText("R", 1046.5f, 894.0f, 0.6f, shaderFont); //R
    
    gRenderer.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    gRenderer.DrawSquare(1070.5f, 876.5, 0.0f, 3.0f, 10.0f, shaderLines);//Tick after R (red)
    
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    gRenderer.DrawSquare(1112.0f, 876.5, 0.0f, 3.0f, 16.0f, shaderLines);//Tick after R white
    gRenderer.DrawSquare(1236.5f, 876.5, 0.0f, 3.0f, 16.0f, shaderLines);//Tick after 1/2
    
    m_fontBold.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_fontBold.DrawText("1/2", 1154.0f, 894.0f, 0.6f, shaderFont);//1/2
    
    m_fontBold.DrawText("1/1", 1276.5f, 894.0f, 0.6f, shaderFont);//1/1
    
    //Petrol Logo
    gRenderer.DrawPictureScaled(m_textureLogoPetrol, 1332.0f, 860.0f, 0.0f, 0.5f, shaderTexture);
    
    //Water Meter
    gRenderer.DrawPictureScaled(m_textureLogoWater, 62.5f, 860.0f, 0.0f, 0.6f, shaderTexture);
    m_fontBold.SetColor(1.0f,1.0f,1.0f,1.0f);
    m_fontBold.DrawText("50", 115.5f, 894.0f, 0.6f, shaderFont); //50
    
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    gRenderer.DrawSquare(193.0f, 876.5, 0.0f, 3.0f, 16.0f, shaderLines);//Tick after 50 white
    m_fontBold.DrawText("90", 238.0f, 894.0f, 0.6f, shaderFont);//90
    gRenderer.DrawSquare(312.0f, 876.5, 0.0f, 3.0f, 16.0f, shaderLines);//Tick after 90
    m_fontBold.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    m_fontBold.DrawText("130", 357.0f, 894.0f, 0.6f, shaderFont);//130
    m_fontBold.DrawText("o", 411.5f, 887.0f, 0.5f, shaderFont);//o
    m_fontBold.DrawText("C", 424.0f, 894.0f, 0.6f, shaderFont);//C
    
    //Meters Petrol
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, m_fAlphaSquare);
    for(unsigned int i = 0; i <= m_unMaxLevelPetrol; i++)
    {
        float fMeterCounterOffset = 18.5f;
        gRenderer.DrawSquare(1055.0f + fMeterCounterOffset * i, 865.0f, 0.0f, 16.0f, 3.0f, shaderLines);
    }
    
    //Meters Water
    for(unsigned int i = 0; i <= m_unMaxLevelWater; i++)
    {
        float fMeterCounterOffset = 18.5f;
        gRenderer.DrawSquare(135.0f + fMeterCounterOffset * i, 865.0f, 0.0f, 16.0f, 3.0f, shaderLines);
    }
    
    //Clock and Temperature Bottom of Middle menu
    DrawClock(522.0f, 865.0f, shaderFont);
    DrawTemperature(818.0f, 865.0f, shaderFont);
}

void MiddleMenu::DrawClock(float fX, float fY, Shader &shaderFont)
{
    time_t now = time(0);
    tm *dateTime = localtime(&now);
    
    std::string sHour = std::to_string(dateTime->tm_hour);
    std::string sMinutes = std::to_string(dateTime->tm_min);
    if(dateTime->tm_min <= 9)
    {
        sMinutes = "0" + std::to_string(dateTime->tm_min);
    }
    std::string sClock = sHour + ":" + sMinutes;
    
    m_fontBold.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
    m_fontBold.DrawText(sClock, fX, fY, 0.7f, shaderFont);
}

void MiddleMenu::DrawTemperature(float fX, float fY, Shader &shaderFont)
{
    std::string sSign = "+";
    std::string sDegrees = "o";
    std::string sCelsius = "C";
    std::string sTemp = "13.5";
    m_fontBold.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
    m_fontBold.DrawText(sSign, fX - 17.0f, fY - 4.0f, 0.7f, shaderFont);
    m_fontBold.DrawText(sTemp, fX, fY, 0.7f, shaderFont);
    m_fontBold.DrawText(sDegrees, fX + 70.0f, fY - 10.0f, 0.5f, shaderFont);
    m_fontBold.DrawText(sCelsius, fX + 82.0f, fY, 0.7f, shaderFont);
}

void MiddleMenu::Process()
{
    std::function<void()>callbackGlobal = [&]
    {
        switch (indicatorRPMObject.GetStateOfGearBox())
        {
            case eParking:
            case eNeutral:
            case eReverse:
                fConsumptRatio = 1.0f;
                break;
            case eDrive:
                fConsumptRatio = 5.0f;
                break;
            case eDynamic:
                fConsumptRatio = 15.0f;
                break;
            default:
                fConsumptRatio = 0.0f;
                break;
        }
        if(indicatorRPMObject.GetStateOfAcceleration() != eDecelerating)
        {
            float fCurrentConsumpt = (indicatorRPMObject.GetRPM() * indicatorRPMObject.GetRPM()) * fConsumptRatio;
            const float fTollerance = 10.0f;
            
            if(fConsumption + fTollerance >= fCurrentConsumpt)
            {
                fConsumption = fCurrentConsumpt;
            }
            else
            {
                fConsumption += fTollerance;
            }
            
        }
        else if(indicatorRPMObject.GetStateOfAcceleration() == eDecelerating)
        {
            const float fTollerance = 10.0f;
            const float fMinChargePos = -40.0f;
            
            fConsumption -= fTollerance;
            
            if(fConsumption <= fMinChargePos)
            {
                fConsumption = fMinChargePos;
            }
        }
        
        bcObject.SetConsmption(fConsumption);
    };
    
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
                bcObject.SetState(eDateAndTime);
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
                bcObject.SetState(eHide);
                m_timerGlobal.Stop();
            }
        }
    };
    
    m_timerGlobal.Tick(callbackGlobal);
    m_timerStart.Tick(callbackKeyIn);
    m_timerDisappearLogo.Tick(callbackDisappearLogo);
    m_timerStop.Tick(callbackKeyOut);
}

void MiddleMenu::KeyIn()
{
    if(!m_timerStart.IsStarted() && m_eState == eKeyOut)
    {
        if(!m_timerGlobal.IsStarted())
        {
            m_timerGlobal.Start(g_usTimerGlobal, 15);
        }
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
