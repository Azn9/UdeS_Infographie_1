#pragma once
#include <DirectXMath.h>

#include "Core/Public/Sprite/AfficheurSprite.h"

class CPanneau : public PM3D::CAfficheurSprite::CSprite
{
public:
    XMFLOAT3 position;
    XMFLOAT2 dimension;

    CPanneau()
    {
        bPanneau = true;
    }
};
