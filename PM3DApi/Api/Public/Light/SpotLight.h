#pragma once
#include "Light.h"
#include "../../../../PetitMoteur3D/Core/Imgui/imgui.h"

namespace PM3D_API
{
    class SpotLight final : public Light
    {
    public:
        SpotLight(
            const std::string& name,
            const DirectX::XMFLOAT3& position,
            const DirectX::XMFLOAT3& rotation,
            const DirectX::XMFLOAT3& color
        ) : Light(
            name,
            position,
            rotation,
            1.0f,
            color
        )
        {
        }

        SpotLight(
            const DirectX::XMFLOAT3& position,
            const DirectX::XMFLOAT3& rotation,
            const DirectX::XMFLOAT3& color
        ) : Light(
            position,
            rotation,
            1.0f,
            color
        )
        {
        }

        SpotLight(
            const std::string& name,
            const DirectX::XMFLOAT3& position,
            const DirectX::XMFLOAT3& rotation,
            const DirectX::XMFLOAT3& color,
            const float intensity
        ) : Light(
            name,
            position,
            rotation,
            intensity,
            color
        )
        {
        }

        SpotLight(
            const DirectX::XMFLOAT3& position,
            const DirectX::XMFLOAT3& rotation,
            const DirectX::XMFLOAT3& color,
            const float intensity
        ) : Light(
            position,
            rotation,
            intensity,
            color
        )
        {
        }

        SpotLight(
            const std::string& name,
            const DirectX::XMFLOAT3& position,
            const DirectX::XMFLOAT3& rotation,
            const DirectX::XMFLOAT3& color,
            const float intensity,
            const float innerAngle,
            const float outerAngle
        ) : Light(
            name,
            position,
            rotation,
            intensity,
            color
        ), innerAngle(innerAngle), outerAngle(outerAngle)
        {
        }

        SpotLight(
            const DirectX::XMFLOAT3& position,
            const DirectX::XMFLOAT3& rotation,
            const DirectX::XMFLOAT3& color,
            const float intensity,
            const float innerAngle,
            const float outerAngle
        ) : Light(
            position,
            rotation,
            intensity,
            color
        ), innerAngle(innerAngle), outerAngle(outerAngle)
        {
        }

        ShaderLightDefaultParameters GetShaderLightDefaultParameters() const override;

        LightType GetType() const override { return LightType::SPOT; }

        void DrawDebugInfo() const override
        {
            ImGui::Text("Type");
            ImGui::SameLine(100.0); ImGui::Text("Point");

            ImGui::Text("Position");
            ImGui::SameLine(100.0); ImGui::Text("x=%f", worldPosition.x);
            ImGui::SameLine(200.0); ImGui::Text("y=%f", worldPosition.y);
            ImGui::SameLine(300.0); ImGui::Text("z=%f", worldPosition.z);

            ImGui::Text("Direction");
            ImGui::SameLine(100.0); ImGui::Text("x=%f", worldRotationEuler.x);
            ImGui::SameLine(200.0); ImGui::Text("y=%f", worldRotationEuler.y);
            ImGui::SameLine(300.0); ImGui::Text("z=%f", worldRotationEuler.z);

            ImGui::Text("InnerAngle");
            ImGui::SameLine(100.0); ImGui::Text("%f", innerAngle);

            ImGui::Text("OuterAngle");
            ImGui::SameLine(100.0); ImGui::Text("%f", outerAngle);

            ImGui::Text("Intensity");
            ImGui::SameLine(100.0); ImGui::Text("%f", intensity);

            ImGui::Text("Color");
            ImGui::SameLine(100.0); ImGui::Text("r=%f", color.x);
            ImGui::SameLine(200.0); ImGui::Text("g=%f", color.y);
            ImGui::SameLine(300.0); ImGui::Text("b=%f", color.z);
        }

    protected:
        float innerAngle = 0.0f;
        float outerAngle = 0.0f;
    };
}
