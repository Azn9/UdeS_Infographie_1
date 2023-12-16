#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"

class Left_Ski : public PM3D_API::BasicShape
{
public:
    Left_Ski() : BasicShape("ski")
    {
    }

private:
    std::wstring GetShaderFileName() override { return L"shader/NewShader.fx"; }
    std::string GetMeshFileName() override { return "Left_ski.obj"; }
};
