#include "TestUIObject.h"

#include "../../../PM3DApi/Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"

void TestUIObject::Initialize()
{
    std::cout << "TestUIObject::Initialize()" << std::endl;
    std::cout << "Relative scale: " << relativeScale.x << ", " << relativeScale.y << std::endl;
    std::cout << "Screen scale: " << screenScale.x << ", " << screenScale.y << std::endl;
    std::cout << "Relative position: " << relativePosition.x << ", " << relativePosition.y << std::endl;
    std::cout << "Screen position: " << screenPosition.x << ", " << screenPosition.y << std::endl;
    std::cout << "Top left position: " << topLeftPosition.x << ", " << topLeftPosition.y << std::endl;
    
    auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"DebugTexture.dds");
    const auto spriteRendererPtr = spriteRenderer.get();
    AddComponent(std::move(spriteRenderer));
    spriteRendererPtr->Initialize();
}
