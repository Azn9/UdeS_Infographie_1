#include "stdafx.h"
#include "Core/Public/Core/CDIManipulateur.h"

#include <any>

#include "Core/Public/Util/util.h"
#include "Core/Public/Util/Resource.h"

bool CDIManipulateur::bDejaInit = false;

bool CDIManipulateur::Init(HINSTANCE hInstance, HWND hWnd)
{
    // Un seul objet DirectInput est permis
    if (!bDejaInit)
    {
        // Objet DirectInput
        PM3D::DXEssayer(DirectInput8Create( hInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&pDirectInput,
        NULL), ERREUR_CREATION_DIRECTINPUT );
        // Objet Clavier
        PM3D::DXEssayer (pDirectInput ->CreateDevice( GUID_SysKeyboard,
                                                      &pClavier,
                                                      NULL),
                         ERREUR_CREATION_CLAVIER);
        PM3D::DXEssayer(pClavier->SetDataFormat( &c_dfDIKeyboard),
                        ERREUR_CREATION_FORMAT_CLAVIER);
        pClavier->SetCooperativeLevel( hWnd,
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        pClavier->Acquire();
        
        // Objet Souris
        PM3D::DXEssayer(pDirectInput ->CreateDevice( GUID_SysMouse, &pSouris, NULL),
                        ERREUR_CREATION_SOURIS);
        PM3D::DXEssayer( pSouris->SetDataFormat( &c_dfDIMouse),ERREUR_CREATION_FORMAT_SOURIS);
        pSouris->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        pSouris->Acquire();
        
        // Objet Joystick
        bDejaInit = true;
    }
    return true;
}

/*
CDIManipulateur::~CDIManipulateur()
{
    if (pClavier)
    {
        pClavier->Unacquire();
        pClavier->Release();
        pClavier = nullptr;
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
    if (pDirectInput)
    {
        pDirectInput->Release();
    }  
}
*/

void CDIManipulateur::StatutClavier()
{
    if (pClavier)
        pClavier->GetDeviceState(sizeof(tamponClavier), (void*)& tamponClavier);
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
