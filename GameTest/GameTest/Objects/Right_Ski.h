#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"

class Right_Ski : public PM3D_API::BasicShape
{
public:
    Right_Ski() : BasicShape("ski")
    {
    }

private:
    std::wstring GetShaderFileName() override { return L"shader/NewShader.fx"; }
    std::string GetMeshFileName() override { return "Right_ski.obj"; }
};
