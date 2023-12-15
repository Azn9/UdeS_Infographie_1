#include "CameraMoverComponent.h"

#include "Api/Public/Camera/Camera.h"
#include "Core/Public/Util/Time.h"
#include "GameTest/GameTest.h"

void CameraMoverComponent::Update()
{
    angle += 20.0f * PM3D::Time::GetInstance().GetUpdateDeltaTime();
    if (angle >= 360)
        angle = 0;

    const auto position = parentObject->GetWorldPosition();
    const auto newPosition = DirectX::XMFLOAT3(
        cosf(angle * DirectX::XM_PI / 180.0f) * distance,
        position.y,
        sinf(angle * DirectX::XM_PI / 180.0f) * distance
    );

    parentObject->SetWorldPosition(newPosition);
    static_cast<PM3D_API::Camera*>(parentObject)->SetFocusPoint(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
}

void CameraMoverComponent::DrawDebugInfo() const
{
    ImGui::Text("Distance");
    ImGui::SameLine(100);
    ImGui::Text(std::to_string(distance).c_str());

    ImGui::Text("Angle");
    ImGui::SameLine(100);
    ImGui::Text(std::to_string(angle).c_str());
}
