#include "DefaultShader.h"

#include "../../../../../PetitMoteur3D/Core/Public/Util/resource.h"
#include "../../../../../PetitMoteur3D/Core/Public/Util/util.h"
#include "Api/Public/GameHost.h"


void PM3D_API::DefaultShader::Initialize()
{
	const auto pD3DDevice = GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DefaultShaderParameters);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bufferDesc, nullptr, &shaderParametersBuffer));

	ID3DBlob* pFXBlob = nullptr;
	PM3D::DXEssayer(D3DCompileFromFile(
		fileName.c_str(),
		nullptr,
		nullptr,
		nullptr,
		"fx_5_0",
		0,
		0,
		&pFXBlob,
		nullptr
		), DXE_ERREURCREATION_FX);

	D3DX11CreateEffectFromMemory(
		pFXBlob->GetBufferPointer(),
		pFXBlob->GetBufferSize(),
		0,
		pD3DDevice,
		&effect
	);

	pFXBlob->Release();
}

void* PM3D_API::DefaultShader::PrepareParameters(
	DirectX::XMMATRIX matWorldViewProj,
	DirectX::XMMATRIX matWorld
)
{
	ID3DX11EffectSamplerVariable* variableSampler = effect->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, sampleState);

	const auto parameters = new DefaultShaderParameters{
		
	};

	return parameters;
}
