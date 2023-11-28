#pragma once
#include <d3d11.h>

#include "Api/Public/Scene/Scene.h"

class ShadowProcessor : public PM3D_API::Component
{
public:
	ShadowProcessor() = default;
	~ShadowProcessor() override;

	void Initialize() override;

	std::vector<PM3D_API::ShaderLightDefaultParameters> ProcessLights();
	void ProcessShadow();

	void SetScene(PM3D_API::Scene* scene) { this->scene = scene; }
private:
	PM3D_API::Scene* scene;
	ID3D11Texture2D* pDepthTexture;
	ID3D11DepthStencilView* depthStencilView;
};
