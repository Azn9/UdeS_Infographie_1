#pragma once
#include <d3d11.h>
#include <vector>

#include "Api/Public/Component/Component.h"

namespace PM3D_API
{
	class Scene;
	struct ShaderLightDefaultParameters;
}

class ShadowProcessor : public PM3D_API::Component
{
public:
	ShadowProcessor() = default;
	~ShadowProcessor() override;

	void Initialize() override;

	std::vector<PM3D_API::ShaderLightDefaultParameters> ProcessLights();
	void ProcessShadow();

	void SetScene(PM3D_API::Scene* scene) { this->scene = scene; }

	ID3D11ShaderResourceView* getDepthTextureResourceView() const { return pDepthTextureResourceView; }
private:
	PM3D_API::Scene* scene;
	ID3D11Texture2D* pDepthTexture;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11ShaderResourceView* pDepthTextureResourceView;
};
