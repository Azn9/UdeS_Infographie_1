#include "TestUIObject.h"

#include "../../../PetitMoteur3D/Core/Public/Util/Time.h"
#include "../../../PM3DApi/Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"

void TestUIObject::Initialize()
{
    std::cout << "TestUIObject::Initialize()" << std::endl;
    std::cout << "Screen scale: " << scale.x << ", " << scale.y << std::endl;
    std::cout << "Top left position: " << position.x << ", " << position.y << std::endl;
    
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
