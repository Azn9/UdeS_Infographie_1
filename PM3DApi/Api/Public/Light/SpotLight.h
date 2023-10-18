#pragma once
#include "Light.h"

class SpotLight final : public Light
{
public:
	ShaderLightDefaultParameters GetShaderLightDefaultParameters() const override;
protected:

private:
	
};
