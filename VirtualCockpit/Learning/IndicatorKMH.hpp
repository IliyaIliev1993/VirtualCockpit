//
//  IndicatorKMH.hpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 27.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#ifndef IndicatorKMH_hpp
#define IndicatorKMH_hpp

#include <iostream>
#include "TimerMgr.hpp"
#include "Texture.hpp"
#include "Font.hpp"
#include "Renderer.hpp"
#include "shader.hpp"
#include "Sine.h"

class IndicatorKMH
{
    
    
private:
    
    Texture m_textureBackground;
    Texture m_textureMarker;
    Texture m_textureNeedle;
    Font m_fontNormal;
    Font m_fontBold;
    Font m_fontExtendedBold;
    TimerMgr m_timerTest;
    Sine sineNeedle;
    
    const float m_fMinPosNeedle = -135.0f;
    const float m_fMaxPosNeedle = 135.0f;
    const float m_fOffsetBetweenTicks = 16.875f;
    
    float m_fAngle = m_fMinPosNeedle;
    float m_fKMH = 0.0f;
    int m_nKMH = 0;
    
public:
    IndicatorKMH();
    void SetKMH(float fKMH);
    float GetKMH() {return m_fKMH;}
    float GetAngle() {return m_fAngle;}
    bool LoadResources();
    void Draw(Shader& shaderTexture, Shader& shaderFont);
    void StartTest();
    void TickTest();
    void OffsetDigitalKmh();
    TimerMgr& GetTimerTest() {return m_timerTest;}
    
    void Process();
    void HandleEvent(GLFWwindow* window);
    
};

#endif /* IndicatorKMH_hpp */
