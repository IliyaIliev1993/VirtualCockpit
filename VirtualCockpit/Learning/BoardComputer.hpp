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
#include "TimerMgr.hpp"
#include "Texture.hpp"
#include "Font.hpp"
#include "Renderer.hpp"
#include "shader.hpp"
#include "Sine.h"

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
};

#endif /* BoardComputer_hpp */
