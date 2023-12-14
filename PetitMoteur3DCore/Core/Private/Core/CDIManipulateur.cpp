#include "Core/Public/Core/CDIManipulateur.h"

#include <any>
#include <iostream>
#include <thread>

#include "Core/Public/Util/util.h"
#include "Core/Public/Util/Resource.h"

bool CDIManipulateur::bDejaInit = false;

void CDIManipulateur::Destroy()
{
    if (pClavier)
    {
        pClavier->Unacquire();
        pClavier->Release();
        pDirectInput = nullptr;
    }
    if (pSouris)
    {
        pSouris->Unacquire();
        pSouris->Release();
        pSouris = nullptr;
    }
    if (pJoystick)
    {
        pJoystick->Unacquire();
        pJoystick->Release();
        pJoystick = nullptr;
    }
}

void CDIManipulateur::AcquireFocus()
{
    if (pSouris)
        if (pSouris->Acquire() != S_OK)
            std::cerr << "Erreur lors de l'acquisition de la souris !" << std::endl;

    if (pClavier)
        if (pClavier->Acquire() != S_OK)
            std::cerr << "Erreur lors de l'acquisition du clavier !" << std::endl;

    auto t = std::thread([&]()
    {
        while (!pClavier || pClavier->Acquire() != S_OK)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    t.detach();
}

bool CDIManipulateur::Init(HINSTANCE hInstance, HWND hWnd)
{
    // Un seul objet DirectInput est permis
    if (bDejaInit)
    {
        return true;
    }

    // Objet DirectInput
    PM3D::DXEssayer(DirectInput8Create(hInstance,
                                       DIRECTINPUT_VERSION,
                                       IID_IDirectInput8,
                                       (void**)&pDirectInput,
                                       NULL), ERREUR_CREATION_DIRECTINPUT);

    // Objet Clavier
    PM3D::DXEssayer(pDirectInput->CreateDevice(GUID_SysKeyboard,
                                               &pClavier,
                                               NULL),
                    ERREUR_CREATION_CLAVIER);
    PM3D::DXEssayer(pClavier->SetDataFormat(&c_dfDIKeyboard),
                    ERREUR_CREATION_FORMAT_CLAVIER);
    PM3D::DXEssayer(pClavier->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    auto res = pClavier->Acquire();

    if (res != S_OK)
    {
        std::cerr << "Erreur lors de l'acquisition du clavier" << std::endl;
    }

    // Objet Souris
    PM3D::DXEssayer(pDirectInput->CreateDevice(GUID_SysMouse, &pSouris, NULL),
                    ERREUR_CREATION_SOURIS);
    PM3D::DXEssayer(pSouris->SetDataFormat(&c_dfDIMouse),ERREUR_CREATION_FORMAT_SOURIS);
    PM3D::DXEssayer(pSouris->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
    res = pSouris->Acquire();

    if (res != S_OK)
    {
        std::cerr << "Erreur lors de l'acquisition du clavier" << std::endl;
    }

    // Objet Joystick

    bDejaInit = true;

    return true;
}

CDIManipulateur::~CDIManipulateur()
{
    Destroy();

    if (pDirectInput)
    {
        pDirectInput->Release();
        pClavier = nullptr;
        pSouris = nullptr;
        pDirectInput = nullptr;
    }
}


void CDIManipulateur::StatutClavier()
{
    if (pClavier)
        pClavier->GetDeviceState(sizeof(tamponClavier), (void*)&tamponClavier);
}

bool CDIManipulateur::ToucheAppuyee(UINT touche)
{
    return (tamponClavier[touche] & 0x80);
}

void CDIManipulateur::SaisirEtatSouris()
{
    if (pSouris)
        pSouris->GetDeviceState(sizeof(mouseState), (void*)&mouseState);
}
