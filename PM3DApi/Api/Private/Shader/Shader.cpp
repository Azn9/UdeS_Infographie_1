#include "../../Public/Shader/Shader.h"

#include "../../../../PetitMoteur3D/Core/Public/Util/resource.h"
#include "../../../../PetitMoteur3D/Core/Public/Util/util.h"
#include "Api/Public/GameHost.h"

Shader::~Shader()
{
	PM3D::DXRelacher(shaderParametersBuffer);
	PM3D::DXRelacher(effect);
	PM3D::DXRelacher(technique);
	PM3D::DXRelacher(passe);
	PM3D::DXRelacher(vertexLayout);
	PM3D::DXRelacher(vertexBuffer);
	PM3D::DXRelacher(indexBuffer);
	PM3D::DXRelacher(sampleState);
}

void Shader::Initialize()
{
	
}

template<typename T> void Shader::InitializeLayout(T layout)
{
	const auto d3DDevice = PM3D_API::GameHost::GetInstance()->GetDispositif()->GetD3DDevice();
	
	technique = effect->GetTechniqueByIndex(0);
	passe = technique->GetPassByIndex(0);

	D3DX11_PASS_SHADER_DESC effectVSDesc;
	passe->GetVertexShaderDesc(&effectVSDesc);

	D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
	effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

	const void* vsCodePtr = effectVSDesc2.pBytecode;
	const uint32_t vsCodeLen = effectVSDesc2.BytecodeLength;
	vertexLayout = nullptr;

	int elementCount = ARRAYSIZE(layout);

	PM3D::DXEssayer(d3DDevice->CreateInputLayout(layout,
			elementCount,
			vsCodePtr,
			vsCodeLen,
			&vertexLayout),
		DXE_CREATIONLAYOUT);

	// Initialisation des paramètres de sampling de la texture
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Création de l’état de sampling
	d3DDevice->CreateSamplerState(&samplerDesc, &sampleState);
}