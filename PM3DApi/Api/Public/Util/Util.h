#pragma once
#include <DirectXMath.h>
#include <type_traits>

class Util
{
public:
	static DirectX::XMFLOAT3 Lerp(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t);
	static DirectX::XMFLOAT3 QuaternionToEuler(DirectX::XMFLOAT4 xmfloat4);

};

template <typename O, typename T>
struct IsDerivedFromHost {
	static constexpr bool value = std::is_base_of<O, T>::value;
};
