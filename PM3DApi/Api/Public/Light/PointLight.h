#pragma once
#include "Light.h"
#include "Core/Imgui/imgui.h"

namespace PM3D_API
{
    class PointLight final : public Light
    {
    public:
        PointLight(
            const std::string& name,
            const DirectX::XMFLOAT3& position,
            const DirectX::XMFLOAT3& color
        ) : Light(
            name,
            position,
            DirectX::XMFLOAT3(0, 0, 0),
            1.0f,
            color
        )
        {
        }

        PointLight(
            const DirectX::XMFLOAT3& position,
            const DirectX::XMFLOAT3& color
        ) : Light(
            position,
            DirectX::XMFLOAT3(0, 0, 0),
            1.0f,
            color
        )
        {
        }

        ShaderLightDefaultParameters GetShaderLightDefaultParameters(GameObject* gameObject) const override;
        ShaderLightDefaultParameters GetShaderLightDefaultParameters(DirectX::XMMATRIX wvp) const override;

        LightType GetType() const override { return LightType::POINT; }

        void DrawDebugInfo() const override
        {
            ImGui::Text("Type");
            ImGui::SameLine(100.0);
            ImGui::Text("Point");

            ImGui::Text("Position");
            ImGui::SameLine(100.0);
            ImGui::Text("x=%f", worldPosition.x);
            ImGui::SameLine(200.0);
            ImGui::Text("y=%f", worldPosition.y);
            ImGui::SameLine(300.0);
            ImGui::Text("z=%f", worldPosition.z);

            ImGui::Text("Intensity");
            ImGui::SameLine(100.0);
            ImGui::Text("%f", intensity);

            ImGui::Text("Color");
            ImGui::SameLine(100.0);
            ImGui::Text("r=%f", color.x);
            ImGui::SameLine(200.0);
            ImGui::Text("g=%f", color.y);
            ImGui::SameLine(300.0);
            ImGui::Text("b=%f", color.z);
        }
    };
}
