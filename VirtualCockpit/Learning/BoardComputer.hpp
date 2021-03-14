//
//  BoardComputer.hpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 9.03.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#ifndef BoardComputer_hpp
#define BoardComputer_hpp

#include <iostream>
#include <functional>
#include <string>
#include "../TimerMgr/TimerMgr.hpp"
#include "../Texture/Texture.hpp"
#include "../Font/Font.hpp"
#include "../Renderer/Renderer.hpp"
#include "../GLFW/shader.hpp"
#include "Easing/Sine.h"

enum EStatesOfBoardComputer
{
    eHide,
    eDateAndTime,
    eFuelConsumption,
    eLongTermMemory,
    eCarInfo
};

class BoardComputer
{
    
private:
    
    Texture m_textureLineSmall;
    Texture m_textureBattery;
    Font m_fontNormal;
    Font m_fontBold;
    Font m_fontExtendedBold;
    
    TimerMgr m_timerMovement;
    
    EStatesOfBoardComputer m_eStateOfBoardComputer = eHide;
    
    const float m_fMaxConsumption = 40.0f;
    float m_fInstantConsumpt = 0.0f;
    float m_fInstantConsumptMemory = 0.0f;
    unsigned int m_nCyclesConsumpt = 0;
    std::string m_sInstantConsumtion = "0";
    
    int m_nKM = 0;
    std::string m_sInstantKM = "0";
    std::string m_sKMAverage = "0 km/h";
    int m_nMeters = 0;
    int m_nMetersHundreds = 0;
    int m_nMetersInSeconds = 0;
    int m_nKMAverage = 0;
    unsigned int m_nSeconds = 1;
    
    int m_nWaterTemp = 0;
    int m_nOilTemp = 0;
    int m_nOilPressure = 0;
    float m_fBoost = 0.0f;
    const float m_fMaxBoost = 200.0f;
    const int m_nMaxOilTemp = 130;
    
    float fGlobalY = 0.0f;
    float m_fConsumtion = 0.0f;
    float m_fInitialPosLabel = 0.0f;
    float m_fDestPosLabel = 0.0f;
    
    bool bPressedNow = false;
    
public:
    BoardComputer();
    bool LoadResources();
    void Draw(Shader& shaderTexture, Shader& shaderFont, Shader& shaderLines, float fAlpha);
    void Process();
    void HandleEvent(GLFWwindow* window);
    
    void SetState(EStatesOfBoardComputer eState) { m_eStateOfBoardComputer = eState; }
    void DrawDateAndTime(float fX, float fY, Shader& shaderTexture, Shader& shaderFont, Shader& shaderLines, float fAlpha);
    void DrawFuelConsupmt(float fX, float fY, Shader& shaderTexture, Shader& shaderFont, Shader& shaderLines, float fAlpha);
    void DrawLongTerm(float fX, float fY, Shader& shaderTexture, Shader& shaderFont, Shader& shaderLines, float fAlpha);
    void DrawCarInfo(float fX, float fY, Shader& shaderTexture, Shader& shaderFont, Shader& shaderLines, float fAlpha);
    void DrawCursor(float fX, float fY, Shader& shaderTexture, Shader& shaderFont, Shader& shaderLines, float fAlpha);
    
    void SetConsmption(float fConsumption);
    void SetAfterStart(float fKMH);
    void SetCoolantTemp(int &nCoolantTemp);
};

#endif /* BoardComputer_hpp */
