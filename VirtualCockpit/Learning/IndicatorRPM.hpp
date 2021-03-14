//
//  IndicatorRPM.hpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 25.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#ifndef Indicator_hpp
#define Indicator_hpp

#include <iostream>
#include <functional>
#include <string>
#include "../TimerMgr/TimerMgr.hpp"
#include "../Texture/Texture.hpp"
#include "../Font/Font.hpp"
#include "../Renderer/Renderer.hpp"
#include "../GLFW/shader.hpp"
#include "Easing/Sine.h"
#include "IndicatorKMH.hpp"

enum EStatesOfRPM
{
    eOff,
    eTest,
    eReadyForStart,
    eStarted,
    eNumberOfStates
};

enum EStatesOfGearbox
{
    eParking,
    eReverse,
    eNeutral,
    eDrive,
    eDynamic
};

enum EStatesOfAcceleration
{
    eReleased,
    eAccelerating,
    eDecelerating
};
class IndicatorRPM
{
    
private:
    
    IndicatorKMH indicatorKMHObject;
    
    Texture m_textureBackground;
    Texture m_textureMarker;
    Texture m_textureNeedle;
    Texture m_textureRsLogo;
    Font m_fontNormal;
    Font m_fontBold;
    Font m_fontExtendedBold;
    TimerMgr m_timerTest;
    TimerMgr m_timerStartEngine;
    TimerMgr m_timerStopEngine;
    Sine sineNeedle;
    
    EStatesOfRPM m_eStateOfEngine = eOff;
    EStatesOfGearbox m_eStateOfGearbox = eParking;
    EStatesOfAcceleration m_eStateOfAcceleration = eReleased;
    
    const float m_fMinPosNeedle = -135.0f;
    const float m_fMaxPosNeedle = 135.0f;
    const float m_fOffsetBetweenTicks = 33.75f;
    const float m_fMaxRPM = 8.0f;
    float m_fMinRPM = 0.850f;
    
    float m_fAngle = m_fMinPosNeedle;
    float m_fRedColorMarker = 0.0f;
    float m_fRPM = 0.0f;
    float m_fKMH = 0.0f;
    int m_nGearNumber = 1;
    
    bool bPressedNow = false;
    bool m_bTimeToChangeGear = false;
    bool m_bChangeMinRPM = false;
    
    std::string m_sGearIndicator = "P";
    std::string m_sGearBoxMode = "AUTO";
    
public:
    
    IndicatorRPM();
    IndicatorKMH& GetKMHObject() {return indicatorKMHObject;}
    EStatesOfRPM& GetStateOfEngine() {return m_eStateOfEngine;}
    EStatesOfGearbox& GetStateOfGearBox(){return m_eStateOfGearbox;}
    EStatesOfAcceleration& GetStateOfAcceleration(){return m_eStateOfAcceleration;}
    void SetStateOfEngine(EStatesOfRPM eState);
    float GetRPM() {return m_fRPM;}
    bool LoadResources();
    void Draw(Shader& shaderTexture, Shader& shaderFont);
    void StartTest();
    void StartEngine();
    void StopEngine();
    void IncrementRPM();
    void DecrementRPM();
    void ChangeGear(EStatesOfGearbox eStateToChange);
    
    void IncrementRPMInParking();
    void IncrementRPMInReverse();
    void IncrementRPMInNeutral();
    void IncrementRPMInDrive();
    
    void DecrementRPMInParking();
    void DecrementRPMInReverse();
    void DecrementRPMInNeutral();
    void DecrementRPMInDrive();
    
    void TickTest();
    void TickStartEngine();
    void TickStopEngine();
    TimerMgr& GetTimerTest() {return m_timerTest;}
    TimerMgr& GetTimerStartEngine() {return m_timerStartEngine;}
    
    void Process();
    void HandleEvent(GLFWwindow* window);
};

#endif /* Indicator_hpp */
