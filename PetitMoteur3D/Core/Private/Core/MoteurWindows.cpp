#include "StdAfx.h"
#include <thread>
#include <string>
#include "Core/Public/Util/resource.h"

#include "Core/Public/Core/MoteurWindows.h"

#include "../../../../PM3DApi/Api/Public/EventSystem/EventSystem.h"
#include "../../../../PM3DApi/Api/Public/EventSystem/Basic/WindowResizeEvent.h"
#include "../../Imgui/imgui.h"
#include "../../Imgui/imgui_impl_win32.h"
#include "../../Imgui/imgui_impl_dx11.h"
#include "Core/Public/Util/util.h"

namespace PM3D
{
HINSTANCE CMoteurWindows::hAppInstance; // handle Windows de l'instance actuelle de l'application

//   FONCTION : SetWindowsAppInstance(HANDLE, int)
//
//   BUT : Prend en note le handle de l'instance
//
void CMoteurWindows::SetWindowsAppInstance(HINSTANCE hInstance)
{
	hAppInstance = hInstance; // Stocke le handle d'instance de l'application, plusieurs fonctions spécifiques en auront besoin
}

//
//  FONCTION : MyRegisterClass()
//
//  BUT : inscrit la classe de fenêtre.
//
//  COMMENTAIRES :
//
//    Cette fonction et son utilisation sont nécessaires uniquement si vous souhaitez que ce code
//    soit compatible avec les systèmes Win32 avant la fonction 'RegisterClassEx'
//    qui a été ajoutée à Windows 95. Il est important d'appeler cette fonction
//    afin que l'application dispose des petites icônes correctes qui lui sont
//    associées.
//
//	  FJ:  Copie-coller de la fonction MyRegisterClass générée automatiquement
//
ATOM CMoteurWindows::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PETITMOTEUR3D));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_PETITMOTEUR3D);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//   FONCTION : InitAppInstance()
//
//   BUT :  Effectue des opérations d'initialisation de l'application
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        créons et affichons la fenêtre principale du programme.
//
bool CMoteurWindows::InitAppInstance()
{
	TCHAR szTitle[MAX_LOADSTRING]; // Le texte de la barre de titre

	// Initialise les chaînes globales
	LoadString(hAppInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hAppInstance, IDC_PETITMOTEUR3D, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hAppInstance);

	hMainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hAppInstance, nullptr);

	if (!hMainWnd)
	{
		return false;
	}

	hAccelTable = LoadAccelerators(hAppInstance, MAKEINTRESOURCE(IDC_PETITMOTEUR3D));

	return true;
}

int CMoteurWindows::Show()
{
	ShowWindow(hMainWnd, SW_SHOWNORMAL);
	UpdateWindow(hMainWnd);

	return 0;
}

int CMoteurWindows::InitialisationsSpecific()
{
	// Initialisations  de l'application;
	InitAppInstance();
	Show();

	// Initialisation de DirectInput
	GestionnaireDeSaisie.Init(hAppInstance, hMainWnd);

	return 0;
}

void CMoteurWindows::InitSceneSpecific()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hMainWnd);
	ImGui_ImplDX11_Init(pDispositif->GetD3DDevice(), pDispositif->GetImmediateContext());
}

//
//   FONCTION : RunSpecific 
//
//   BUT :	Éléments internes de la boucle de l'application (Boucle message)
//
//   NOTES:
//      Il s'agit d'une version modifiée spécifiquement pour nos besoins des
//      éléments de la boucle message de Windows
//
bool CMoteurWindows::RunSpecific()
{
	MSG msg;
	bool bBoucle = true;

	// Y-a-t'il un message Windows à traiter?
	if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// Est-ce un message de fermeture ?
		if (msg.message == WM_QUIT) bBoucle = false;

		// distribuer le message
		if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	return bBoucle;
}

//  FONCTION : CreationDispositifSpecific 
//
//  BUT :	Fonction responsable de créer le 
//			dispositif selon certains paramètres
//  NOTES:		
//		 CDS_MODE: 	CDS_FENETRE 		application fenêtrée
//					CDS_PLEIN_ECRAN 	application plein écran
//
CDispositifD3D11* CMoteurWindows::CreationDispositifSpecific(const CDS_MODE cdsMode, UINT largeur, UINT hauteur)
{
	return new CDispositifD3D11(cdsMode, hMainWnd, largeur, hauteur);
}
CDispositifD3D11* CMoteurWindows::CreationDispositifSpecific(const CDS_MODE cdsMode)
{
	return new CDispositifD3D11(cdsMode, hMainWnd);
}

