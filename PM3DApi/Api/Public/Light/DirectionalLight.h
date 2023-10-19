#pragma once
#include "Light.h"

class DirectionalLight final : public Light
{
public:
	explicit DirectionalLight(DirectX::XMFLOAT3 rotation);
	DirectionalLight(DirectX::XMFLOAT3 rotation, float intensity);
	DirectionalLight(DirectX::XMFLOAT3 rotation, float intensity, DirectX::XMFLOAT3 color);

	DirectionalLight(const std::string& name, DirectX::XMFLOAT3 rotation);
	DirectionalLight(const std::string& name, DirectX::XMFLOAT3 rotation, float intensity);
	DirectionalLight(const std::string& name, DirectX::XMFLOAT3 rotation, float intensity, DirectX::XMFLOAT3 color);

	ShaderLightDefaultParameters GetShaderLightDefaultParameters() const override;

	LightType GetType() const override { return LightType::DIRECTIONAL; }
protected:

private:

};
