#pragma once

#include <DirectXMath.h>
#include <string>

#include "../../../../PetitMoteur3D/Core/Public/Shader/d3dx11effect.h"
#include "../../Public/GameHost.h"

namespace PM3D_API
{
class Shader
{
public:
	virtual ~Shader() = default;

	virtual ID3D11Buffer* GetShaderParametersBuffer() const = 0;
	virtual ID3DX11EffectPass* GetPass() const = 0;
	virtual ID3D11InputLayout* GetVertexLayout() const = 0;
	virtual ID3D11Buffer* GetIndexBuffer() const = 0;

	virtual ID3D11Buffer** GetVertexBufferPtr() const = 0;
	virtual ID3D11Buffer** GetIndexBufferPtr() const = 0;

	virtual void* PrepareParameters(
		DirectX::XMMATRIX matWorldViewProj,
		DirectX::XMMATRIX matWorld
	) = 0;
	virtual void ApplyMaterialParameters(
		void* shaderParameters,
		DirectX::XMVECTOR materialAmbiant,
		DirectX::XMVECTOR materialDiffuse,
		DirectX::XMVECTOR materialSpecular,
		float specularPower,
		ID3D11ShaderResourceView* albedoTexture,
		ID3D11ShaderResourceView* normalmapTexture
	) = 0;

	virtual void DeleteParameters(void* shader_parameters) = 0;
	
	virtual void ApplyShaderParams() const = 0;

	virtual void LoadLights(ID3D11DeviceContext* context) = 0;

};
}
