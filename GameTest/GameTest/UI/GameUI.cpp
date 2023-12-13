#include "GameUI.h"

#include "TimerDisplay.h"
#include "Api/Public/UI/Button.h"
#include "GameTest/Components/MainScene/PauseComponent.h"
#include "SpeedDisplay/SpeedDisplay.h"

void GameUI::Initialize()
{
    constexpr DirectX::XMFLOAT2 expectedScreenSize = {1920.f, 1080.f};

    auto pauseComponent = std::make_unique<PauseComponent>();
    pauseComponentPtr = pauseComponent.get();
    canvas->AddChild(std::move(pauseComponent));
    pauseComponentPtr->Initialize();

    auto pauseButton = std::make_unique<Button>(
        L"sprite/ui/small_btn_base.dds",
        L"sprite/ui/small_btn_hover.dds",
        L"sprite/ui/small_btn_pressed.dds",
        48.f / expectedScreenSize.x,
        913.f / expectedScreenSize.y,
        120.f / expectedScreenSize.x,
        120.f / expectedScreenSize.y,
        [this]
        {
            pauseComponentPtr->TogglePause();
        }
    );
    const auto pauseButtonPtr = pauseButton.get();
    canvas->AddChild(std::move(pauseButton));
    pauseButtonPtr->Initialize();

    auto speedDisplay = std::make_unique<SpeedDisplay>();
    const auto speedDisplayPtr = speedDisplay.get();
    canvas->AddChild(std::move(speedDisplay));
    speedDisplayPtr->Initialize();

    auto timerDisplay = std::make_unique<TimerDisplay>();
    const auto timerDisplayPtr = timerDisplay.get();
    canvas->AddChild(std::move(timerDisplay));
    timerDisplayPtr->Initialize();
}

void GameUI::Update()
{
    // TODO
}

