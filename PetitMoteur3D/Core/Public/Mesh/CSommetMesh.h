#pragma once

class CSommetMesh
{
public:
    CSommetMesh() = default;
    
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT3 binormal;
    DirectX::XMFLOAT3 tangent;
    DirectX::XMFLOAT2 coordTex;
};