#pragma once
#include "Light.h"

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

	ShaderLightDefaultParameters GetShaderLightDefaultParameters() const override;

	LightType GetType() const override { return LightType::AMBIANT; }

};
