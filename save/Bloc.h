#pragma once

#include "Objet3D.h"
#include "DispositifD3D11.h"

namespace PM3D
{
// Classe : CBloc
// BUT : Classe de bloc
class CBloc : public CObjet3D
{
public:
	CBloc(const float dx,
	      const float dy,
	      const float dz,
	      CDispositifD3D11* pDispositif_);
	// Destructeur
	virtual ~CBloc();
	
	virtual void Anime(float tempsEcoule);
	void Draw();

private:
	CDispositifD3D11* pDispositif;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3D11VertexShader* pVertexShader;
	ID3D11InputLayout* pVertexLayout;
	ID3D11Buffer* pConstantBuffer;
	ID3D11PixelShader* pPixelShader;
	XMMATRIX matWorld;

	float rotation = 0.0f;

	void InitShaders();

};
}
