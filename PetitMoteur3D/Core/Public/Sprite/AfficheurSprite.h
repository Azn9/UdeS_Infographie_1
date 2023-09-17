﻿#pragma once

#include "SpriteTemp.h"
#include "Core/Public/Shader/d3dx11effect.h"
#include "Core/Public/Core/DispositifD3D11.h"
#include "Core/Public/Object/objet3d.h"

namespace PM3D
{
class CSommetSprite
{
public:
	CSommetSprite() = default;

	CSommetSprite(const XMFLOAT3& position, const XMFLOAT2& coordTex)
		: m_Position(position)
		  , m_CoordTex(coordTex)
	{
	}

public:
	static UINT numElements;
	static D3D11_INPUT_ELEMENT_DESC layout[];
	XMFLOAT3 m_Position;
	XMFLOAT2 m_CoordTex;
};

class CAfficheurSprite : public CObjet3D
{
public:
	CAfficheurSprite(
		CDispositifD3D11* pDispositif
	);
	virtual ~CAfficheurSprite();

	void AjouterSprite(std::string NomTexture, int _x, int _y, int _dx=0, int _dy=0);

private:
	class CSprite
	{
		public :
			ID3D11ShaderResourceView* pTextureD3D;
		XMMATRIX matPosDim;
		bool bPanneau;

		CSprite()
			: bPanneau(false)
			  , pTextureD3D(nullptr)
		{
		}
	};

private:
	static CSommetSprite sommets[6];
	ID3D11Buffer* pVertexBuffer;
	CDispositifD3D11* pDispositif;

	ID3D11Buffer* pConstantBuffer;
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique;
	ID3DX11EffectPass* pPasse;
	ID3D11InputLayout* pVertexLayout;
	ID3D11SamplerState* pSampleState;

	// Tous nos sprites
	std::vector<std::unique_ptr<CSprite>> tabSprites;

	virtual void InitEffet();

	virtual void Anime(float) override
	{
	};
	virtual void Draw();
};
} // namespace PM3D
