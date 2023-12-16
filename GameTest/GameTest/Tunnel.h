#pragma once
#include "Api/Public/GameObject/Basic/BasicShape.h"

class Tunnel : public PM3D_API::BasicShape
{
public:
    Tunnel() : BasicShape("tunnel")
    {
    }

private:
    std::wstring GetShaderFileName() override { return L"shader/NewShader.fx"; }
    std::string GetMeshFileName() override { return "tunnel.obj"; }
};
