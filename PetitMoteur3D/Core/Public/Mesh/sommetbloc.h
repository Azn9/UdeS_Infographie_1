#pragma once

#include <DirectXMath.h>

namespace PM3D
{

class CSommetBloc
{
public:
	CSommetBloc() = default;
	CSommetBloc(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT2& coordTex = DirectX::XMFLOAT2(0.0f, 0.0f));

public:
	static UINT numElements;
	static D3D11_INPUT_ELEMENT_DESC layout[];

protected:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Normal;
	DirectX::XMFLOAT2 m_CoordTex;
};

} // namespace PM3D
