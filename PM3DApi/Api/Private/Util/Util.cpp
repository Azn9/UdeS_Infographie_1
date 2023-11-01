#include "../../Public/Util/Util.h"

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
	DirectX::XMFLOAT3 angles;

	// Calculez les angles d'Euler en fonction de la direction avec Y vers le haut
	// Supposons que la direction représente la direction "forward" de l'objet.

	// Calcul de l'angle de tangage (pitch)
	const auto xRad = atan2f(-direction.y, sqrtf(direction.x * direction.x + direction.z * direction.z));
	angles.x = xRad * 180.0f / DirectX::XM_PI;

	// Calcul de l'angle de lacet (yaw)
	angles.y = atan2f(direction.x, direction.z) * 180.0f / DirectX::XM_PI;

	// L'objet ne roule pas, donc l'angle de roulis (roll) est généralement défini à 0.0f
	angles.z = 0.0f;

	return angles;
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
