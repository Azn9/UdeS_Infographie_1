#pragma once
#include <stdbool.h>

#include "../../../../../Public/Component/Component.h"
#include "../../../../../Public/Shader/Shader.h"

namespace PM3D_API
{
class Renderer : public Component
{
public:
	explicit Renderer(std::unique_ptr<Shader>&& shader) : shader(std::move(shader)) {}

	const Shader* GetShader() const { return shader.get(); }

	virtual bool IsVisible() const {return true;}

protected:
	std::unique_ptr<Shader> shader;
	
private:
	
};
}
