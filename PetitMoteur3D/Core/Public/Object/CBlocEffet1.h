#pragma once
#include "Objet3D.h"
#include "bloc.h"
#include "Core/Public/Shader/d3dx11effect.h"
#include "Core/Public/Texture/Texture.h"

namespace PM3D
{

class CDispositifD3D11;


//  Classe : CBloc
//
//  BUT : 	Classe de bloc
//
class CBlocEffet1 : public CObjet3D
{
public:
	CBlocEffet1(const float dx,
		  const float dy,
		  const float dz,
		  CDispositifD3D11* pDispositif);

	// Destructeur
	virtual ~CBlocEffet1();

	virtual void Anime(float tempsEcoule) override;
	virtual void Draw() override;

	void SetTexture(CTexture* pTexture);

private:
	CDispositifD3D11* pDispositif;
	void InitEffet();

	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;

	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3D11InputLayout* pVertexLayout;

	// Définitions des valeurs d'animation
	ID3D11Buffer* pConstantBuffer;
	XMMATRIX matWorld;
	float rotation;
	float position;

	// Pour les effets
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique;
	ID3DX11EffectPass* pPasse;

	// Textures
	ID3D11ShaderResourceView* pTextureD3D;
	ID3D11SamplerState* pSampleState;
};

} // namespace PM3D
