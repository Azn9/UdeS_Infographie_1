#pragma once

#include <DirectXMath.h>
#include <string>

#include "../../../../PetitMoteur3D/Core/Public/Shader/d3dx11effect.h"

class Shader
{
public:
	explicit Shader(std::wstring fileName) : fileName(std::move(fileName)) {}
	virtual ~Shader();
	
	virtual void Initialize() = 0;

	template <typename T>
	void InitializeLayout(T layout);

	const std::wstring& GetFileName() const { return fileName; }
	ID3D11Buffer* GetShaderParametersBuffer() const { return shaderParametersBuffer; }
	ID3DX11Effect* GetEffect() const { return effect; }
	ID3DX11EffectTechnique* GetTechnique() const { return technique; }
	ID3DX11EffectPass* GetPass() const { return passe; }
	ID3D11InputLayout* GetVertexLayout() const { return vertexLayout; }
	ID3D11Buffer* GetVertexBuffer() const { return vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() const { return indexBuffer; }
	ID3D11SamplerState* GetSampleState() const { return sampleState; }

	ID3D11Buffer* const* GetVertexBufferPtr() const { return &vertexBuffer; }

	virtual void* PrepareParameters(
		DirectX::XMMATRIX matWorldViewProj,
		DirectX::XMMATRIX matWorld
	) = 0;

protected:
	std::wstring fileName;

	ID3D11Buffer* shaderParametersBuffer = nullptr;
	ID3DX11Effect* effect = nullptr;
	ID3DX11EffectTechnique* technique = nullptr;
	ID3DX11EffectPass* passe = nullptr;
	ID3D11InputLayout* vertexLayout = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11SamplerState* sampleState = nullptr;
	
private:
	
};
