#include "TournerComponent.h"

#include "Api/Public/GameObject/GameObject.h"

void TournerComponent::Initialize()
{
    originalPosition = parentObject->GetWorldPosition();
}

void TournerComponent::Update()
{
    angle++;
    if (angle > 360)
        angle = 0;

    const auto newPosition = DirectX::XMFLOAT3(
        originalPosition.x + 10 * sinf(static_cast<float>(angle) * DirectX::XM_PI / 180.f),
        originalPosition.y,
        originalPosition.z + 10 * cosf(static_cast<float>(angle) * DirectX::XM_PI / 180.f)
    );
    parentObject->SetWorldPosition(newPosition);
}
