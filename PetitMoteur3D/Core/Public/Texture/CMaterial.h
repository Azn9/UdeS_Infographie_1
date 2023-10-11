#pragma once

class CMaterial
{
public:
	std::string NomFichierTexture;
	std::string NomMateriau;
	ID3D11ShaderResourceView* pTextureD3D;
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
		Puissance = 0;
		pTextureD3D = nullptr;
		NomFichierTexture = "";
	}
};