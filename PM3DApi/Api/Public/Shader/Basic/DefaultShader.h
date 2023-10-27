#pragma once

#include "../../../Public/Shader/Shader.h"

namespace PM3D_API
{
class DefaultShader final : public Shader
{
public:
	explicit DefaultShader(const std::wstring& fileName);
	~DefaultShader() override;

	ID3D11Buffer* GetShaderParametersBuffer() const override { return shaderParametersBuffer; }
	ID3DX11EffectPass* GetPass() const override { return passe; }
	ID3D11InputLayout* GetVertexLayout() const override { return vertexLayout; }
	ID3D11Buffer* GetIndexBuffer() const override { return indexBuffer; }

	ID3D11InputLayout* GetShadowVertexLayout() const override { return vertexLayoutShadow; }
	ID3D11Texture2D* GetDepthTexture() const override { return depthTexture; }
	ID3D11DepthStencilView* GetDepthStencilView() const override { return depthStencilView; }
	ID3D11ShaderResourceView* GetDepthShaderResourceView() const override { return depthShaderResourceView; }

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
	ID3D11SamplerState* normalmapSampleState;

	ID3D11InputLayout* vertexLayoutShadow;
	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11ShaderResourceView* depthShaderResourceView;

	mutable ID3D11Buffer* vertexBuffer;
	mutable ID3D11Buffer* indexBuffer;

private:
	struct DefaultShaderParameters
	{
		DirectX::XMMATRIX matWorldViewProj;
		DirectX::XMMATRIX matWorld;

		DirectX::XMVECTOR cameraPos;
		DirectX::XMVECTOR cameraDir;

		DirectX::XMVECTOR materialAmbiant;
		DirectX::XMVECTOR materialDiffuse;
		DirectX::XMVECTOR materialSpecular;

		float materialSpecularPower;

		bool hasAlbedoTexture;
		bool hasNormalmapTexture;
	};
};
}