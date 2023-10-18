#pragma once

#include "../../Component/Component.h"

#include <string>

#include "../../Util/fast_obj.h"

#include "Renderer.h"
#include "../../../../../PetitMoteur3D/Core/Public/Mesh/chargeur.h"
#include "../../../../../PetitMoteur3D/Core/Public/Texture/CMaterial.h"

namespace PM3D_API
{
class MeshRenderer final : public Renderer
{
public:
	MeshRenderer(std::unique_ptr<Shader>&& shader, std::string meshName);
	MeshRenderer(std::unique_ptr<Shader>&& shader, PM3D::IChargeur* chargeur);

	~MeshRenderer() override;

	void Initialize() override;
	void DrawSelf() const override;

private:
	void LoadMesh();

	PM3D::IChargeur* chargeur;
	fastObjMesh* mesh;
	bool meshLoaded;

	// Les sous-objets
	int NombreSubmesh; // Nombre de sous-objets dans le mesh
	std::vector<int> SubmeshMaterialIndex; // Index des matériaux
	std::vector<int> SubmeshIndex; // Index des sous-objets
	std::vector<CMaterial> Material; // Vecteur des matériaux

};
}
