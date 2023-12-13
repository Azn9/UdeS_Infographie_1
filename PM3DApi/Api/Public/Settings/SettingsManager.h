#pragma once
#include "Core/Public/Util/Singleton.h"

class SettingsManager : public PM3D::CSingleton<SettingsManager>
{
public:
    SettingsManager() = default;

    void Initialize();
    void Save();
};
