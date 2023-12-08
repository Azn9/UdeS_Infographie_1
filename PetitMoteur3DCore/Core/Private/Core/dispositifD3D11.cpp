#include <iostream>

#include "Core/Public/Util/resource.h"
#include "Core/Public/Core/DispositifD3D11.h"
#include "Core/Public/Util/Util.h"
#include "Core/Public/Core/InfoDispositif.h"

namespace PM3D
{

CDispositifD3D11::~CDispositifD3D11()
{
	pSwapChain->SetFullscreenState(FALSE, nullptr); // passer en mode fen�tr�

	DXRelacher(mSolidCullBackRS);
	DXRelacher(pDepthTexture);
	DXRelacher(pDepthStencilView);
	DXRelacher(pDepthStencilState);
	DXRelacher(pNoDepthTestDepthStencilState);
	DXRelacher(pNoDepthTestAndWriteTestDepthStencilState);

	if (pImmediateContext)
	{
		pImmediateContext->ClearState();
	}
	DXRelacher(pRenderTargetView);
	DXRelacher(pImmediateContext);
	DXRelacher(pSwapChain);
	DXRelacher(pD3DDevice);
}

//  FONCTION : CDispositifD3D11, constructeur param�tr� 
//  BUT :	Constructeur de notre classe sp�cifique de dispositif 
//  PARAM�TRES:		
//		cdsMode:	CDS_FENETRE application fen�tr�e
//					CDS_PLEIN_ECRAN application plein �cran
//
//		hWnd:	Handle sur la fen�tre Windows de l'application,
//    			n�cessaire pour la fonction de cr�ation du 
//				dispositif
CDispositifD3D11::CDispositifD3D11(const CDS_MODE cdsMode,
								   const HWND hWnd) : CDispositifD3D11(cdsMode, hWnd, 1280, 720)
{
	std::cout << "CDispositifD3D11::CDispositifD3D11(const CDS_MODE cdsMode, const HWND hWnd)" << std::endl;
}
	
CDispositifD3D11::CDispositifD3D11(const CDS_MODE cdsMode, const HWND hWnd, UINT largeur, UINT hauteur)
{
	std::cout << "4A" << std::endl;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	const UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	pImmediateContext = nullptr;
	pSwapChain = nullptr;
	pRenderTargetView = nullptr;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	// Obtenir les informations de l'adaptateur de d�faut
	CInfoDispositif Dispo0(ADAPTATEUR_COURANT);

	switch (cdsMode)
	{
	case CDS_FENETRE:
		sd.Windowed = TRUE;

		break;

	case CDS_PLEIN_ECRAN:
		sd.Windowed = FALSE;
		break;

	}

	DXGI_MODE_DESC desc;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height = hauteur;
	desc.Width = largeur;
	desc.RefreshRate.Numerator = 60;
	desc.RefreshRate.Denominator = 1;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	CInfoDispositif DispoVoulu(desc);
	DispoVoulu.GetDesc(desc);

	largeur = desc.Width;
	hauteur = desc.Height;
	largeurEcran = largeur;
	hauteurEcran = hauteur;

	sd.BufferCount = 1;
	sd.BufferDesc.Width = desc.Width;
	sd.BufferDesc.Height = desc.Height;
	sd.BufferDesc.Format = desc.Format;
	sd.BufferDesc.RefreshRate.Numerator = desc.RefreshRate.Numerator;
	sd.BufferDesc.RefreshRate.Denominator = desc.RefreshRate.Denominator;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Permettre l'�change plein �cran

	// r�gler le probl�me no 1 du passage en mode fen�tr�
	RECT rcClient, rcWindow;
	POINT ptDiff;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(hWnd, rcWindow.left, rcWindow.top, largeur + ptDiff.x, hauteur + ptDiff.y, TRUE);

	const auto code = D3D11CreateDeviceAndSwapChain(0,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pD3DDevice,
			nullptr,
			&pImmediateContext);

	if (code != S_OK)
	{
		std::cerr << "Erreur de cr�ation du dispositif Direct3D 11" << std::endl;
		abort();
	}

	// Cr�ation d'un �render target view�
	ID3D11Texture2D* pBackBuffer;
	DXEssayer(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer), DXE_ERREUROBTENTIONBUFFER);

	DXEssayer(pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView), DXE_ERREURCREATIONRENDERTARGET);
	pBackBuffer->Release();

	InitDepthBuffer();

	InitDepthState();
	//DesactiverDepth();
	SetDepthState(true, true);

	InitBlendStates();

	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)largeur;
	vp.Height = (FLOAT)hauteur;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1, &vp);

	// Cr�ation et initialisation des �tats
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK; // FRONT,BACK ou NONE
	rsDesc.FrontCounterClockwise = false;
	pD3DDevice->CreateRasterizerState(&rsDesc, &mSolidCullBackRS);

	pImmediateContext->RSSetState(mSolidCullBackRS);
}

