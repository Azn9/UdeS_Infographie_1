#pragma once
#include "Object/objet3d.h"
#include "Core/dispositifD3D11.h"
#include "Shader/d3dx11effect.h"
#include "chargeur.h"
#include <vector>
using namespace std;

namespace PM3D
{
class CObjetMesh : public CObjet3D
{
public:
	CObjetMesh(IChargeur& chargeur, CDispositifD3D11* pDispositif);
	virtual ~CObjetMesh();

private:
	// **** Données membres
	XMMATRIX matWorld; // Matrice de transformation dans le monde
	float rotation;
	
	// Pour le dessin
	CDispositifD3D11* pDispositif; // On prend en note le dispositif
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	
	// Les sous-objets
	int NombreSubmesh; // Nombre de sous-objets dans le mesh
	vector<int> SubmeshMaterialIndex; // Index des matériaux
	vector<int> SubmeshIndex; // Index des sous-objets
	vector<CMaterial> Material; // Vecteur des matériaux
	
	// Pour les effets et shaders
	ID3D11SamplerState* pSampleState;
	ID3D11Buffer* pConstantBuffer;
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique;
	ID3DX11EffectPass* pPasse;
	ID3D11InputLayout* pVertexLayout;
	
	CObjetMesh();
};
}
