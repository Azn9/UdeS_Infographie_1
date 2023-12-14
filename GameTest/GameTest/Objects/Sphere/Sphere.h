#pragma once
#include "Api/Public/GameObject/GameObject.h"

class Sphere final : public PM3D_API::GameObject
{
public:
    Sphere() : GameObject("Sphere")
    {
    }

    void Initialize() override;
};
