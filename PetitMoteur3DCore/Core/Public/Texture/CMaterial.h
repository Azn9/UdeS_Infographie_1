#pragma once
#include <DirectXMath.h>
#include <string>
#include <d3d11.h>


class CMaterial
{
public:
    std::string albedoTextureFileName;
    std::string normalmapTextureFileName;
    std::string NomMateriau;
    ID3D11ShaderResourceView* pAlbedoTexture;
    ID3D11ShaderResourceView* pNormalmapTexture;
    DirectX::XMFLOAT4 Ambient;
    DirectX::XMFLOAT4 Diffuse;
    DirectX::XMFLOAT4 Specular;
    float Puissance;
    bool transparent;

    CMaterial()
    {
        Ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        Specular = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
        transparent = false;
        Puissance = 250;
        pAlbedoTexture = nullptr;
        albedoTextureFileName = "";
        pNormalmapTexture = nullptr;
        normalmapTextureFileName = "";
    }
};
