//
//  IndicatorRPM.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 25.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include "IndicatorRPM.hpp"

const unsigned short g_usSizeOfFonts = 40;
const unsigned short g_usTimerTestID = 1;
const unsigned short g_usTimerStartEngineID = 2;
const unsigned short g_usTimerStopEngineID = 3;

IndicatorRPM::IndicatorRPM()
{
    
}

bool IndicatorRPM::LoadResources()
{
    m_textureBackground.LoadFromFile("Resources/tacho.png");
    m_textureMarker.LoadFromFile("Resources/marker.png");
    m_textureNeedle.LoadFromFile("Resources/needle.png");
    m_textureRsLogo.LoadFromFile("Resources/rs_logo.png");
    
    m_fontNormal.SetFont("Fonts/Normal.ttf", g_usSizeOfFonts);
    m_fontBold.SetFont("Fonts/Bold.ttf", g_usSizeOfFonts);
    m_fontExtendedBold.SetFont("Fonts/ExtendedBold.ttf", g_usSizeOfFonts);
    
    indicatorKMHObject.LoadResources();
    
    return true;
}

void IndicatorRPM::Draw(Shader &shaderTexture, Shader& shaderFont)
{
    //Indicator KMH
    indicatorKMHObject.Draw(shaderTexture, shaderFont);
    
    //Indicator RPM
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    gRenderer.DrawPictureInAspectRatio(m_textureBackground, 20.0f, 400.0f, 0.0f, shaderTexture);
    m_fRedColorMarker = m_fAngle * 0.0075;
    gRenderer.SetColor(1.0f, 1.0f - m_fRedColorMarker, 1.0f - m_fRedColorMarker, 1.0f);
    gRenderer.DrawPictureInAspectRatio(m_textureMarker, 20.0f, 400.0f, 0.0f, shaderTexture);
    //Text
    m_fontBold.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_fontBold.DrawText("OFF", 125.5f, 778.0f, 0.45f, shaderFont);
    m_fontBold.DrawText("1/min x 1000", 217.0f, 827.0f, 0.30f, shaderFont);
    m_fontNormal.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
    m_fontNormal.DrawText("1", 77.5f, 692.0f, 1.0f, shaderFont);
    m_fontNormal.DrawText("2", 87.0f, 586.5f, 1.0f, shaderFont);
    m_fontNormal.DrawText("3", 144.5f, 512.0f, 1.0f, shaderFont);
    m_fontNormal.DrawText("4", 243.0f, 482.5f, 1.0f, shaderFont);
    m_fontNormal.DrawText("5", 345.0f, 513.5f, 1.0f, shaderFont);
    m_fontNormal.DrawText("6", 407.0f, 588.5f, 1.0f, shaderFont);
    m_fontNormal.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    m_fontNormal.DrawText("7", 421.5f, 693.5f, 1.0f, shaderFont);
    m_fontNormal.DrawText("8", 373.0f, 777.0f, 1.0f, shaderFont);
    //RS Logo
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
    gRenderer.DrawPictureScaled(m_textureRsLogo, 195.0f, 531.5f, 0.0f, 0.1f, shaderTexture);
    //Needle
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    gRenderer.DrawPictureRotated(m_textureNeedle, 20.0f, 400.0f, 0.0f, m_fAngle,
                                 -35.0f,
                                 (float)m_textureBackground.GetWidth() / 2,
                                 (float)m_textureBackground.GetHeight() / 2,
                                 0.0f, shaderTexture);
    //Menu
    m_fontNormal.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    float xGear = 238.5f;
    float xMode = 237.0f;
    if(m_eStateOfGearbox == eDrive)
    {
        xGear = 220.5f;
    }
    
    if(m_eStateOfGearbox == eDynamic)
    {
        xGear = 220.5f;
        xMode = 223.0f;
        m_fontNormal.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    }
    m_fontBold.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
    m_fontBold.DrawText(m_sGearIndicator, xGear, 664.5f, 1.5f, shaderFont);
    
    //GearBox Mode
    m_fontNormal.DrawText(m_sGearBoxMode, xMode, 800.0f, 0.4f, shaderFont);
}

void IndicatorRPM::StartTest()
{
    if(m_eStateOfEngine == eOff && !m_timerTest.IsStarted())
    {
        indicatorKMHObject.StartTest();
        m_timerTest.Start(g_usTimerTestID, 15);
        m_eStateOfEngine = eTest;
    }
}

