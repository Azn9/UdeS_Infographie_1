#include "Api/Public/Component/Basic/MeshRenderer.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdexcept>

#include "Api/Public/GameHost.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Scene/Scene.h"

PM3D_API::MeshRenderer::MeshRenderer(std::string meshName)
{
	if (meshName.find_last_of(".obj") == std::string::npos)
		meshName += ".obj";

	mesh = fast_obj_read(meshName.c_str());
}

PM3D_API::MeshRenderer::MeshRenderer(fastObjMesh* mesh) : mesh(mesh)
{

}



PM3D_API::MeshRenderer::~MeshRenderer()
{
	fast_obj_destroy(mesh);
}

void PM3D_API::MeshRenderer::Initialize()
{
	// Load mesh

	
}

struct ShadersParams
{
	DirectX::XMMATRIX matWorldViewProj; // la matrice totale
	DirectX::XMMATRIX matWorld; // matrice de transformation dans le monde
	DirectX::XMVECTOR vLumiere; // la position de la source d’éclairage (Point)
	DirectX::XMVECTOR vCamera; // la position de la caméra
	DirectX::XMVECTOR vAEcl; // la valeur ambiante de l’éclairage
	DirectX::XMVECTOR vAMat; // la valeur ambiante du matériau
	DirectX::XMVECTOR vDEcl; // la valeur diffuse de l’éclairage
	DirectX::XMVECTOR vDMat; // la valeur diffuse du matériau
	DirectX::XMVECTOR vSEcl; // la valeur spéculaire de l’éclairage
	DirectX::XMVECTOR vSMat; // la valeur spéculaire du matériau
	float puissance; // la puissance de spécularité
	int bTex; // Texture ou materiau
	DirectX::XMFLOAT2 remplissage;
};

void PM3D_API::MeshRenderer::DrawSelf() const
{
	if (!mesh)
		throw std::runtime_error("MeshRenderer::DrawSelf: mesh is null");

	const auto scene = parentObject->GetScene();
	const auto camera = scene->GetMainCamera();
	const auto directionnalLight = scene->GetDirectionalLight();

	if (!camera || !directionnalLight)
		throw std::runtime_error("MeshRenderer::DrawSelf: camera or directionnalLight is null");

	// Obtenir le contexte
	ID3D11DeviceContext* pImmediateContext = GameHost::GetInstance()->GetDispositif()->GetImmediateContext();

	// Choisir la topologie des primitives
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// input layout des sommets
	pImmediateContext->IASetInputLayout(pVertexLayout);

	// Index buffer
	pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Vertex buffer
	UINT stride = sizeof(CSommetMesh);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Initialiser et sélectionner les « constantes » de l’effet
	ShadersParams sp;
	DirectX::XMMATRIX viewProj = camera->GetMatViewProj();
	sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
	sp.matWorld = DirectX::XMMatrixTranspose(matWorld);

	if (directionnalLight)
	{
		auto rotation = directionnalLight->GetRotation();
		sp.vLumiere = DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, 1.0f);
	}
	else
	{
		sp.vLumiere = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	}

	auto cameraPosition = camera->GetPosition();
	sp.vCamera = DirectX::XMVectorSet(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);
	
	sp.vAEcl = DirectX::XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
	sp.vDEcl = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	sp.vSEcl = DirectX::XMVectorSet(0.6f, 0.6f, 0.6f, 1.0f);

	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, pSampleState);

	// Dessiner les sous-objets non-transparents
	for (int i = 0; i < mesh->object_count; ++i)
	{
		auto objGroup = mesh->objects[i];
		unsigned indexStart = objGroup.index_offset;
		int indexDrawAmount = mesh->objects[i + 1].index_offset - indexStart;

		if (!indexDrawAmount)
		{
			continue;
		}

		

		sp.vAMat = DirectX::XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Ambient);
		sp.vDMat = DirectX::XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Diffuse);
		sp.vSMat = DirectX::XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Specular);
		sp.puissance = Material[SubmeshMaterialIndex[i]].Puissance;

		// Activation de la texture ou non
		if (Material[SubmeshMaterialIndex[i]].pTextureD3D != nullptr)
		{
			ID3DX11EffectShaderResourceVariable* variableTexture;
			variableTexture =
			pEffet->GetVariableByName("textureEntree")->AsShaderResource();
			variableTexture->SetResource(Material[SubmeshMaterialIndex[i]].pTextureD3D);
			sp.bTex = 1;
		}
		else
		{
			sp.bTex = 0;
		}

		// IMPORTANT pour ajuster les param.
		pPasse->Apply(0, pImmediateContext);

		// Nous n’avons qu’un seul CBuffer
		ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
		pCB->SetConstantBuffer(pConstantBuffer);

		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &sp, 0, 0);
		pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
	}
}
