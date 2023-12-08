#include <thread>
#include <string>
#include "Core/Public/Util/resource.h"

#include "Core/Public/Core/MoteurWindows.h"

#include "Api/Public/EventSystem/EventSystem.h"
#include "Api/Public/EventSystem/Basic/WindowResizeEvent.h"
#include "../../Imgui/imgui.h"
#include "../../Imgui/imgui_impl_win32.h"
#include "../../Imgui/imgui_impl_dx11.h"
#include "Core/Public/Util/util.h"

namespace PM3D
{
HINSTANCE CMoteurWindows::hAppInstance; // handle Windows de l'instance actuelle de l'application

//   FONCTION�: SetWindowsAppInstance(HANDLE, int)
//
//   BUT�: Prend en note le handle de l'instance
//
void CMoteurWindows::SetWindowsAppInstance(HINSTANCE hInstance)
{
	hAppInstance = hInstance; // Stocke le handle d'instance de l'application, plusieurs fonctions sp�cifiques en auront besoin
}

//
//  FONCTION�: MyRegisterClass()
//
//  BUT�: inscrit la classe de fen�tre.
//
//  COMMENTAIRES�:
//
//    Cette fonction et son utilisation sont n�cessaires uniquement si vous souhaitez que ce code
//    soit compatible avec les syst�mes Win32 avant la fonction 'RegisterClassEx'
//    qui a �t� ajout�e � Windows�95. Il est important d'appeler cette fonction
//    afin que l'application dispose des petites ic�nes correctes qui lui sont
//    associ�es.
//
//	  FJ:  Copie-coller de la fonction MyRegisterClass g�n�r�e automatiquement
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

//   FONCTION�: InitAppInstance()
//
//   BUT�:  Effectue des op�rations d'initialisation de l'application
//
//   COMMENTAIRES�:
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        cr�ons et affichons la fen�tre principale du programme.
//
bool CMoteurWindows::InitAppInstance()
{
	TCHAR szTitle[MAX_LOADSTRING]; // Le texte de la barre de titre

	// Initialise les cha�nes globales
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
//   BUT :	�l�ments internes de la boucle de l'application (Boucle message)
//
//   NOTES:
//      Il s'agit d'une version modifi�e sp�cifiquement pour nos besoins des
//      �l�ments de la boucle message de Windows
//
bool CMoteurWindows::RunSpecific()
{
	MSG msg;
	bool bBoucle = true;

	// Y-a-t'il un message Windows � traiter?
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
//  BUT :	Fonction responsable de cr�er le 
//			dispositif selon certains param�tres
//  NOTES:		
//		 CDS_MODE: 	CDS_FENETRE 		application fen�tr�e
//					CDS_PLEIN_ECRAN 	application plein �cran
//
CDispositifD3D11* CMoteurWindows::CreationDispositifSpecific(const CDS_MODE cdsMode, UINT largeur, UINT hauteur)
{
	return new CDispositifD3D11(cdsMode, hMainWnd, largeur, hauteur);
}
CDispositifD3D11* CMoteurWindows::CreationDispositifSpecific(const CDS_MODE cdsMode)
{
	std::cout << "3A" << std::endl;
	return new CDispositifD3D11(cdsMode, hMainWnd);
}

void CMoteurWindows::BeginRenderSceneSpecific()
{
	if (!canRender) return;
	
	ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
	ID3D11RenderTargetView* pRenderTargetView = pDispositif->GetRenderTargetView();

	// On efface la surface de rendu
	float Couleur[4] = {0.0f, 0.0f, 0.2f, 1.0f}; //  RGBA - Vert pour le moment

	if (const auto scene = PM3D_API::GameHost::GetInstance()->GetScene())
	{
		if (const auto camera = scene->GetMainCamera())
		{
			const auto clearColor = camera->GetClearColor();
			Couleur[0] = clearColor.x;
			Couleur[1] = clearColor.y;
			Couleur[2] = clearColor.z;
		}
	}

	pImmediateContext->ClearRenderTargetView(pRenderTargetView, Couleur);
	
	//ImGui::DockSpaceOverViewport();

	// On r�-initialise le tampon de profondeur
	if (ID3D11DepthStencilView* pDepthStencilView = pDispositif->GetDepthStencilView())
		pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CMoteurWindows::EndRenderSceneSpecific()
{
	if (!canRender) return;

/*#ifdef _DEBUG
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif*/
}

void CMoteurWindows::BeginRenderDebug()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CMoteurWindows::EndRenderDebug()
{
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

	pDispositif->SetDepthState(true, true);
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

	PM3D_API::EventSystem::Publish(PM3D_API::WindowResizeEvent(largeur, hauteur));

	canRender = true;
}

void CMoteurWindows::ResizeWindow(int largeur, int hauteur)
{
	SetWindowPos(hMainWnd, NULL, 0, 0, largeur, hauteur, SWP_NOMOVE);
}

//  FONCTION�: WndProc(HWND, unsigned, WORD, LONG)
//
//  BUT�:  traite les messages pour la fen�tre principale.
//
//  WM_COMMAND	- traite le menu de l'application
//  WM_PAINT	- dessine la fen�tre principale
//  WM_DESTROY	- g�n�re un message d'arr�t et retourne
//
//
LRESULT CALLBACK CMoteurWindows::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
#endif
	
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
	// Analyse les s�lections de menu�:
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
	// �vitez d'ajouter du code ici...
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
	case WM_SETFOCUS:
		GetInstance().GestionnaireDeSaisie.AcquireFocus();
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Gestionnaire de messages pour la bo�te de dialogue � propos de.
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
