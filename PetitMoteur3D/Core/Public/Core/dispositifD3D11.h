#pragma once

#include "dispositif.h"

namespace PM3D
{

class CDispositifD3D11 final : public CDispositif
{
public:
	CDispositifD3D11(const CDS_MODE cdsMode, const HWND hWnd);
	CDispositifD3D11(const CDS_MODE cdsMode, const HWND hWnd, UINT largeur, UINT hauteur);
	virtual ~CDispositifD3D11();

	void PresentSpecific() override;

	void ActiverMelangeAlpha() const;
    void DesactiverMelangeAlpha() const;

	void ActiverDepth() const;
	void DesactiverDepth() const;

	// Fonction d'accès aux membres protégés
	ID3D11Device* GetD3DDevice() { return pD3DDevice; }
	ID3D11DeviceContext* GetImmediateContext() { return pImmediateContext; }
	IDXGISwapChain* GetSwapChain() { return pSwapChain; }
	ID3D11RenderTargetView* GetRenderTargetView() { return pRenderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() { return pDepthStencilView; }
	ID3D11RasterizerState* GetRasterizerState() { return mSolidCullBackRS; }
	ID3D11DepthStencilState* GetDepthStencilState() { return pDepthStencilState; }
	ID3D11DepthStencilState* GetNoDepthStencilState() { return pNoDepthStencilState; }
	
	ID3D11DepthStencilView** GetDepthStencilViewPtr() { return &pDepthStencilView; }
	ID3D11RenderTargetView** GetRenderTargetViewPtr() { return &pRenderTargetView; }
	ID3D11RasterizerState** GetRasterizerStatePtr() { return &mSolidCullBackRS; }

	void InitDepthBuffer();
	void InitDepthState();
	void InitBlendStates();


private:
	ID3D11Device* pD3DDevice;
	ID3D11DeviceContext* pImmediateContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRenderTargetView;

	// Pour le tampon de profondeur
	ID3D11Texture2D* pDepthTexture;
	ID3D11DepthStencilView* pDepthStencilView;
	
	ID3D11DepthStencilState* pDepthStencilState;
	ID3D11DepthStencilState* pNoDepthStencilState;

	// Variables d'état
	ID3D11RasterizerState* mSolidCullBackRS;

	// Pour le mélange alpha (transparence)
    ID3D11BlendState* alphaBlendEnable;
    ID3D11BlendState* alphaBlendDisable;

};

} // namespace PM3D
