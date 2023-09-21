#include "Api/Public/Component/Basic/MeshRenderer.h"

#include <DirectXMath.h>
#include <stdexcept>

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

struct ShadersParams
{
	DirectX::XMMATRIX matWorldViewProj; // la matrice totale
	DirectX::XMMATRIX matWorld; // matrice de transformation dans le monde
	DirectX::XMVECTOR vLumiere; // la position de la source d’éclairage
	DirectX::XMVECTOR vCamera; // la position de la caméra
	DirectX::XMVECTOR vAEcl; // la valeur ambiante de l’éclairage
	DirectX::XMVECTOR vAMat; // la valeur ambiante du matériau
	DirectX::XMVECTOR vDEcl; // la valeur diffuse de l’éclairage
	DirectX::XMVECTOR vDMat; // la valeur diffuse du matériau
};

void PM3D_API::MeshRenderer::DrawSelf() const
{
	if (!mesh)
		throw std::runtime_error("MeshRenderer::DrawSelf: mesh is null");

	auto scene = parentObject->GetScene();
	auto camera = scene->GetMainCamera();
	auto directionnalLight = scene->GetDirectionalLight();


}
