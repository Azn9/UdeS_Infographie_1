#include "GameUI.h"

#include "Api/Public/UI/Button.h"
#include "SpeedDisplay/SpeedDisplay.h"

void GameUI::Initialize()
{
    constexpr DirectX::XMFLOAT2 expectedScreenSize = {1920.f, 1080.f};

    auto pauseButton = std::make_unique<Button>(
        L"sprite/ui/small_btn_base.dds",
        L"sprite/ui/small_btn_hover.dds",
        L"sprite/ui/small_btn_pressed.dds",
        48.f / expectedScreenSize.x,
        913.f / expectedScreenSize.y,
        120.f / expectedScreenSize.x,
        120.f / expectedScreenSize.y,
        []
        {
            // TODO
        }
    );
    const auto quitButtonPtr = pauseButton.get();
    canvas->AddChild(std::move(pauseButton));
    quitButtonPtr->Initialize();

    auto speedDisplay = std::make_unique<SpeedDisplay>();
    const auto speedDisplayPtr = speedDisplay.get();
    canvas->AddChild(std::move(speedDisplay));
    speedDisplayPtr->Initialize();
}

void GameUI::Update()
{
    // TODO
}

