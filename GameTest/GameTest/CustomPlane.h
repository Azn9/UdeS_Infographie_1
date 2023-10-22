#pragma once
#include "../../PM3DApi/Api/Public/GameObject/Basic/BasicShape.h"

class CustomPlane : public PM3D_API::BasicShape
{
public:
    CustomPlane() : BasicShape("TestPlane") {}

private:
    std::wstring GetShaderFileName() override { return L"Test1.fx"; }
    std::string GetMeshFileName() override { return "testplane.obj"; }
};