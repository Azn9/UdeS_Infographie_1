#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"
#include <Core/Public/Mesh/FastobjChargeur.h>

class objWithChargeur : public PM3D_API::GameObject
{
public:
	objWithChargeur() = delete;
	objWithChargeur(PM3D::FastobjChargeur* chargeur, std::string nom) : GameObject(nom) , _chargeur(chargeur){}
	void Initialize() override
	{
		ConstructRenderer();
	}

private:
	std::wstring GetShaderFileName() { return L"shader/NewShader.fx"; }
	PM3D::FastobjChargeur* _chargeur;
	void ConstructRenderer()
	{
		auto shader = std::make_unique<PM3D_API::DefaultShader>(GetShaderFileName());
		auto meshRenderer = std::make_unique<PM3D_API::MeshRenderer>(std::move(shader), _chargeur);
		meshRenderer->Initialize();
		GameObject::AddComponent(std::move(meshRenderer));
	}
};
