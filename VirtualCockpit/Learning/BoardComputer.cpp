//
//  BoardComputer.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 9.03.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include "BoardComputer.hpp"
const unsigned short g_usSizeOfFonts = 40;
const unsigned short g_usTimerMovement = 1;
const float fInitialYPosLabels = 533.0f;
const float fScrollerStep = 31.5f;

BoardComputer::BoardComputer()
{
    
}

bool BoardComputer::LoadResources()
{
    m_textureLineSmall.LoadFromFile("Resources/line_small.png");
    m_textureBattery.LoadFromFile("Resources/battery.png");
    m_fontNormal.SetFont("Fonts/Normal.ttf", g_usSizeOfFonts);
    m_fontBold.SetFont("Fonts/Bold.ttf", g_usSizeOfFonts);
    m_fontExtendedBold.SetFont("Fonts/ExtendedBold.ttf", g_usSizeOfFonts);
    
    return true;
}

void BoardComputer::Draw(Shader &shaderTexture, Shader &shaderFont, Shader &shaderLines, float fAlpha)
{
    float fYCursor = fInitialYPosLabels + (fGlobalY * 2);
    float fYDateTime = fInitialYPosLabels - fGlobalY;
    float fYFuelConsumpt = (fInitialYPosLabels + fScrollerStep) - fGlobalY;
    float fYLongTermMemory = (fInitialYPosLabels + (fScrollerStep * 2)) - fGlobalY;
    float fYCarInfo = (fInitialYPosLabels + (fScrollerStep * 3)) - fGlobalY;
    
    DrawCursor(936.5f, fYCursor, shaderTexture, shaderFont, shaderLines, fAlpha);
    DrawDateAndTime(512.0f, fYDateTime, shaderTexture, shaderFont, shaderLines, fAlpha - ((fInitialYPosLabels - fYDateTime) / 40.0f));
    DrawFuelConsupmt(512.0f, fYFuelConsumpt, shaderTexture, shaderFont, shaderLines, fAlpha - ((fInitialYPosLabels - fYFuelConsumpt) / 40.0f));
    DrawLongTerm(512.0f, fYLongTermMemory, shaderTexture, shaderFont, shaderLines, fAlpha - ((fInitialYPosLabels - fYLongTermMemory) / 40.0f));
    DrawCarInfo(512.0f, fYCarInfo, shaderTexture, shaderFont, shaderLines, fAlpha - ((fInitialYPosLabels - fYCarInfo) / 40.0f));
}

void BoardComputer::Process()
{
    std::function<void()>callbackMovement = [&]
    {
        if(m_fDestPosLabel < m_fInitialPosLabel)
        {
            fGlobalY -= 1.0f;
            if(fGlobalY <= m_fDestPosLabel)
            {
                fGlobalY = m_fDestPosLabel;
                m_timerMovement.Stop();
            }
        }
        else if(m_fDestPosLabel > m_fInitialPosLabel)
        {
            fGlobalY += 1.0f;
            if(fGlobalY >= m_fDestPosLabel)
            {
                fGlobalY = m_fDestPosLabel;
                m_timerMovement.Stop();
            }
        }
        
    };
    
    m_timerMovement.Tick(callbackMovement);
}

