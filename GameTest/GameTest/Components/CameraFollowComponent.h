#pragma once
#include <DirectXMath.h>
#include <string>

#include "Api/Public/Component/Component.h"
#include "Core/Imgui/imgui.h"

enum class CameraType
{
    ThirdPerson = 0,
    FirstPerson = 1,
    FirstPersonWTF = 2
};

class CameraFollowComponent final : public PM3D_API::Component
{
public:
    CameraFollowComponent();

    void PhysicsUpdate() override;

    void DrawDebugInfo() const override
    {
        ImGui::Text("Distance");
        ImGui::SameLine(100);
        ImGui::Text(std::to_string(_distance).c_str());

        ImGui::Text("Angle");
        ImGui::SameLine(100);
        ImGui::Text(std::to_string(_angle).c_str());
    }

    void SetObjectToFollow(PM3D_API::GameObject* objectToFollow)
    {
        _objectToFollow = objectToFollow;
    }

private:
    float _distance = 5.0f;
    float _angle = 45.0f;
    PM3D_API::GameObject* _objectToFollow{};
    CameraType camType = CameraType::ThirdPerson;
    bool _inTunnel = false;
    bool _resetRequested = false;
    DirectX::XMFLOAT3 _currentLookAt{};
};
