#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"
#include "Api/Public/Util/MapImporter/MapImporter.h"

class Map : public PM3D_API::GameObject
{
public:
    Map();
    void Initialize() override;

private:
    PM3D_API::MapImporter mapImporter;
};
