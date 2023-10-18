#pragma once
#include <DirectXMath.h>

#include "Api/Public/Shader/Shader.h"

namespace PM3D_API
{
class DefaultShader final : public Shader
{
public:
	explicit DefaultShader(const std::wstring& fileName)
		: Shader(fileName)
	{
	}

	void Initialize() override;

	void* PrepareParameters(
		DirectX::XMMATRIX matWorldViewProj,
		DirectX::XMMATRIX matWorld
	) override;

protected:


private:
	struct DefaultShaderParameters
	{
		DirectX::XMMATRIX matWorldViewProj;
		DirectX::XMMATRIX matWorld;

		DirectX::XMVECTOR cameraPos;

		DirectX::XMVECTOR materialAmbiant;
		DirectX::XMVECTOR materialDiffuse;
		DirectX::XMVECTOR materialSpecular;

		float materialSpecularPower;

		bool hasTexture;
	};
};
}