void IndicatorRPM::TickTest()
{
    const float fInitialRPM = 0.0f;
    const float fFinalRPM = 8.0f;
    m_fRPM = sineNeedle.easeIn(m_timerTest.GetTimes(), fInitialRPM, fFinalRPM/2.0f, 25);
    m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
    
    if(m_fRPM <= fInitialRPM)
    {
        m_timerTest.Stop();
        m_eStateOfEngine = eReadyForStart;
        StartEngine();
    }
}

void IndicatorRPM::StartEngine()
{
    if(m_eStateOfEngine == eReadyForStart && !m_timerStartEngine.IsStarted())
    {
        m_timerStartEngine.Start(g_usTimerStartEngineID, 15);
    }
}

void IndicatorRPM::StopEngine()
{
    if(m_eStateOfEngine == eStarted && !m_timerStopEngine.IsStarted())
    {
        m_timerStopEngine.Start(g_usTimerStopEngineID, 15);
    }
}

void IndicatorRPM::TickStartEngine()
{
    const float fInitialRPM = 0.0f;
    const float fFinalRPM = m_fMinRPM;
    m_fRPM = sineNeedle.easeIn(m_timerStartEngine.GetTimes(), fInitialRPM, fFinalRPM, 10);
    m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
    
    if(m_fRPM >= fFinalRPM)
    {
        m_timerStartEngine.Stop();
        m_eStateOfEngine = eStarted;
    }
}

void IndicatorRPM::TickStopEngine()
{
    const float fInitialRPM = 0.0f;
    const float fFinalRPM = m_fRPM;
    m_fRPM = fFinalRPM - sineNeedle.easeOut(m_timerStopEngine.GetTimes(), fInitialRPM, fFinalRPM, 10);
    m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
    
    if(m_fRPM <= 0.0f)
    {
        m_timerStopEngine.Stop();
        m_eStateOfEngine = eReadyForStart;
    }
}

void IndicatorRPM::Process()
{
    std::function<void()>callbackTest = [&]
    {
        TickTest();
    };
    
    std::function<void()>callbackStartEngine = [&]
    {
        TickStartEngine();
    };
    
    std::function<void()>callbackStopEngine = [&]
    {
        TickStopEngine();
    };
    
    indicatorKMHObject.Process();
    m_timerTest.Tick(callbackTest);
    m_timerStartEngine.Tick(callbackStartEngine);
    m_timerStopEngine.Tick(callbackStopEngine);
    
}

void IndicatorRPM::IncrementRPMInParking()
{
    if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eParking)
    {
        float fIncrementFactor = m_fRPM * 0.026f;

        m_fRPM += fIncrementFactor;;
        
        if(m_fRPM > 3.5f)
        {
            m_fRPM = 3.5f;
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
    }
}

void IndicatorRPM::IncrementRPMInNeutral()
{
    if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eNeutral)
    {
        float fIncrementFactor = m_fRPM * 0.026f;
        
        if(m_fRPM > 7.5f)
        {
            fIncrementFactor = 0.03f;
        }
        m_fRPM += fIncrementFactor;;
        
        if(m_fRPM > m_fMaxRPM)
        {
            m_fRPM = 7.5f;
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
    }
}

void IndicatorRPM::IncrementRPMInReverse()
{
    if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eReverse)
    {
        float fIncrementFactor = m_fRPM * 0.026f;
        if(m_fRPM > 7.5f)
        {
            fIncrementFactor = 0.03f;
        }
        m_fRPM += fIncrementFactor;
        
        if(m_fRPM > m_fMaxRPM)
        {
            m_fRPM = 7.5f;
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
        
        m_fKMH += m_fRPM * 0.052f;
        if(m_fKMH >= 40.0f)
        {
            m_fKMH = 40.0f;
        }
        
        indicatorKMHObject.SetKMH(m_fKMH);
        
    }
}

