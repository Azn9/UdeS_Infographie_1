#pragma once
#include <vector>
#include <DirectXMath.h>

#include "../../Public/Texture/CMaterial.h"

using namespace DirectX;

namespace PM3D
{

class CParametresChargement
{
public:
	CParametresChargement()
	{
		bInverserCulling = false;
		bMainGauche = false;
	}

	std::string NomFichier;
	std::string NomChemin;
	bool bInverserCulling;
	bool bMainGauche;
};

class IChargeur
{
public:
	virtual ~IChargeur(void) = default;

	virtual void Chargement(const CParametresChargement& param) = 0;

	virtual size_t GetNombreSommets() const = 0;
	virtual size_t GetNombreIndex() const = 0;
	virtual const void* GetIndexData()  const = 0;
	virtual int GetNombreSubset() const = 0;
	virtual size_t  GetNombreMaterial() const = 0;
	virtual CMaterial GetMaterial(int _i) const = 0;

	virtual const std::string& GetMaterialName(int i) const = 0;

	virtual void CopieSubsetIndex(std::vector<int>& dest) const = 0;

	virtual const XMFLOAT3& GetPosition(int NoSommet) const = 0;
	virtual const XMFLOAT2& GetCoordTex(int NoSommet) const = 0;
	virtual const XMFLOAT3& GetNormale(int NoSommet) const = 0;
	virtual const XMFLOAT3& GetBiNormale(int NoSommet) const = 0;
	virtual const XMFLOAT3& GetTangent(int NoSommet) const = 0;

	virtual void* GetMesh()
	{
		return nullptr;
	}
};

} // namespace PM3D
