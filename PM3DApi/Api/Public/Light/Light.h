#pragma once

#include "LightType.h"
#include "ShaderLightDefaultParameters.h"
#include "../GameObject/GameObject.h"

namespace PM3D_API
{
	class Light : public PM3D_API::GameObject
	{
	public:
		Light(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);
		Light(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, float intensity);
		Light(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, float intensity, DirectX::XMFLOAT3 color);

		Light(const std::string& name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);
		Light(const std::string& name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, float intensity);
		Light(const std::string& name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, float intensity, DirectX::XMFLOAT3 color);

		void SetLocalPosition(DirectX::XMFLOAT3 newPosition) override;
		void SetWorldPosition(DirectX::XMFLOAT3 newPosition) override;
		void SetLocalRotation(Quaternion newRotation) override;
		void SetWorldRotation(Quaternion newRotation) override;
		void SetLocalRotation(DirectX::XMFLOAT3 newRotation) override;
		void SetWorldRotation(DirectX::XMFLOAT3 newRotation) override;

		float GetIntensity() const { return intensity; }
		void SetIntensity(const float newIntensity) { intensity = newIntensity; }

		DirectX::XMFLOAT3 GetColor() const { return color; }
		void SetColor(const DirectX::XMFLOAT3 newColor) { color = newColor; }

		virtual ShaderLightDefaultParameters GetShaderLightDefaultParameters() const = 0;

		virtual LightType GetType() const = 0;

		void DrawDebugInfo() const override {};
	protected:
		float intensity;
		DirectX::XMFLOAT3 color;
	};
}