void IndicatorRPM::IncrementRPMInDrive()
{
    if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eDynamic)
    {
        float fIncrementFactor = m_fRPM * 0.026f;
        if(!m_bTimeToChangeGear)
        {
           m_fRPM += fIncrementFactor / (m_nGearNumber * 2);
        }
        else
        {
            m_fRPM -= 0.2f;
            if(m_fRPM <= 4.2f)
            {
                m_bTimeToChangeGear = false;
            }
        }
        
        
        if(m_fRPM >= 7.2f && m_nGearNumber < 7)
        {
            m_bTimeToChangeGear = true;
            m_nGearNumber += 1;
            m_sGearIndicator = "S" + std::to_string(m_nGearNumber);
        }
        else if(m_fRPM >= 7.5f && m_nGearNumber == 7)
        {
            m_fRPM = 7.5f;
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
        m_fKMH += m_fRPM * (0.154f / m_nGearNumber);
        
        if(m_fKMH >= 320.0f)
        {
            m_fKMH = 320.0f;
        }
        
        indicatorKMHObject.SetKMH(m_fKMH);
        
    }
    else if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eDrive)
    {
        float fIncrementFactor = m_fRPM * 0.013f;
        if(!m_bTimeToChangeGear)
        {
           m_fRPM += fIncrementFactor / (m_nGearNumber * 2);
        }
        else
        {
            m_fRPM -= 0.2f;
            if(m_fRPM <= 1.8f)
            {
                m_bTimeToChangeGear = false;
            }
        }
        
        
        if(m_fRPM >= 3.5f && m_nGearNumber < 7)
        {
            m_bTimeToChangeGear = true;
            m_nGearNumber += 1;
            m_sGearIndicator = "D" + std::to_string(m_nGearNumber);
        }
        else if(m_fRPM >= 4.5f && m_nGearNumber == 7)
        {
            m_fRPM = 4.5f;
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
        m_fKMH += m_fRPM * (0.094f / m_nGearNumber);
        
        if(m_fKMH >= 220.0f)
        {
            m_fKMH = 220.0f;
        }
        
        indicatorKMHObject.SetKMH(m_fKMH);
        
    }
}

void IndicatorRPM::DecrementRPMInParking()
{
    if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eParking)
    {
        float fDecrementFactor = m_fRPM * 0.013f;
        if(m_fRPM < 1.1f)
        {
            fDecrementFactor = 0.01f;
        }
        m_fRPM -= fDecrementFactor;
        
        if(m_fRPM <= m_fMinRPM)
        {
            m_fRPM = m_fMinRPM;
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
    }
}

void IndicatorRPM::DecrementRPMInReverse()
{
    if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eReverse)
    {
        float fDecrementFactor = m_fRPM * 0.013f;
        if(m_fRPM < 1.1f)
        {
            fDecrementFactor = 0.01f;
        }
        m_fRPM -= fDecrementFactor;
        
        if(m_fRPM <= m_fMinRPM)
        {
            m_fRPM = m_fMinRPM;
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
        
        m_fKMH -= m_fRPM * 0.046f;
        
        if(m_fKMH <= 0.0f)
        {
            m_fKMH = 0.0f;
        }
        
        indicatorKMHObject.SetKMH(m_fKMH);
    }
}

void IndicatorRPM::DecrementRPMInNeutral()
{
    if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eNeutral)
    {
        float fDecrementFactor = m_fRPM * 0.013f;
        if(m_fRPM < 1.1f)
        {
            fDecrementFactor = 0.01f;
        }
        m_fRPM -= fDecrementFactor;
        
        if(m_fRPM <= m_fMinRPM)
        {
            m_fRPM = m_fMinRPM;
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
    }
}

void IndicatorRPM::DecrementRPMInDrive()
{
    if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eDynamic)
    {
        if(m_bChangeMinRPM)
        {
            m_fRPM += 0.017f;
            if(m_fRPM >= 1.1f)
            {
                m_fMinRPM = 1.1f;
                m_bChangeMinRPM = false;
            }
        }
        
        float fDecrementFactor = m_fRPM * 0.026f;
        if(!m_bTimeToChangeGear)
        {
           m_fRPM -= fDecrementFactor / (m_nGearNumber * 2);
        }
        else
        {
            m_fRPM += 0.2f;
            if(m_fRPM >= 6.2f)
            {
                m_bTimeToChangeGear = false;
            }
        }

        if(m_fRPM <= m_fMinRPM)
        {
            m_fRPM = m_fMinRPM;
        }
    
        
        if(m_nGearNumber > 1 && m_fRPM <= 3.0f)
        {
            m_bTimeToChangeGear = true;
            m_nGearNumber -= 1;
            m_sGearIndicator = "S" + std::to_string(m_nGearNumber);
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
        m_fKMH -= m_fRPM * (0.154f / m_nGearNumber);
        
        if(m_fKMH <= 0.0f)
        {
            m_fKMH = 0.0f;
        }
        
        indicatorKMHObject.SetKMH(m_fKMH);
    }
    else if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eDrive)
    {
        m_fMinRPM = 0.850f;
        
        float fDecrementFactor = m_fRPM * 0.013f;
        if(!m_bTimeToChangeGear)
        {
           m_fRPM -= fDecrementFactor / (m_nGearNumber * 2);
        }
        else
        {
            m_fRPM += 0.2f;
            if(m_fRPM >= 3.2f)
            {
                m_bTimeToChangeGear = false;
            }
        }

        if(m_fRPM <= m_fMinRPM)
        {
            m_fRPM = m_fMinRPM;
        }
    
        
        if(m_nGearNumber > 1 && m_fRPM <= 1.7f)
        {
            m_bTimeToChangeGear = true;
            m_nGearNumber -= 1;
            m_sGearIndicator = "D" + std::to_string(m_nGearNumber);
        }
        
        m_fAngle = m_fMinPosNeedle + (m_fOffsetBetweenTicks * m_fRPM);
        m_fKMH -= m_fRPM * (0.094f / m_nGearNumber);
        
        if(m_fKMH <= 0.0f)
        {
            m_fKMH = 0.0f;
        }
        
        indicatorKMHObject.SetKMH(m_fKMH);
    }
}

