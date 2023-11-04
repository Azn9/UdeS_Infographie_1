#pragma once
#include "../../../../../Public/Component/Component.h"
#include "../../../../../Public/Shader/Shader.h"

namespace PM3D_API
{
class Renderer : public Component
{
public:
	explicit Renderer(std::unique_ptr<Shader>&& shader) : shader(std::move(shader)) {}

	const Shader* GetShader() const { return shader.get(); }

protected:
	std::unique_ptr<Shader> shader;
	
private:
	
};
}
