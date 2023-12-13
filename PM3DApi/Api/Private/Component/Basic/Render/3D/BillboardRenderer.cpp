#include "Api/Public/Component/Basic/Render/3D/BillboardRenderer.h"

#include "Core/Public/Core/MoteurWindows.h"
#include "Core/Public/Mesh/CSommetMesh.h"
#include "Core/Public/Texture/GestionnaireDeTextures.h"

PM3D_API::BillboardRenderer::~BillboardRenderer()
{

}

void PM3D_API::BillboardRenderer::Initialize()
{
	const auto pDispositif = GameHost::GetInstance()->GetDispositif();
	texture = PM3D::CMoteurWindows::GetInstance().GetTextureManager().GetNewTexture(textureName, pDispositif);
}

void PM3D_API::BillboardRenderer::DrawSelf() const
{
	LogBeginDrawSelf();

	const auto pTexture = texture->GetD3DTexture();

	// Obtenir le contexte
	const auto pDispositif = GameHost::GetInstance()->GetDispositif();
	ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

	// Choisir la topologie des primitives
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// input layout des sommets
	pImmediateContext->IASetInputLayout(shader->GetVertexLayout());

	// Index buffer
	pImmediateContext->IASetIndexBuffer(shader->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Vertex buffer
	constexpr UINT stride = sizeof(CSommetMesh);
	constexpr UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, shader->GetVertexBufferPtr(), &stride, &offset);

	const auto camera = GameHost::GetInstance()->GetScene()->GetMainCamera();
	const auto cameraPosition = camera->GetWorldPosition();
	const auto cameraDirection = camera->GetWorldDirection();
	
	const auto position = parentObject->GetWorldPosition();
	XMFLOAT3 scale = parentObject->GetWorldScale();
	scale.x *= selfScale.x;
	scale.y *= selfScale.y;
	scale.z *= selfScale.z;

	XMMATRIX worldMatrix;

	const auto cameraPosVec = XMVectorSet(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);
	const auto cameraDirVec = XMVectorSet(cameraDirection.x, cameraDirection.y, cameraDirection.z, 1.0f);
	const auto cameraFocusVec = cameraPosVec + cameraDirVec;

	switch (alignment)
	{
	case BillboardAlignment::Z:
		worldMatrix = XMMatrixIdentity();
		worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);
		worldMatrix *= XMMatrixScaling(scale.x, scale.y, 1.0f);

		// Regarde toujours vers la cam�ra
		XMMATRIX lookAtMatrix = XMMatrixLookAtLH(
			cameraPosVec,
			cameraFocusVec,
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		);

		worldMatrix *= lookAtMatrix;

		break;
	case BillboardAlignment::XZ:
		worldMatrix = XMMatrixIdentity();
		worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);
		worldMatrix *= XMMatrixScaling(scale.x, scale.y, 1.0f);

		lookAtMatrix = XMMatrixLookAtLH(
			cameraPosVec,
			cameraFocusVec,
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		);

		worldMatrix *= lookAtMatrix;
		break;
	case BillboardAlignment::YZ:
		worldMatrix = XMMatrixIdentity();
		worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);
		worldMatrix *= XMMatrixScaling(1.0f, scale.y, scale.z);

		lookAtMatrix = XMMatrixLookAtLH(
			cameraPosVec,
			cameraFocusVec,
			XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)
		);

		worldMatrix *= lookAtMatrix;
		break;
	case BillboardAlignment::XYZ:
		worldMatrix = XMMatrixIdentity();
		worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);
		worldMatrix *= XMMatrixScaling(scale.x, scale.y, scale.z);

		lookAtMatrix = XMMatrixLookAtLH(
			cameraPosVec,
			cameraFocusVec,
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		);

		worldMatrix *= lookAtMatrix;
		break;
	default: ;
	}
	
	const XMMATRIX viewProj = camera->GetMatViewProj();

	const auto shaderParameters = shader->PrepareParameters(
		worldMatrix * viewProj,
		worldMatrix
	);

	shader->ApplyMaterialParameters(
		shaderParameters,
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		0.0f,
		pTexture,
		nullptr
	);

	shader->GetPass()->Apply(0, pImmediateContext);

	shader->ApplyShaderParams();

	pImmediateContext->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, shaderParameters, 0, 0);

	pImmediateContext->Draw(6, 0);

	shader->DeleteParameters(shaderParameters);

	LogEndDrawSelf();
}
