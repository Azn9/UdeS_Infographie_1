#pragma once
#include "Core/Public/Util/Singleton.h"

class Settings : PM3D::CSingleton<Settings>
{
public:
    Settings() = default;

    void Load();
};
