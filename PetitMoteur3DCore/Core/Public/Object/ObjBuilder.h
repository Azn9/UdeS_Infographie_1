#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"
#include "Api/Public/Component/Basic/Physics/MeshCollider.h"
#include "Core/Public/Mesh/FastobjChargeur.h"

template <class T>
class ObjBuilder {
public:
	ObjBuilder(std::string fileName) {

		if (fileName.find_last_of(".obj") == std::string::npos)
			fileName += ".obj";

		_chargeur = std::make_shared<PM3D::FastobjChargeur>();

		PM3D::CParametresChargement params;
		params.NomChemin = "";
		params.NomFichier = fileName;

		_chargeur->Chargement(params);
	}

	/// <summary>
	/// Build an object 
	/// </summary>
	/// <param name="physicsResolver"></param>
	/// <param name="position"></param>
	/// <param name="scale"></param>
	/// <param name="rotation"></param>
	/// <returns></returns>
	std::unique_ptr<T> build
		(XMFLOAT3 position = XMFLOAT3(0.f, 0.f, 0.f),
		XMFLOAT3 scale = XMFLOAT3(1.f, 1.f, 1.f),
		XMFLOAT3 rotation = XMFLOAT3(0.f, 0.f, 0.f)) 
	{
		auto obj = std::make_unique<T>(_chargeur);
		obj->SetWorldPosition(position);
		obj->SetWorldScale(scale);
		obj->SetWorldRotation(rotation);
		obj->Initialize();

		return obj;
	}

private:
	std::shared_ptr<PM3D::FastobjChargeur> _chargeur;
};