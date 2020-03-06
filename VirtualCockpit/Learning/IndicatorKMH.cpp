//
//  IndicatorKMH.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 27.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include "IndicatorKMH.hpp"
const unsigned short g_usSizeOfFonts = 40;
const unsigned short g_usTimerTestID = 1;

float g_fDigitalKmhX = 1155.5f;
float g_fDigitalKmhY = 645.0f;

IndicatorKMH::IndicatorKMH()
{
    
}

bool IndicatorKMH::LoadResources()
{
    m_textureBackground.LoadFromFile("Resources/tacho.png");
    m_textureMarker.LoadFromFile("Resources/marker.png");
    m_textureNeedle.LoadFromFile("Resources/needle.png");
    
    m_fontNormal.SetFont("Fonts/Normal.ttf", g_usSizeOfFonts);
    m_fontBold.SetFont("Fonts/Bold.ttf", g_usSizeOfFonts);
    m_fontExtendedBold.SetFont("Fonts/ExtendedBold.ttf", g_usSizeOfFonts);
    
    return true;
}

void IndicatorKMH::Draw(Shader &shaderTexture, Shader &shaderFont)
{
    //Indicator KMH
    gRenderer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    gRenderer.DrawPictureInAspectRatio(m_textureBackground, 940.0f, 400.0f, 0.0f, shaderTexture);
    gRenderer.DrawPictureInAspectRatio(m_textureMarker, 940.0f, 400.0f, 0.0f, shaderTexture);
    gRenderer.DrawPictureRotated(m_textureMarker, 940.0f, 400.0f, 0.0f, 16.5f, 0.0f,
                                 (float)m_textureBackground.GetWidth()/2,
                                 (float)m_textureBackground.GetHeight()/2, 0.0f, shaderTexture);
    
    //Text
    m_fontNormal.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
    m_fontNormal.DrawText("0", 1042.0f, 780.0f, 1.0f, shaderFont);
    m_fontNormal.DrawText("20", 1007.0f, 739.5f, 0.5f, shaderFont);
    m_fontNormal.DrawText("40", 989.0f, 692.5f, 0.8f, shaderFont);
    m_fontNormal.DrawText("60", 988.0f, 634.0f, 0.5f, shaderFont);
    m_fontNormal.DrawText("80", 993.5f, 587.0f, 0.8f, shaderFont);
    m_fontNormal.DrawText("100", 1024.0f, 532.0f, 0.5f, shaderFont);
    m_fontNormal.DrawText("120", 1051.0f, 508.5f, 0.8f, shaderFont);
    m_fontNormal.DrawText("140", 1108.5f, 476.0f, 0.5f, shaderFont);
    m_fontNormal.DrawText("160", 1152.5f, 473.5f, 0.8f, shaderFont);
    m_fontNormal.DrawText("180", 1221.5f, 478.0f, 0.5f, shaderFont);
    m_fontNormal.DrawText("200", 1244.0f, 510.5f, 0.8f, shaderFont);
    m_fontNormal.DrawText("220", 1296.0f, 536.5f, 0.5f, shaderFont);
    m_fontNormal.DrawText("240", 1299.5f, 580.5, 0.8f, shaderFont);
    m_fontNormal.DrawText("260", 1333.0f, 626.0f, 0.5f, shaderFont);
    m_fontNormal.DrawText("280", 1309.5f, 682.5f, 0.8f, shaderFont);
    m_fontNormal.DrawText("300", 1311.5f, 732.0f, 0.5f, shaderFont);
    m_fontNormal.DrawText("320", 1258.5f, 779.5f, 0.8f, shaderFont);
    //Needle
    gRenderer.DrawPictureRotated(m_textureNeedle, 940.0f, 400.0f, 0.0f, m_fAngle,
                                 -35.0f,
                                 (float)m_textureBackground.GetWidth() / 2,
                                 (float)m_textureBackground.GetHeight() / 2,
                                 0.0f, shaderTexture);
    
    // Menu
    m_fontBold.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
    m_fontBold.DrawText(std::to_string(m_nKMH), g_fDigitalKmhX, g_fDigitalKmhY, 1.5f, shaderFont);
    m_fontNormal.DrawText("km/h", 1150.5f, 675.0f, 0.5f, shaderFont);
}

void IndicatorKMH::StartTest()
{
    m_timerTest.Start(g_usTimerTestID, 15);
}

void IndicatorKMH::TickTest()
{
    const float fInitialKMH = 0.0f;
    const float fFinalKMH = 320.0f;
    m_fKMH = sineNeedle.easeIn(m_timerTest.GetTimes(), fInitialKMH, fFinalKMH/2.0f, 25);
    m_fAngle = m_fMinPosNeedle + m_fKMH * 0.85;
    OffsetDigitalKmh();
    
    if(m_fKMH <= fInitialKMH)
    {
        m_timerTest.Stop();
    }
}

void IndicatorKMH::SetKMH(float fKMH)
{
    m_fKMH = fKMH;
    if(m_fKMH < 0.0f)
    {
        m_fKMH = 0.0f;
    }
    m_nKMH = m_fKMH;
    OffsetDigitalKmh();
    m_fAngle = m_fMinPosNeedle + m_fKMH * 0.85;
}

void IndicatorKMH::OffsetDigitalKmh()
{
    if(m_nKMH < 10)
    {
        g_fDigitalKmhX = 1155.5f;
    }
    else if(m_nKMH < 100)
    {
        g_fDigitalKmhX = 1135.5f;
    }
    else
    {
        g_fDigitalKmhX = 1115.5f;
    }
}

void IndicatorKMH::Process()
{
    std::function<void()>callbackTest = [&]
    {
        TickTest();
    };
    
    m_timerTest.Tick(callbackTest);
}

void IndicatorKMH::HandleEvent(GLFWwindow *window)
{

}

