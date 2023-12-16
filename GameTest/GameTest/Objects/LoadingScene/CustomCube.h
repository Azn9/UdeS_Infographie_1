#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"

class CustomCube : public PM3D_API::BasicShape
{
public:
    CustomCube() : BasicShape("TestCube")
    {
    }

private:
    std::wstring GetShaderFileName() override { return L"shader/NewShader.fx"; }
    std::string GetMeshFileName() override { return "DefaultObj/sphere.obj"; }
};
