#include "../../Public/Util/Util.h"

#include <codecvt>
#include <locale>

#include "Api/Public/Util/Quaternion.h"

DirectX::XMFLOAT3 Util::Lerp(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t)
{
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t,
		a.z + (b.z - a.z) * t
	};
}

DirectX::XMFLOAT2 Util::operator*(const DirectX::XMFLOAT2& a, const float& f)
{
	return {a.x * f, a.y * f};
}

DirectX::XMFLOAT2 Util::operator/(const DirectX::XMFLOAT2& a, const float& f)
{
	return {a.x / f, a.y / f};
}

DirectX::XMFLOAT2 Util::operator+(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	return {a.x + b.x, a.y + b.y};
}

DirectX::XMFLOAT2 Util::operator-(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	return {a.x - b.x, a.y - b.y};
}

DirectX::XMFLOAT3 Util::DirectionToEulerAngles(const DirectX::XMFLOAT3 direction)
{
	float x = asinf(direction.z / DirectX::XMVectorGetX(DirectX::XMVector3Length(XMLoadFloat3(&direction)))) * 180.0f / DirectX::XM_PI;
	float y = atan2f(direction.x, direction.y) * 180.0f / DirectX::XM_PI;
	float z = 0.0f;

	return {x, y, z};
}

PM3D_API::Quaternion Util::DirectionToQuaternion(const DirectX::XMFLOAT3 direction)
{
	// Normalizez la direction pour obtenir un vecteur unitaire
	const DirectX::XMVECTOR dirVector = DirectX::XMVector3Normalize(XMLoadFloat3(&direction));

	// Créez un quaternion à partir de l'axe (direction) et de l'angle nul (0)
	const DirectX::XMVECTOR quatVector = DirectX::XMQuaternionRotationAxis(dirVector, 0.0f);

	// Convertissez le quaternion en XMFLOAT4
	PM3D_API::Quaternion quaternion;
	DirectX::XMStoreFloat4(&quaternion, quatVector);

	return quaternion;
}

std::string Util::ws2s(const std::wstring& wstr)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(wstr);
}