void BoardComputer::HandleEvent(GLFWwindow *window)
{
    if(m_eStateOfBoardComputer == eHide)
    {
        return;
    }
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !bPressedNow)
    {
        if(!m_timerMovement.IsStarted() && m_eStateOfBoardComputer != eDateAndTime)
        {
            m_fInitialPosLabel = fGlobalY;
            m_fDestPosLabel = 0.0f;
            m_timerMovement.Start(g_usTimerMovement, 15);
        }
        
        bPressedNow = true;
        return;
    }
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !bPressedNow)
    {
        if(!m_timerMovement.IsStarted() && m_eStateOfBoardComputer != eFuelConsumption)
        {
            m_fInitialPosLabel = fGlobalY;
            m_fDestPosLabel = 32.5f;
            m_timerMovement.Start(g_usTimerMovement, 15);
        }
        
        bPressedNow = true;
        return;
    }
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !bPressedNow)
    {
        if(!m_timerMovement.IsStarted() && m_eStateOfBoardComputer != eLongTermMemory)
        {
            m_fInitialPosLabel = fGlobalY;
            m_fDestPosLabel = 65.0f;
            m_timerMovement.Start(g_usTimerMovement, 15);
        }
        
        bPressedNow = true;
        return;
    }
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !bPressedNow)
    {
        if(!m_timerMovement.IsStarted() && m_eStateOfBoardComputer != eCarInfo)
        {
            m_fInitialPosLabel = fGlobalY;
            m_fDestPosLabel = 97.5f;
            m_timerMovement.Start(g_usTimerMovement, 15);
        }
        
        bPressedNow = true;
        return;
    }
    
    if(fGlobalY >= 0 && fGlobalY <= fScrollerStep)
    {
        m_eStateOfBoardComputer = eDateAndTime;
    }
    else if(fGlobalY > fScrollerStep && fGlobalY <= fScrollerStep * 2)
    {
        m_eStateOfBoardComputer = eFuelConsumption;
    }
    else if(fGlobalY > fScrollerStep * 2 && fGlobalY < fScrollerStep * 3)
    {
        m_eStateOfBoardComputer = eLongTermMemory;
    }
    else if(fGlobalY >= fScrollerStep * 3 && fScrollerStep <= fScrollerStep * 4)
    {
        m_eStateOfBoardComputer = eCarInfo;
    }
    
    bPressedNow = false;
}

void BoardComputer::DrawDateAndTime(float fX, float fY, Shader &shaderTexture, Shader &shaderFont, Shader &shaderLines, float fAlpha)
{
    if(m_eStateOfBoardComputer == eDateAndTime)
    {
        if(fAlpha >= 0.7f)
        {
            fAlpha = 0.7f;
        }
        else if(fAlpha <= 0.0f)
        {
            fAlpha = 0.0f;
        }
        
        //Clock
        time_t now = time(0);
        tm *dateTime = localtime(&now);
        
        std::string sHour = std::to_string(dateTime->tm_hour);
        std::string sMinutes = std::to_string(dateTime->tm_min);
        if(dateTime->tm_min <= 9)
        {
            sMinutes = "0" + std::to_string(dateTime->tm_min);
        }
        std::string sClock = sHour + ":" + sMinutes;
        
        //Day Of Week
        std::string sDay;
        switch (dateTime->tm_wday)
        {
            case 1:
                sDay = "MONDAY";
                break;
            case 2:
                 sDay = "TUESDAY";
                 break;
            case 3:
                 sDay = "WEDNESDAY";
                 break;
            case 4:
                sDay = "THURSDAY";
                 break;
            case 5:
                 sDay = "FRIDAY";
                 break;
            case 6:
                 sDay = "SATURDAY";
                 break;
            case 7:
                 sDay = "SUNDAY";
                 break;
                
            default:
                sDay = "NAN";
                break;
        }
        
        //Current Date
        std::string sDate = std::to_string(dateTime->tm_mday);
        std::string sMonth = std::to_string(1 +dateTime->tm_mon);
        std::string sYear = std::to_string(1900+dateTime->tm_year);
        std::string sCurrentDate = sDate + "/" + sMonth + "/" + sYear;
    
        m_fontBold.SetColor(1.0f, 1.0f, 1.0f, fAlpha);
        m_fontBold.DrawText("Date & Time", fX + 85.0f, fY - 10.0f, 0.9f, shaderFont);
        gRenderer.SetColor(1.0f, 1.0f, 1.0f, fAlpha);
        gRenderer.DrawPictureScaled(m_textureLineSmall, fX, fY, 0.0f, 0.5f, shaderTexture);
        m_fontBold.DrawText(sClock, fX + 100.0f, fY + 120.0f, 1.5f, shaderFont);
        m_fontBold.DrawText(sDay, fX + 130.0f, fY + 160.0f, 0.7f, shaderFont);
        m_fontBold.DrawText(sCurrentDate, fX + 115.0f, fY + 200.0f, 0.7f, shaderFont);
    }
}

