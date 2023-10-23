#pragma once
#include "../Core/Horloge.h"

namespace Time
{
static double DeltaTime = 0.0;
static double PhysicsDeltaTime = 0.0;
static PM3D::Horloge clock;

inline int64_t GetTimeSpecific()
{
	return clock.GetTimeCount();
}

inline int64_t GetCurrentTimeMs()
{
	return clock.GetTimeCount() * static_cast<int64_t>(clock.GetSecPerCount() * 1000.0);
}

inline double GetTimeIntervalsInMs(const int64_t start, const int64_t stop)
{
	return clock.GetTimeBetweenCounts(start, stop) * 1000.0;
}

inline double GetTimeIntervalsInSec(const int64_t start, const int64_t stop)
{
	return clock.GetTimeBetweenCounts(start, stop);
};
}
