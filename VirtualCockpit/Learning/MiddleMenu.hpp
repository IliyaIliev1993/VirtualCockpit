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
#include "TimerMgr.hpp"
#include "Texture.hpp"
#include "Font.hpp"
#include "Renderer.hpp"
#include "shader.hpp"
#include "Sine.h"

#include "IndicatorRPM.hpp"
enum EStatesOfMiddleMenu
{
    eKeyOut,
    eKeyIn
};

class MiddleMenu
{
    
private:
    
    IndicatorRPM indicatorRPMObject;
    
    Texture m_textureLogo;
    Texture m_textureLine;
    Texture m_textureSquare;
    
    TimerMgr m_timerStart;
    TimerMgr m_timerStop;
    TimerMgr m_timerDisappearLogo;
    Sine sineKeyIn;
    
    EStatesOfMiddleMenu m_eState = eKeyOut;
    float m_fAlphaStart = 0.0f;
    float m_fAlphaSquare = 0.0f;
    bool bPressedNow = false;
    
public:
    
    MiddleMenu();
    IndicatorRPM& GetIndicatorRPMObject() {return indicatorRPMObject;}
    EStatesOfMiddleMenu& GetState() {return m_eState;}
    bool LoadResources();
    void Draw(Shader& shaderTexture, Shader& shaderFont);
    void Process();
    void HandleEvent(GLFWwindow* window);
    
    void KeyIn();
    void KeyOut();
    
};

#endif /* MiddleMenu_hpp */
