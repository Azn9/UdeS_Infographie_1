#pragma once
#include "chargeur.h"

#include "fast_obj.h"

namespace PM3D
{
class FastobjChargeur : public IChargeur
{
public:
	~FastobjChargeur() override;
	void Chargement(const CParametresChargement& param) override;
	size_t GetNombreSommets() const override;
	int GetNombreSubset() const override;
	size_t GetNombreMaterial() const override;
	void GetMaterial(int _i,
		std::string& _NomFichierTexture,
		std::string& _NomMateriau,
		XMFLOAT4& _Ambient,
		XMFLOAT4& _Diffuse,
		XMFLOAT4& _Specular,
		float& _Puissance) const override;
	const std::string& GetMaterialName(int i) const override;
	void CopieSubsetIndex(std::vector<int>& dest) const override;
	const XMFLOAT3& GetPosition(int NoSommet) const override;
	const XMFLOAT2& GetCoordTex(int NoSommet) const override;
	const XMFLOAT3& GetNormale(int NoSommet) const override;

	size_t GetNombreIndex() const override { return tabIndex.size(); }
	const void* GetIndexData() const override { return tabIndex.data(); }

	void* GetMesh() override { return mesh; }

private:
	fastObjMesh* mesh;

	std::vector<int> SubsetIndex;

	// *** Les sommets 
	std::vector<XMFLOAT3> Position;				// Tableau des positions ( v )
	std::vector<XMFLOAT2> CoordTex;				// Tableau des coordonn�es de texture ( vt )
	std::vector<XMFLOAT3> Normale;				// Tableau des normales ( vn )

	std::vector<std::string> materialName;

	std::vector<int> tabIndex;
};

}
