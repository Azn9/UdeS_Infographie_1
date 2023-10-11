#pragma once

class CSommetMesh
{
public:
	CSommetMesh()
	{
	};
	CSommetMesh(DirectX::XMFLOAT3 _position,
				DirectX::XMFLOAT3 _normal,
				DirectX::XMFLOAT2 _coordTex = DirectX::XMFLOAT2(0, 0));

public:
	static UINT numElements;
	static D3D11_INPUT_ELEMENT_DESC layout[];
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 coordTex;
};

// Definir l’organisation de notre sommet
D3D11_INPUT_ELEMENT_DESC CSommetMesh::layout[] =
{
	{
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0
	},
	{
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0
	},
	{
		"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
		D3D11_INPUT_PER_VERTEX_DATA, 0
	}
};
UINT CSommetMesh::numElements;
