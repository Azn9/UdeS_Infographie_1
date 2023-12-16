#pragma once

#include "Singleton.h"
#include "Core/Public/Core/Horloge.h"

namespace PM3D
{
    class Time : public CSingleton<Time>
    {
    public:
        float GetUpdateDeltaTime() const
        {
            return UpdateDeltaTime;
        }

        float GetPhysicsDeltaTime() const
        {
            return PhysicsDeltaTime;
        }

        int64_t GetTimeSpecific() const
        {
            return clock.GetTimeCount();
        }

        int64_t GetCurrentTimeMs() const
        {
            return static_cast<int64_t>(clock.GetTimeCount() * (clock.GetSecPerCount() * 1000.0));
        }

        double GetTimeIntervalsInMs(const int64_t start, const int64_t stop) const
        {
            return clock.GetTimeBetweenCounts(start, stop) * 1000.0;
        }

        double GetTimeIntervalsInSec(const int64_t start, const int64_t stop) const
        {
            return clock.GetTimeBetweenCounts(start, stop);
        }

        float GetTimeScale() const
        {
            return TimeScale;
        }

        void SetTimeScale(const float timeScale)
        {
            TimeScale = timeScale;
        }

        void SetUpdateDeltaTime(const float deltaTime)
        {
            UpdateDeltaTime = deltaTime;
        }

        void SetPhysicsDeltaTime(const float deltaTime)
        {
            PhysicsDeltaTime = deltaTime;
        }

        float* GetTimeScalePtr()
        {
            return &TimeScale;
        }

        void SetUpdateDeltaTimeA(const float deltaTime)
        {
            UpdateDeltaTimeA = deltaTime;
        }

        void SetPhysicsDeltaTimeA(const float deltaTime)
        {
            PhysicsDeltaTimeA = deltaTime;
        }

        float GetUpdateDeltaTimeA() const
        {
            return UpdateDeltaTimeA;
        }

        float GetPhysicsDeltaTimeA() const
        {
            return PhysicsDeltaTimeA;
        }

    private:
        Horloge clock;

        float UpdateDeltaTime = 0.0;
        float PhysicsDeltaTime = 0.0;
        float UpdateDeltaTimeA = 0.0;
        float PhysicsDeltaTimeA = 0.0;
        float TimeScale = 1.0;
    };
}
