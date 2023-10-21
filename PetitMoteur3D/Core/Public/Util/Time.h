#pragma once
#include "../../Public/Core/MoteurWindows.h"

class Time
{
public:
    static uint64_t GetTimeSpecific()
    {
        return PM3D::CMoteurWindows::GetInstance().GetTimeSpecific();
    }

    static double GetTimeIntervalsInSec(const uint64_t start, const uint64_t stop)
    {
        return PM3D::CMoteurWindows::GetInstance().GetTimeIntervalsInSec(start, stop);
    }
};
