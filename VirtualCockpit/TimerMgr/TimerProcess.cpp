//
//  TimerProcess.cpp
//  VirtualCockpit
//
//  Created by Iliya Iliev on 10.02.20.
//  Copyright © 2020 Iliya Iliev. All rights reserved.
//

#include "TimerProcess.hpp"

TimerProcess::TimerProcess()
{
    gTimerProcess.m_vecTimerObjects.erase(gTimerProcess.m_vecTimerObjects.begin(),
                                          gTimerProcess.m_vecTimerObjects.end());
}

void TimerProcess::Process()
{
    for(auto& objects : gTimerProcess.m_vecTimerObjects)
    {
        objects.Tick(m_callback = [&]{});
    }
}

std::vector<TimerMgr>& TimerProcess::GetTimerObjects()
{
    return gTimerProcess.m_vecTimerObjects;
}

TimerProcess gTimerProcess;
