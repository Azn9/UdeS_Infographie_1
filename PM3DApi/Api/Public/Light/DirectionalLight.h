#pragma once
#include "Light.h"
#include "../../../../PetitMoteur3D/Core/Imgui/imgui.h"

namespace PM3D_API
{
	class DirectionalLight final : public Light
	{
	public:
		explicit DirectionalLight(DirectX::XMFLOAT3 rotation);
		DirectionalLight(DirectX::XMFLOAT3 rotation, float intensity);
		DirectionalLight(DirectX::XMFLOAT3 rotation, float intensity, DirectX::XMFLOAT3 color);

		DirectionalLight(const std::string& name, DirectX::XMFLOAT3 rotation);
		DirectionalLight(const std::string& name, DirectX::XMFLOAT3 rotation, float intensity);
		DirectionalLight(const std::string& name, DirectX::XMFLOAT3 rotation, float intensity, DirectX::XMFLOAT3 color);

		ShaderLightDefaultParameters GetShaderLightDefaultParameters(GameObject* gameObject) const override;

		LightType GetType() const override { return LightType::DIRECTIONAL; }

		void DrawDebugInfo() const override
		{
			ImGui::Text("Type");
			ImGui::SameLine(100.0); ImGui::Text("Directional");

			ImGui::Text("Direction");
			ImGui::SameLine(100.0); ImGui::Text("x=%f", worldRotationEuler.x);
			ImGui::SameLine(200.0); ImGui::Text("y=%f", worldRotationEuler.y);
			ImGui::SameLine(300.0); ImGui::Text("z=%f", worldRotationEuler.z);

			ImGui::Text("Intensity");
			ImGui::SameLine(100.0); ImGui::Text("%f", intensity);

			ImGui::Text("Color");
			ImGui::SameLine(100.0); ImGui::Text("r=%f", color.x);
			ImGui::SameLine(200.0); ImGui::Text("g=%f", color.y);
			ImGui::SameLine(300.0); ImGui::Text("b=%f", color.z);
		}
	};
}
