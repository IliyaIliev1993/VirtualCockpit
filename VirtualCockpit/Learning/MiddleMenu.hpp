//
//  MiddleMenu.hpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 1.03.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#ifndef MiddleMenu_hpp
#define MiddleMenu_hpp

#include <iostream>
#include <functional>
#include <string>
#include "../TimerMgr/TimerMgr.hpp"
#include "../Texture/Texture.hpp"
#include "../Font/Font.hpp"
#include "../Renderer/Renderer.hpp"
#include "../GLFW/shader.hpp"

#include "IndicatorRPM.hpp"
#include "BoardComputer.hpp"
enum EStatesOfMiddleMenu
{
    eKeyOut,
    eKeyIn
};

class MiddleMenu
{
    
private:
    
    IndicatorRPM indicatorRPMObject;
    BoardComputer bcObject;
    
    Texture m_textureLogo;
    Texture m_textureLine;
    Texture m_textureLineSmall;
    Texture m_textureSquare;
    Texture m_textureLogoPetrol;
    Texture m_textureLogoWater;
    Font m_fontNormal;
    Font m_fontBold;
    Font m_fontExtendedBold;
    TimerMgr m_timerStart;
    TimerMgr m_timerStop;
    TimerMgr m_timerDisappearLogo;
    TimerMgr m_timerGlobal;
    Sine sineKeyIn;
    
    EStatesOfMiddleMenu m_eState = eKeyOut;
    float m_fAlphaStart = 0.0f;
    float m_fAlphaSquare = 0.0f;
    bool bPressedNow = false;
    
    unsigned int m_unMaxLevelPetrol = 12;
    unsigned int m_unMaxLevelWater = 6;
    int m_nCurrentWaterLevel = 0;
    
    float fConsumptRatio = 0.0f;
    float fConsumption = 0.0f;
    
public:
    
    MiddleMenu();
    IndicatorRPM& GetIndicatorRPMObject() {return indicatorRPMObject;}
    BoardComputer& GetBoardComputerObject() {return bcObject;}
    EStatesOfMiddleMenu& GetState() {return m_eState;}
    
    bool LoadResources();
    void Draw(Shader& shaderTexture, Shader& shaderFont, Shader& shaderLines);
    void Process();
    void HandleEvent(GLFWwindow* window);
    
    void KeyIn();
    void KeyOut();
    void DrawClock(float fX, float fY, Shader& shaderFont);
    void DrawTemperature(float fX, float fY, Shader& shaderFont);
    
};

#endif /* MiddleMenu_hpp */
