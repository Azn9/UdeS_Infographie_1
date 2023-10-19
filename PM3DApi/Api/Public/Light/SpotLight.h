#pragma once
#include "Light.h"

class SpotLight final : public Light
{
public:
	ShaderLightDefaultParameters GetShaderLightDefaultParameters() const override;

	LightType GetType() const override { return LightType::SPOT; }
protected:

private:
	
};