void BoardComputer::DrawFuelConsupmt(float fX, float fY, Shader &shaderTexture, Shader &shaderFont, Shader &shaderLines, float fAlpha)
{
    if(m_eStateOfBoardComputer == eFuelConsumption)
    {
        if(fAlpha >= 0.7f)
        {
            fAlpha = 0.7f;
        }
        else if(fAlpha <= 0.0f)
        {
            fAlpha = 0.0f;
        }
        m_fontBold.SetColor(1.0f, 1.0f, 1.0f, fAlpha);
        m_fontBold.DrawText("Fuel Consum", fX + 85.0f, fY - 10.0f, 0.9f, shaderFont);
        gRenderer.SetColor(1.0f, 1.0f, 1.0f, fAlpha);
        gRenderer.DrawPictureScaled(m_textureLineSmall, fX, fY, 0.0f, 0.5f, shaderTexture);
        if(m_fConsumtion < 0)
        {
            gRenderer.SetColor(0.0f, 1.0f, 0.0f, fAlpha);
        }
        gRenderer.DrawPictureScaled(m_textureBattery, 516.0f, fY + 188.0f, 0.0f, 0.6f, shaderTexture);
        gRenderer.DrawSquare(558.0f, fY + 183.0f, 0.0f, m_fConsumtion, 4.0f, shaderLines);
        m_fontBold.DrawText("10", 625.0f, fY + 220.0f, 0.6f, shaderFont);
        m_fontBold.DrawText("20", 705.0f, fY + 220.0f, 0.6f, shaderFont);
        m_fontBold.DrawText("30", 785.5f, fY + 220.0f, 0.6f, shaderFont);
        m_fontBold.DrawText("40", 867.5f, fY + 220.0f, 0.6f, shaderFont);
    }
}

void BoardComputer::DrawLongTerm(float fX, float fY, Shader &shaderTexture, Shader &shaderFont, Shader &shaderLines, float fAlpha)
{
    if(m_eStateOfBoardComputer == eLongTermMemory)
    {
        if(fAlpha >= 0.7f)
        {
            fAlpha = 0.7f;
        }
        else if(fAlpha <= 0.0f)
        {
            fAlpha = 0.0f;
        }
        m_fontBold.SetColor(1.0f, 1.0f, 1.0f, fAlpha);
        m_fontBold.DrawText("After Start", fX + 95.0f, fY - 10.0f, 0.9f, shaderFont);
        gRenderer.SetColor(1.0f, 1.0f, 1.0f, fAlpha);
        gRenderer.DrawPictureScaled(m_textureLineSmall, fX, fY, 0.0f, 0.5f, shaderTexture);
    }
}

void BoardComputer::DrawCarInfo(float fX, float fY, Shader &shaderTexture, Shader &shaderFont, Shader &shaderLines, float fAlpha)
{
    if(m_eStateOfBoardComputer == eCarInfo)
    {
        if(fAlpha >= 0.7f)
        {
            fAlpha = 0.7f;
        }
        else if(fAlpha <= 0.0f)
        {
            fAlpha = 0.0f;
        }
        m_fontBold.SetColor(1.0f, 1.0f, 1.0f, fAlpha);
        m_fontBold.DrawText("Car Information", fX + 55.0f, fY - 10.0f, 0.9f, shaderFont);
        gRenderer.SetColor(1.0f, 1.0f, 1.0f, fAlpha);
        gRenderer.DrawPictureScaled(m_textureLineSmall, fX, fY, 0.0f, 0.5f, shaderTexture);
    }
}

void BoardComputer::DrawCursor(float fX, float fY, Shader &shaderTexture, Shader &shaderFont, Shader &shaderLines, float fAlpha)
{
    if(m_eStateOfBoardComputer != eHide)
    {
        gRenderer.SetColor(1.0f, 1.0f, 1.0f, fAlpha);
        gRenderer.DrawSquare(fX, 533.0f, 0.0f, 1.0f, 260.0f, shaderLines);//Cursor
        gRenderer.DrawSquare(fX - 2.0f, fY, 0.0f, 2.0f, 65.0f, shaderLines);//Cursor slide
    }
}

void BoardComputer::SetConsmption(float fConsumption)
{
    if(fConsumption >= 340.0f)
    {
        fConsumption = 340.0f;
    }
    m_fConsumtion = fConsumption;
}
