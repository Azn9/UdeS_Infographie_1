#include "../../../Public/GameObject/Basic/Plane.h"

#include <DirectXMath.h>

#include "../../../../../PetitMoteur3D/Core/Public/Mesh/sommetbloc.h"
#include "../../../../../PetitMoteur3D/Core/Public/Util/resource.h"
#include "../../../../../PetitMoteur3D/Core/Public/Util/util.h"
#include "Api/Public/Shader/Basic/DefaultShader.h"

void Plane::Initialize()
{
	shader = std::make_unique<PM3D_API::DefaultShader>(L"NewShader.fx");

	DirectX::XMFLOAT3 points[4] = {
		DirectX::XMFLOAT3(-1.0f * worldScale.x + worldPosition.x, worldPosition.y, -1.0f * worldScale.z + worldPosition.z),
		DirectX::XMFLOAT3(-1.0f * worldScale.x + worldPosition.x, worldPosition.y, 1.0f * worldScale.z + worldPosition.z),
		DirectX::XMFLOAT3(1.0f * worldScale.x + worldPosition.x, worldPosition.y, 1.0f * worldScale.z + worldPosition.z),
		DirectX::XMFLOAT3(1.0f * worldScale.x + worldPosition.x, worldPosition.y, -1.0f * worldScale.z + worldPosition.z)
	};
	this->points = points;

	normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	PM3D::CSommetBloc sommets[4] =
	{
		PM3D::CSommetBloc(points[0], normal, DirectX::XMFLOAT2(0.0f, 1.0f)),
		PM3D::CSommetBloc(points[1], normal, DirectX::XMFLOAT2(0.0f, 0.0f)),
		PM3D::CSommetBloc(points[2], normal, DirectX::XMFLOAT2(1.0f, 0.0f)),
		PM3D::CSommetBloc(points[3], normal, DirectX::XMFLOAT2(1.0f, 1.0f)),
	};
	this->sommets = sommets;

	ID3D11Device* pD3DDevice = PM3D_API::GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(sommets);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = sommets;

	PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

	// Création de l'index buffer et copie des indices
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(index_bloc);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = index_bloc;

	PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer), DXE_CREATIONINDEXBUFFER);
}

void Plane::DrawSelf() const
{
	LogBeginDrawSelf();
	
	ID3D11DeviceContext* pImmediateContext = PM3D_API::GameHost::GetInstance()->GetDispositif()->GetImmediateContext();

	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const UINT stride = sizeof(PM3D::CSommetBloc);
	const UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->IASetInputLayout(shader->GetVertexLayout());

	pImmediateContext->VSSetShader(nullptr, nullptr, 0);

	shader->LoadLights(pImmediateContext);
	
	const DirectX::XMMATRIX viewProj = PM3D_API::GameHost::GetInstance()->GetScene()->GetMainCamera()->GetMatViewProj();
	
	const auto shaderParameters = shader->PrepareParameters(
		XMMatrixTranspose(matWorld * viewProj),
		DirectX::XMMatrixTranspose(matWorld)
	);

	shader->ApplyMaterialParameters(
		shaderParameters,
		DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		nullptr
	);

	shader->GetPass()->Apply(0, pImmediateContext);
	
	shader->ApplyShaderParams();

	pImmediateContext->DrawIndexed(ARRAYSIZE(index_bloc), 0, 0);

	LogEndDrawSelf();
}
