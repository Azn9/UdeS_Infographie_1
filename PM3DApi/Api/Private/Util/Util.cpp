#include "../../Public/Util/Util.h"
#include <valarray>
#include "../../../../PetitMoteur3D/Core/Imgui/imgui.h"


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

float Util::magnitude(const DirectX::XMFLOAT2& f2)
{
	return std::sqrt(f2.x * f2.x + f2.y * f2.y);
}

float Util::magnitude(const DirectX::XMFLOAT3& f3)
{
	return std::sqrt(f3.x * f3.x + f3.y * f3.y + f3.z * f3.z);
}


Util::Plane::Plane(const DirectX::XMVECTOR& _normal, const DirectX::XMVECTOR& point)
	: normal(DirectX::XMVector3Normalize(_normal))
{
	dist = DirectX::XMVector3Dot(point, normal).m128_f32[0];
}

float Util::Plane::SignedDistanceFromPlane(const DirectX::XMVECTOR& point) const
{
	return DirectX::XMVector3Dot(point, normal).m128_f32[0] - dist;
}

bool Util::Plane::IsSphereInFrontOfPlane(const DirectX::XMVECTOR& point, const float& radius) const
{
	float result = SignedDistanceFromPlane(point);
	return  result > radius + 0.01f;
}

void Util::DrawDebugVector3(const std::string name, const DirectX::XMVECTOR& vec)
{
	ImGui::Text(name.c_str());
	ImGui::SameLine(100); ImGui::Text(("x=" + std::to_string(vec.m128_f32[0])).c_str());
	ImGui::SameLine(200); ImGui::Text(("y=" + std::to_string(vec.m128_f32[1])).c_str());
	ImGui::SameLine(300); ImGui::Text(("z=" + std::to_string(vec.m128_f32[2])).c_str());
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
