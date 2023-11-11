#include "TestUIObject.h"

#include "Core/Public/Util/Time.h"
#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"

void TestUIObject::Initialize()
{
    auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"DebugTexture.dds");
    const auto spriteRendererPtr = spriteRenderer.get();
    AddComponent(std::move(spriteRenderer));
    spriteRendererPtr->Initialize();
}

void TestUIObject::Update()
{
    UIObject::Update();
    
    const auto amount = 1.f * PM3D::Time::GetInstance().GetUpdateDeltaTime();

    auto rotVal = GetRotation();
    rotVal += amount;
    if (rotVal > DirectX::XM_2PI)
        rotVal -= DirectX::XM_2PI;
    
    SetCentralRotation(rotVal);
}
