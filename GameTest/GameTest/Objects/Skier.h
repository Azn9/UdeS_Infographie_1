#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"

class Skier : public PM3D_API::BasicShape
{
public:
    Skier() : BasicShape("skier") {}

private:
    std::wstring GetShaderFileName() override { return L"shader/NewShader.fx"; }
    std::string GetMeshFileName() override { return "skier.obj"; }
};
