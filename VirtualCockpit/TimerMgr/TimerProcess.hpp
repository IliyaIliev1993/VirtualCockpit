//
//  TimerProcess.hpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 10.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#ifndef TimerProcess_hpp
#define TimerProcess_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "TimerMgr.hpp"

class TimerProcess
{
private:
    
    std::vector<TimerMgr>m_vecTimerObjects;
    std::function<void()>m_callback;
    
public:
    
    TimerProcess();
    std::vector<TimerMgr>& GetTimerObjects();
    void Process();
};
extern TimerProcess gTimerProcess;
#endif /* TimerProcess_hpp */
