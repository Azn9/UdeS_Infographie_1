#include "TimerDisplay.h"

#include "Api/Public/Component/Basic/Render/2D/TextRenderer.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/InTunnelEvent.h"
#include "Core/Public/Util/Time.h"
#include "GameTest/RestartEvent.h"
#include "GameTest/Event/GameOverEvent.h"
#include "GameTest/Event/GameStartEvent.h"
#include "GameTest/Event/PauseEvent.h"

void TimerDisplay::Initialize()
{
    PM3D_API::EventSystem::Subscribe([this](const InTunnelEvent&)
    {
        if(isInTunnel)
            return;
        isRunning = false;
        endTime = PM3D::Time::GetInstance().GetTimeSpecific();
        isInTunnel = true;
    });

    PM3D_API::EventSystem::GetInstance().Subscribe([&](const GameStartEvent&)
    {
        std::cout << "GameStartEvent" << std::endl;
        startTime = PM3D::Time::GetInstance().GetTimeSpecific();
        isRunning = true;
        alpha = 1.f;
    });

    PM3D_API::EventSystem::GetInstance().Subscribe([&](const RestartEvent&)
    {
        std::cout << "RestartEvent" << std::endl;
        startTime = PM3D::Time::GetInstance().GetTimeSpecific();
        isRunning = true;
        isInTunnel = false;
        alpha = 1.f;
    });

    PM3D_API::EventSystem::GetInstance().Subscribe([&](const GameOverEvent&)
    {
        std::cout << "GameOverEvent" << std::endl;
        endTime = PM3D::Time::GetInstance().GetTimeSpecific();
        isRunning = false;
        //alpha = 0.f;
    });

    PM3D_API::EventSystem::GetInstance().Subscribe([&](const PauseEvent& event)
    {
        if (event.paused)
        {
            isRunning = false;
            //alpha = 0.f;
        }
        else
        {
            isRunning = !isInTunnel;
            alpha = 1.f;
        }
    });

    auto fontLoader = PM3D_API::FontLoader(
        "sprite/font/arial_black.fnt",
        L"sprite/font/arial_black.dds"
    );
    auto textDisplay = std::make_unique<PM3D_API::TextRenderer>(
        fontLoader,
        "00:00:000",
        DirectX::XMFLOAT2(0.f, 0.f),
        DirectX::XMFLOAT3(1.f, 1.f, 1.f),
        0.2f
    );
    textDisplayPtr = textDisplay.get();
    AddComponent(std::move(textDisplay));
    textDisplayPtr->Initialize();
}

void TimerDisplay::Update()
{
    if (isRunning)
    {
        endTime = PM3D::Time::GetInstance().GetTimeSpecific();
    }

    const uint64_t diff = PM3D::Time::GetInstance().GetTimeIntervalsInMs(startTime, endTime);

    const uint64_t ms = diff % 1000;
    const uint64_t s = (diff / 1000) % 60;
    const uint64_t m = diff / 1000 / 60;

    std::string msStr;
    if (ms < 10)
    {
        msStr = "00" + std::to_string(ms);
    }
    else if (ms < 100)
    {
        msStr = "0" + std::to_string(ms);
    }
    else
    {
        msStr = std::to_string(ms);
    }

    std::string sStr;
    if (s < 10)
    {
        sStr = "0" + std::to_string(s);
    }
    else
    {
        sStr = std::to_string(s);
    }

    std::string mStr;
    if (m < 10)
    {
        mStr = "0" + std::to_string(m);
    }
    else
    {
        mStr = std::to_string(m);
    }

    const auto text = mStr + ":" + sStr + ":" + msStr;

    textDisplayPtr->SetText(text);
}
