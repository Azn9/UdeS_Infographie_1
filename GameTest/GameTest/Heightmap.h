#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"

class Heightmap : public PM3D_API::BasicShape
{
public:
    Heightmap() : BasicShape("heightMap") {}

private:
    std::wstring GetShaderFileName() override { return L"shader/NewShader.fx"; }
    std::string GetMeshFileName() override { return "Terrain2LOD0.obj"; }
};
