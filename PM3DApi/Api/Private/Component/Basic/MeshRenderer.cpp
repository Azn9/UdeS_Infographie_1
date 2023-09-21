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
}
