#pragma once
#include "Light.h"
#include "Core/Imgui/imgui.h"

namespace PM3D_API
{
    class AmbiantLight : public Light
    {
    public:
        AmbiantLight(
            float intensity,
            const DirectX::XMFLOAT3& color
        ) : Light(
            DirectX::XMFLOAT3(0, 0, 0),
            DirectX::XMFLOAT3(0, 0, 0),
            intensity,
            color
        )
        {
        }

        ShaderLightDefaultParameters GetShaderLightDefaultParameters(GameObject* gameObject) const override;

        LightType GetType() const override { return LightType::AMBIANT; }

        void DrawDebugInfo() const override
        {
            ImGui::Text("Type");
            ImGui::SameLine(100.0); ImGui::Text("Ambiant");

            ImGui::Text("Intensity");
            ImGui::SameLine(100.0); ImGui::Text("%f", intensity);

            ImGui::Text("Color");
            ImGui::SameLine(100.0); ImGui::Text("r=%f", color.x);
            ImGui::SameLine(200.0); ImGui::Text("g=%f", color.y);
            ImGui::SameLine(300.0); ImGui::Text("b=%f", color.z);
        }
    };
}
