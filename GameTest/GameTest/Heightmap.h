#pragma once
#include "../../PM3DApi/Api/Public/GameObject/Basic/BasicShape.h"

class Heightmap : public PM3D_API::BasicShape
{
public:
    Heightmap() : BasicShape("heightMap") {}

private:
    std::wstring GetShaderFileName() override { return L"NewShader.fx"; }
    std::string GetMeshFileName() override { return "heightMap.obj"; }
};
