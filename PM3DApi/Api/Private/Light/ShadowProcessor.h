#pragma once
#include <d3d11.h>

#include "Api/Public/Scene/Scene.h"

class ShadowProcessor
{
public:
	ShadowProcessor() = default;
	~ShadowProcessor() = default;

	void ProcessShadow();

private:
	friend class PM3D_API::Scene;
	PM3D_API::Scene* scene;

	void SetScene(PM3D_API::Scene* scene) { this->scene = scene; }


	ID3D11DepthStencilView* depthStencilView;
};
