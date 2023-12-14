#pragma once
#include "Moteur.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace PM3D
{
#define MAX_LOADSTRING 100

    class CMoteurWindows final : public CMoteur
    {
    public:
        CMoteurWindows() = default;
        void SetWindowsAppInstance(HINSTANCE hInstance);

        static CMoteurWindows& GetInstance()
        {
            static CMoteurWindows instance;
            return instance;
        }

        //double GetLastFrameTime() const override
        //{
        //	return lastFrameTime;
        //}

        void SetCursorDefault() override;
        void SetCursorHidden() override;
        void SetCursorDisabled() override;
        void SetCursorClick() override;

    private:
        ATOM MyRegisterClass(HINSTANCE hInstance);
        bool InitAppInstance();
        int Show();

        // Les fonctions spécifiques
        virtual int InitialisationsSpecific() override;
        virtual bool RunSpecific() override;
        virtual CDispositifD3D11* CreationDispositifSpecific(const CDS_MODE cdsMode) override;
        virtual CDispositifD3D11* CreationDispositifSpecific(const CDS_MODE cdsMode, UINT largeur, UINT hauteur);
        virtual void InitSceneSpecific() override;
        virtual void BeginRenderSceneSpecific() override;
        virtual void EndRenderSceneSpecific() override;
		void BeginRenderDebug() override;
		void EndRenderDebug() override;

        void Resize(WORD largeur, WORD hauteur) override;
        void ResizeWindow(int largeur, int hauteur) override;

        // Fonctions "Callback" -- Doivent être statiques
        static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
        static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

        HACCEL hAccelTable; // handle Windows de la table des accélérateurs
        static HINSTANCE hAppInstance; // handle Windows de l'instance actuelle de l'application
        HWND hMainWnd; // handle Windows de la fenêtre principale
        TCHAR szWindowClass[MAX_LOADSTRING]; // le nom de la classe de fenêtre principale

        void SetThreadName(std::thread& thread, const std::string& name) override;
    };
} // namespace PM3D
