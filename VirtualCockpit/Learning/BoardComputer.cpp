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
    m_textureLineSmall.LoadFromFile("../src/Resources/line_small.png");
    m_textureBattery.LoadFromFile("../src/Resources/battery.png");
    m_fontNormal.SetFont("../src/Fonts/Normal.ttf", g_usSizeOfFonts);
    m_fontBold.SetFont("../src/Fonts/Bold.ttf", g_usSizeOfFonts);
    m_fontExtendedBold.SetFont("../src/Fonts/ExtendedBold.ttf", g_usSizeOfFonts);
    
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
        float fOffsetSDay = 0.0f;
        switch (dateTime->tm_wday)
        {
            case 1:
                sDay = "MONDAY";
                fOffsetSDay = 130.0f;
                break;
            case 2:
                 sDay = "TUESDAY";
                fOffsetSDay = 130.0f;
                 break;
            case 3:
                 sDay = "WEDNESDAY";
                fOffsetSDay = 105.0f;
                 break;
            case 4:
                sDay = "THURSDAY";
                fOffsetSDay = 120.0f;
                 break;
            case 5:
                 sDay = "FRIDAY";
                fOffsetSDay = 145.0f;
                 break;
            case 6:
                 sDay = "SATURDAY";
                fOffsetSDay = 120.0f;
                 break;
            case 0:
                 sDay = "SUNDAY";
                fOffsetSDay = 135.0f;
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
        m_fontBold.DrawText(sDay, fX + fOffsetSDay, fY + 160.0f, 0.7f, shaderFont);
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
        m_fontBold.DrawText(m_sInstantConsumtion, 575.0f, fY + 110.0f, 1.0f, shaderFont);
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
        
        m_fontBold.DrawText(m_sInstantKM + " km", fX + 150.0f, fY + 50.0f, 0.6f, shaderFont);
        m_fontBold.DrawText(m_sKMAverage + " km/h", fX + 140.0f, fY + 100.0f, 0.6f, shaderFont);
        m_fontBold.DrawText(m_sInstantConsumtion, fX + 110.0f, fY + 150.0f, 0.6f, shaderFont);
        
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
        m_fontBold.DrawText(std::to_string(m_nKM) + " km", fX + 110.0f, fY + 50.0f, 0.6f, shaderFont);
        m_fontBold.DrawText("Coolant " + std::to_string(m_nWaterTemp), fX + 110.0f, fY + 100.0f, 0.6f, shaderFont);
        m_fontBold.DrawText("Oil " + std::to_string(m_nOilTemp), fX + 110.0f, fY + 150.0f, 0.6f, shaderFont);
        m_fontBold.DrawText("o ", fX + 250.0f, fY + 90.0f, 0.3f, shaderFont);
        m_fontBold.DrawText("C ", fX + 257.0f, fY + 100.0f, 0.6f, shaderFont);
        m_fontBold.DrawText("o ", fX + 205.0f, fY + 140.0f, 0.3f, shaderFont);
        m_fontBold.DrawText("C ", fX + 212.0f, fY + 150.0f, 0.6f, shaderFont);
        m_fontBold.DrawText("Oil Pressure " + std::to_string(m_nOilPressure) + " bar", fX + 110.0f, fY + 200.0f, 0.6f, shaderFont);
        m_fontBold.DrawText("Boost", fX + 110.0f, fY + 250.0f, 0.6f, shaderFont);
        gRenderer.DrawSquare(fX + 190.0f, fY + 245.0f, 0.0f, m_fBoost, 4.0f, shaderLines);
        
        m_fBoost = m_fConsumtion / 2;
        if(m_fBoost <= 0.0f)
        {
            m_fBoost = 0.0f;
        }
        else if(m_fBoost >= m_fMaxBoost)
        {
            m_fBoost = m_fMaxBoost;
        }
        
        m_nOilPressure = m_fBoost * 0.06;
        
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
    
    float fConsumptFactor = m_fMaxConsumption / 300.0f;
    m_fInstantConsumpt = m_fConsumtion * fConsumptFactor;
    m_nCyclesConsumpt += 1;
    
    float fInstantConsumptCalc = (m_fInstantConsumpt += m_fInstantConsumptMemory) / m_nCyclesConsumpt;
    if(fInstantConsumptCalc >= m_fMaxConsumption)
    {
        fInstantConsumptCalc = m_fMaxConsumption;
    }
    else if(fInstantConsumptCalc <= 0.0f)
    {
        fInstantConsumptCalc = 0.0f;
    }
    int nIntegerConsumpt = fInstantConsumptCalc;
    int nIndegerConsumptAfterPoint = (int)(fInstantConsumptCalc * 10) % 10;
    
    m_sInstantConsumtion = std::to_string(nIntegerConsumpt) +
    "." +
    std::to_string(nIndegerConsumptAfterPoint) + " lt/100km";
    
    m_fInstantConsumptMemory = m_fInstantConsumpt;
    
}

void BoardComputer::SetAfterStart(float fKMH)
{
    int nCurrentKMH = fKMH;
   nCurrentKMH += m_nKMAverage / m_nSeconds;
    m_nMetersInSeconds = fKMH / 3.6;
    
    if(m_nCyclesConsumpt % 60 == 0)
    {
        m_nSeconds += 1;
        m_nMeters += (m_nMetersInSeconds / 10) % 10;
    
        if(m_nSeconds % 5 == 0)
        {
            m_nKMAverage = nCurrentKMH;
        }
        
        if(m_nMeters >= 10)
        {
            m_nMeters = 0;
            m_nMetersHundreds += 1;
        }
        if(m_nMetersHundreds >= 10)
        {
            m_nMetersHundreds = 0;
            m_nKM += 1;
        }
    }
    
    m_sInstantKM = std::to_string(m_nKM) + "." + std::to_string(m_nMetersHundreds);
    m_sKMAverage = std::to_string(m_nKMAverage);
    
    
}

void BoardComputer::SetCoolantTemp(int &nCoolantTemp)
{
    if(m_nWaterTemp >= 50)
    {
        if(m_nWaterTemp > 50 && m_nWaterTemp <= 60)
        {
            nCoolantTemp = 0;
        }
        else if(m_nWaterTemp > 60 && m_nWaterTemp <= 70)
        {
            nCoolantTemp = 2;
        }
        else if(m_nWaterTemp > 70 && m_nWaterTemp <= 80)
        {
            nCoolantTemp = 3;
        }
        else if(m_nWaterTemp > 80 && m_nWaterTemp <= 90)
        {
            nCoolantTemp = 5;
        }
        else if(m_nWaterTemp > 90 && m_nWaterTemp <= 100)
        {
            nCoolantTemp = 6;
        }
    }

    m_nWaterTemp = m_nOilTemp - 1.0f;
    if(m_nWaterTemp <= 0)
    {
        m_nWaterTemp = 0;
    }
    if(m_nWaterTemp >= 92)
    {
        m_nWaterTemp = 92;
    }
    
    m_nOilTemp = m_nOilTemp + m_nOilPressure - 0.5f;
    if(m_nOilTemp >= m_nMaxOilTemp)
    {
        m_nOilTemp = m_nMaxOilTemp;
    }
    else if(m_nOilTemp <= m_nWaterTemp)
    {
        m_nOilTemp = m_nWaterTemp;
    }
    
}
