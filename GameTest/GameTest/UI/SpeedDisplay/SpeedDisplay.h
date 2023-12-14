#pragma once
#include "SpeedDisplayShader.h"
#include "Api/Public/GameObject/UIObject.h"
#include "Api/Public/GameObject/Basic/BasicSphere.h"
#include "Api/Public/Shader/Basic/SpriteShader.h"
#include "Core/Public/Core/MoteurWindows.h"
#include "GameTest/Components/SizeModifierComponent.h"

class SpeedDisplay final : public PM3D_API::UIObject
{
public:
    SpeedDisplay() : UIObject(
                         "Speed Display",
                         DirectX::XMFLOAT2(0.1f, 0.1f),
                         DirectX::XMFLOAT2(0.0f, 0.0f),
                         0.0f,
                         true,
                         true
                     ), matWVP(DirectX::XMMatrixIdentity())
    {
    }

    void Initialize() override;
    void Update() override;
    void DrawSelf() const override;

private:
    const float MAX_SIZE = SizeModifierComponent::MAX_SIZE;
    const float HALF_MAX_SIZE = MAX_SIZE / 2.f;

    SizeModifierComponent* sizeModifierComponent = nullptr;
    PM3D::CTexture* bgTexture = nullptr;
    PM3D::CTexture* frontTexture = nullptr;
    DirectX::XMMATRIX matWVP;
    std::unique_ptr<PM3D_API::SpriteShader> frontShader;
    std::unique_ptr<PM3D_API::SpeedDisplayShader> bgShader;

    XMFLOAT3 color = XMFLOAT3(1.f, 1.f, 1.f);
    float percent = 1.f;

    void DrawBg() const;
    void DrawFront() const;
};
