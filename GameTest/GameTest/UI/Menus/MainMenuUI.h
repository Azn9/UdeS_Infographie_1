#pragma once
#include "Api/Public/GameObject/UIObject.h"
#include "Api/Public/UI/Button.h"
#include "GameTest/Components/LoadingScene/LoadingSceneComponent.h"

class MainMenuUI final : public PM3D_API::UIObject
{
public:
    MainMenuUI() : UIObject(
        "MainMenuUI",
        DirectX::XMFLOAT2(1.0f, 1.0f),
        DirectX::XMFLOAT2(0.0f, 0.0f),
        0.0f,
        true,
        true
    )
    {
    }

    void Initialize() override;
    void Draw() override;
    void DrawSelf() const override;
    void LoadComplete();

    void SetDisplayed(const bool displayed) { this->displayed = displayed; }

private:
    bool displayed = true;
    bool creditsDisplayed = false;
    bool paywallDisplayed = false;

    std::unique_ptr<Button>::pointer disabledPlayButtonPtr;
    std::unique_ptr<Button>::pointer playButtonPtr;
    std::unique_ptr<LoadingSceneComponent>::pointer loadingSceneComponentPtr;
    std::unique_ptr<UIObject>::pointer creditsObjectPtr;
    std::unique_ptr<UIObject>::pointer paywallObjectPtr;
    std::vector<UIObject*> allComponents;
    bool loaded = false;
};
