#pragma once
#include "Api/Public/Shader/Shader.h"

namespace PM3D_API
{
class SpriteShader final : public Shader
{
public:
	explicit SpriteShader(const std::wstring& fileName);
	~SpriteShader() override;

	ID3D11Buffer* GetShaderParametersBuffer() const override { return shaderParametersBuffer; }
	ID3DX11EffectPass* GetPass() const override { return passe; }
	ID3D11InputLayout* GetVertexLayout() const override { return vertexLayout; }
	ID3D11Buffer* GetIndexBuffer() const override { return indexBuffer; }

	ID3D11InputLayout* GetShadowVertexLayout() const override { return nullptr; }
	ID3D11Texture2D* GetDepthTexture() const override { return nullptr; }
	ID3D11DepthStencilView* GetDepthStencilView() const override { return nullptr; }
	ID3D11ShaderResourceView* GetDepthShaderResourceView() const override { return nullptr; }

	ID3DX11Effect* GetEffect() const override { return effect; }

	ID3D11Buffer** GetVertexBufferPtr() const override { return &vertexBuffer; }
	ID3D11Buffer** GetIndexBufferPtr() const override { return &indexBuffer; }

	void LoadLights(ID3D11DeviceContext* context, GameObject* gameObject) override;
	
	void* PrepareParameters(
		DirectX::XMMATRIX matWorldViewProj,
		DirectX::XMMATRIX matWorld
	) override;

	void ApplyMaterialParameters(
		void* shaderParameters,
		DirectX::XMVECTOR materialAmbiant,
		DirectX::XMVECTOR materialDiffuse,
		DirectX::XMVECTOR materialSpecular,
		float specularPower,
		ID3D11ShaderResourceView* albedoTexture,
		ID3D11ShaderResourceView* normalmapTexture
	) override;

	void DeleteParameters(void* shader_parameters) override;

	void ApplyShaderParams() const override {
		ID3DX11EffectConstantBuffer* pCB = effect->GetConstantBufferByName("param");
		pCB->SetConstantBuffer(shaderParametersBuffer);
	}

protected:
	std::wstring fileName;

	ID3D11Buffer* shaderParametersBuffer;
	ID3DX11Effect* effect;
	ID3DX11EffectTechnique* technique;
	ID3DX11EffectPass* passe;
	ID3D11InputLayout* vertexLayout;

	ID3D11SamplerState* albedoSampleState;

	mutable ID3D11Buffer* vertexBuffer;
	mutable ID3D11Buffer* indexBuffer;

private:
	struct SpriteShaderParameters
	{
		DirectX::XMMATRIX matWorldViewProj;
	};
};
}
