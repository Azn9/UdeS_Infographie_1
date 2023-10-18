#pragma once
#include "Light.h"

class PointLight final : public Light
{
public:
	ShaderLightDefaultParameters GetShaderLightDefaultParameters() const override;
protected:

private:
	
};
