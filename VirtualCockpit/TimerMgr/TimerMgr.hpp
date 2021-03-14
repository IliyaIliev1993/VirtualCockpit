//
//  TimerMgr.hpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 10.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#ifndef TimerMgr_hpp
#define TimerMgr_hpp

#include <stdio.h>
#include <iostream>
#include <functional>
#include <ctime>
#include "glad.h"
#include "glfw3.h"


class TimerMgr
{
private:
    
    double m_Started;
    double m_Elapsed;
    
    enum class StateOfTimer
    {
        eStopped,
        eStarted,
        eNumberOfStates
    };
    
    StateOfTimer m_eState = StateOfTimer::eStopped;
    double GetTicks();
    
    unsigned int m_nID;
    unsigned int m_nPeriod;
    unsigned int m_nTimes;
    
    
public:
    
    TimerMgr();
    void Start(unsigned int nID, unsigned int nPeriod);
    void Stop();
    bool IsStarted();
    bool IsStopped();
    unsigned int& GetTimes();
    void Tick(std::function<void()>callback);
    
};

#endif /* TimerMgr_hpp */
