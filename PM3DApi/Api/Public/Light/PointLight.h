#pragma once
#include "Light.h"

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

	ShaderLightDefaultParameters GetShaderLightDefaultParameters() const override;

	LightType GetType() const override { return LightType::POINT; }

protected:

private:

};