void IndicatorRPM::IncrementRPM()
{
    m_eStateOfAcceleration = eAccelerating;
    IncrementRPMInParking();
    IncrementRPMInReverse();
    IncrementRPMInNeutral();
    IncrementRPMInDrive();
}

void IndicatorRPM::DecrementRPM()
{
    if(m_timerStopEngine.IsStarted())
    {
        return;
    }
    m_eStateOfAcceleration = eDecelerating;
    if(indicatorKMHObject.GetKMH() <= 0.0f)
    {
        m_eStateOfAcceleration = eReleased;
    }

    DecrementRPMInParking();
    DecrementRPMInReverse();
    DecrementRPMInNeutral();
    DecrementRPMInDrive();
}

void IndicatorRPM::ChangeGear(EStatesOfGearbox eStateToChange)
{
    m_eStateOfGearbox = eStateToChange;
    switch (m_eStateOfGearbox)
    {
        case eParking:
            m_sGearIndicator = "P";
            break;
            
        case eReverse:
            m_sGearIndicator = "R";
            break;
            
        case eNeutral:
            m_sGearIndicator = "N";
            break;
            
        case eDrive:
            m_sGearIndicator = "D" + std::to_string(m_nGearNumber);
            m_sGearBoxMode = "AUTO";
            m_bChangeMinRPM = false;
            break;
            
        case eDynamic:
            m_sGearIndicator = "S" + std::to_string(m_nGearNumber);
            m_sGearBoxMode = "DYNAMIC";
            m_bChangeMinRPM = true;
            break;
            
        default:
            break;
    }
    
}

void IndicatorRPM::HandleEvent(GLFWwindow *window)
{

    //StopEngine
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !bPressedNow)
    {
        if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eParking)
        {
            StopEngine();
        }
        
        bPressedNow = true;
        return;
    }
    //StartEngine
    if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && !bPressedNow)
    {
        if(m_eStateOfEngine == eReadyForStart)
        {
            StartEngine();
        }
        
        bPressedNow = true;
        return;

    }
    //Change Gear P
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !bPressedNow)
    {
        if(m_eStateOfEngine == eStarted && m_eStateOfGearbox != eDynamic)
        {
            ChangeGear(eParking);
        }
        
        bPressedNow = true;
        return;

    }
    //Change Gear R
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !bPressedNow)
    {
        if(m_eStateOfEngine == eStarted && m_eStateOfGearbox != eDynamic)
        {
            ChangeGear(eReverse);
        }
        
        bPressedNow = true;
        return;

    }
    //Change Gear N
    if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !bPressedNow)
    {
        if(m_eStateOfEngine == eStarted && m_eStateOfGearbox != eDynamic)
        {
            ChangeGear(eNeutral);
        }
        
        bPressedNow = true;
        return;

    }
    //Change Gear D
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !bPressedNow)
    {

        if(m_eStateOfEngine == eStarted)
        {
            ChangeGear(eDrive);
        }
        
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS &&
           glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS )
        {
            if(m_eStateOfEngine == eStarted && m_eStateOfGearbox == eDrive)
            {
                ChangeGear(eDynamic);
            }
        }
        
        bPressedNow = true;
        return;

    }
    //Acceleration
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        IncrementRPM();
        return;
    }
    
    //Deceleration
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
       m_eStateOfGearbox != eParking &&
       m_eStateOfGearbox != eNeutral)
    {
        DecrementRPM();
        DecrementRPM();
    }
    
    bPressedNow = false;
    
    DecrementRPM();

}

void IndicatorRPM::SetStateOfEngine(EStatesOfRPM eState)
{
    m_eStateOfEngine = eState;
}

