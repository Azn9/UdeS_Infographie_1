#pragma once
#include "../../PM3DApi/Api/Public/GameObject/Basic/BasicShape.h"

class Heightmap : public PM3D_API::BasicShape
{
public:
    Heightmap() : BasicShape("corps") {}

private:
    std::wstring GetShaderFileName() override { return L"Test1.fx"; }
    std::string GetMeshFileName() override { return "corps.obj"; }
};