void CMoteurWindows::BeginRenderSceneSpecific()
{
	if (!canRender) return;
	
	ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
	ID3D11RenderTargetView* pRenderTargetView = pDispositif->GetRenderTargetView();

	// On efface la surface de rendu
	const float Couleur[4] = {0.0f, 0.0f, 0.2f, 1.0f}; //  RGBA - Vert pour le moment

	pImmediateContext->ClearRenderTargetView(pRenderTargetView, Couleur);
	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	//ImGui::DockSpaceOverViewport();

	// On ré-initialise le tampon de profondeur
	ID3D11DepthStencilView* pDepthStencilView = pDispositif->GetDepthStencilView();
	if (pDepthStencilView)
		pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CMoteurWindows::EndRenderSceneSpecific()
{
	if (!canRender) return;
	
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CMoteurWindows::Resize(WORD largeur, WORD hauteur)
{
	if (!pDispositif) return;

	std::cout << "Resize: " << largeur << "x" << hauteur << std::endl;

	canRender = false;

	ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
	pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	ID3D11RenderTargetView** pRenderTargetView = pDispositif->GetRenderTargetViewPtr();
	(*pRenderTargetView)->Release();

	DXEssayer(pDispositif->GetSwapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));
//ResizeBuffers(1, largeur, hauteur, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ID3D11Texture2D* pBuffer;
	DXEssayer(pDispositif->GetSwapChain()->GetBuffer(0, __uuidof( ID3D11Texture2D), (void**) &pBuffer ));
	DXEssayer(pDispositif->GetD3DDevice()->CreateRenderTargetView(pBuffer, nullptr, pRenderTargetView));

	pBuffer->Release();

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)largeur;
	vp.Height = (FLOAT)hauteur;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1, &vp);
	
	pDispositif->GetDepthStencilState()->Release();
	pDispositif->GetNoDepthStencilState()->Release();
	pDispositif->GetDepthStencilView()->Release();

	pDispositif->largeurEcran = largeur;
	pDispositif->hauteurEcran = hauteur;

	pDispositif->InitDepthBuffer();
	pDispositif->InitDepthState();

	pDispositif->ActiverDepth();
	//pDispositif->DesactiverDepth();

	pDispositif->InitBlendStates();

	pImmediateContext->RSSetState(pDispositif->GetRasterizerState());
	
	pImmediateContext->OMSetRenderTargets(1, pRenderTargetView, pDispositif->GetDepthStencilView());

	/*
	// Update the camera
	if (gameHost)
	{
		const auto scene = gameHost->GetScene();
		if (scene)
		{
			const auto camera = scene->GetMainCamera();
			if (camera && camera->IsInitialized()) 
			{
				//camera->UpdateInternalMatrices();
			}
		}
	}
	*/

	PM3D_API::EventSystem::Publish(new PM3D_API::WindowResizeEvent(largeur, hauteur));

	canRender = true;
}

void CMoteurWindows::ResizeWindow(int largeur, int hauteur)
{
	SetWindowPos(hMainWnd, NULL, 0, 0, largeur, hauteur, SWP_NOMOVE);
}

//  FONCTION : WndProc(HWND, unsigned, WORD, LONG)
//
//  BUT :  traite les messages pour la fenêtre principale.
//
//  WM_COMMAND	- traite le menu de l'application
//  WM_PAINT	- dessine la fenêtre principale
//  WM_DESTROY	- génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK CMoteurWindows::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
	
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
	// Analyse les sélections de menu :
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hAppInstance, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
	// Évitez d'ajouter du code ici...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			break;
		std::cout << "== RESIZE " << LOWORD(lParam) << "x" << HIWORD(lParam) << std::endl;
		CMoteurWindows::GetInstance().Resize(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK CMoteurWindows::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void CMoteurWindows::SetThreadName(std::thread& thread, const std::string& name)
{
	const std::wstring wName(name.begin(), name.end());
	if (thread.native_handle() != nullptr) {
		SetThreadDescription(thread.native_handle(), wName.c_str());
	}
}
} // namespace PM3D
