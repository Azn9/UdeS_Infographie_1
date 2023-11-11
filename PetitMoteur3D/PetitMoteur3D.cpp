// PetitMoteur3D.cpp�: d�finit le point d'entr�e pour l'application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include "Core/Public/Core/PetitMoteur3D.h"
#include "Core/Public/Core/MoteurWindows.h"


#include "Api/Public/GameHost.h"

using namespace PM3D;
using namespace PM3D_API;

int APIENTRY _tWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	// Pour ne pas avoir d'avertissement
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	
	std::cout << "A" << std::endl;

	const auto gameHost = GameHost::GetInstance();
	gameHost->Initialize();

	std::cout << "B" << std::endl;

	try
	{
		// Cr�ation de l'objet Moteur
		CMoteurWindows& rMoteur = CMoteurWindows::GetInstance();

		std::cout << "C" << std::endl;
		
		rMoteur.SetGameHost(gameHost);

		std::cout << "D" << std::endl;
		
		// Sp�cifiques � une application Windows
		rMoteur.SetWindowsAppInstance(hInstance);

		std::cout << "E" << std::endl;

		// Initialisation du moteur
		rMoteur.Initialisations();

		std::cout << "F" << std::endl;

		// Boucle d'application
		rMoteur.Run();

		std::cout << "G" << std::endl;

		return (int)1;
	}

	catch (const std::exception& E)
	{
		const int BufferSize = 128;
		wchar_t message[BufferSize];

		size_t numCharacterConverted;
		mbstowcs_s(&numCharacterConverted, message, E.what(), BufferSize - 1);
		::MessageBox(nullptr, message, L"Erreur", MB_ICONWARNING);

		return (int)99;
	}

	catch (int codeErreur)
	{
		wchar_t szErrMsg[MAX_LOADSTRING]; // Un message d'erreur selon le code

		::LoadString(hInstance, codeErreur, szErrMsg, MAX_LOADSTRING);
		::MessageBox(nullptr, szErrMsg, L"Erreur", MB_ICONWARNING);

		return (int)99; // POURQUOI 99???
	}

}