void CDispositifD3D11::PresentSpecific()
{
	pSwapChain->Present(0, 0);
}

void CDispositifD3D11::ActiverMelangeAlpha() const
{
	// Activer le m�lange - alpha blending.
    pImmediateContext->OMSetBlendState(alphaBlendEnable, nullptr, 0xffffffff);
}

void CDispositifD3D11::DesactiverMelangeAlpha() const
{
	// Activer le m�lange - alpha blending.
	pImmediateContext->OMSetBlendState(alphaBlendDisable, nullptr, 0xffffffff);
}

void CDispositifD3D11::SetDepthState(const bool& depthTest, const bool& depthWrite) const
{
	if(depthWrite && depthTest) {
		pImmediateContext->OMSetDepthStencilState(pDepthStencilState, 1);
	}
	else if (depthWrite && !depthTest)
	{
		pImmediateContext->OMSetDepthStencilState(pNoDepthTestDepthStencilState, 1);
	}
	else if (!depthWrite && !depthTest)
	{
		pImmediateContext->OMSetDepthStencilState(pNoDepthTestAndWriteTestDepthStencilState, 1);
	}
	else
	{
		throw "Non supporté";
	}
}

void CDispositifD3D11::InitDepthBuffer()
{
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = largeurEcran;
	depthTextureDesc.Height = hauteurEcran;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MiscFlags = 0;

	DXEssayer(pD3DDevice->CreateTexture2D(&depthTextureDesc, nullptr, &pDepthTexture), DXE_ERREURCREATIONTEXTURE);

	// Cr�ation de la vue du tampon de profondeur (ou de stencil)
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
	ZeroMemory(&descDSView, sizeof(descDSView));
	descDSView.Format = depthTextureDesc.Format;
	descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSView.Texture2D.MipSlice = 0;
	DXEssayer(pD3DDevice->CreateDepthStencilView(pDepthTexture, &descDSView, &pDepthStencilView),
		DXE_ERREURCREATIONDEPTHSTENCILTARGET);
}

void CDispositifD3D11::InitDepthState()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	pD3DDevice->CreateDepthStencilState(&dsDesc, &pDepthStencilState);

	D3D11_DEPTH_STENCIL_DESC dsDescCopy = dsDesc;
	dsDescCopy.DepthEnable = false;

	pD3DDevice->CreateDepthStencilState(&dsDescCopy, &pNoDepthTestDepthStencilState);

	D3D11_DEPTH_STENCIL_DESC dsDescCopy2 = dsDescCopy;
	dsDescCopy.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	pD3DDevice->CreateDepthStencilState(&dsDescCopy2, &pNoDepthTestAndWriteTestDepthStencilState);
}

void CDispositifD3D11::InitBlendStates()
{
	D3D11_BLEND_DESC blendDesc;
	
	// Effacer la description
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	
	// On initialise la description pour un m�lange alpha classique
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	// On cr�� l��tat alphaBlendEnable
	DXEssayer(pD3DDevice->CreateBlendState(&blendDesc, &alphaBlendEnable), DXE_ERREURCREATION_BLENDSTATE);

	// Seul le booleen BlendEnable n�cessite d��tre modifi�
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	
	// On cr�� l��tat alphaBlendDisable
	DXEssayer( pD3DDevice->CreateBlendState(&blendDesc,&alphaBlendDisable), DXE_ERREURCREATION_BLENDSTATE);
}

void CDispositifD3D11::SetViewportDimension(int largeur, int hauteur)
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(largeur);
	vp.Height = static_cast<FLOAT>(hauteur);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports( 1, &vp );
}

void CDispositifD3D11::ResetViewportDimension()
{
	D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(largeurEcran);
    vp.Height = static_cast<FLOAT>(hauteurEcran);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pImmediateContext->RSSetViewports( 1, &vp );
}

void CDispositifD3D11::SetNormalRSState()
{
	pImmediateContext->RSSetState(mSolidCullBackRS);
}

void CDispositifD3D11::SetRenderTargetView(
	ID3D11RenderTargetView* pRenderTargetView_in, ID3D11DepthStencilView* pDepthStencilView_in)
{
	pRenderTargetView = pRenderTargetView_in;
	pDepthStencilView = pDepthStencilView_in;
	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
}

void CDispositifD3D11::SetRenderTargetView(
	ID3D11RenderTargetView* pRenderTargetView_in)
{
	pRenderTargetView = pRenderTargetView_in;
	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
}

} // namespace PM3D
