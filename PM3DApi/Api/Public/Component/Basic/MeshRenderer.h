#pragma once

#include "../../Component/Component.h"

#include <string>

#include "../../Util/fast_obj.h"
#include <d3d11.h>

#include "../../../../../PetitMoteur3D/Core/Public/Mesh/chargeur.h"
#include "../../../../../PetitMoteur3D/Core/Public/Shader/d3dx11effect.h"
#include "../../../../../PetitMoteur3D/Core/Public/Texture/CMaterial.h"

namespace PM3D_API
{
class MeshRenderer final : public Component
{
public:
	explicit MeshRenderer(std::string meshName);
	explicit MeshRenderer(PM3D::IChargeur* chargeur);

	~MeshRenderer() override;

	void Initialize() override;
	void DrawSelf() const override;

private:
	void LoadMesh();
	void InitEffect();

	PM3D::IChargeur* chargeur;
	fastObjMesh* mesh;
	bool meshLoaded;

	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3D11SamplerState* pSampleState;
	ID3D11Buffer* pConstantBuffer;
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique;
	ID3DX11EffectPass* pPasse;
	ID3D11InputLayout* pVertexLayout;

	// Les sous-objets
	int NombreSubmesh; // Nombre de sous-objets dans le mesh
	std::vector<int> SubmeshMaterialIndex; // Index des matériaux
	std::vector<int> SubmeshIndex; // Index des sous-objets
	std::vector<CMaterial> Material; // Vecteur des matériaux

};
}
