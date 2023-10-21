#pragma once
#include <d3d11.h>

#include "../../../../../PetitMoteur3D/Core/Public/Mesh/sommetbloc.h"
#include "../../../../../PM3DApi/Api/Public/GameObject/GameObject.h"
#include "../../../../../PM3DApi/Api/Public/Shader/Basic/DefaultShader.h"

class Plane final : public PM3D_API::GameObject
{
public:
	Plane(
		const std::string& name,
		const DirectX::XMFLOAT3 worldPosition,
		const DirectX::XMFLOAT3 worldRotation,
		const DirectX::XMFLOAT3 worldScale
	) : GameObject(
		name,
		worldPosition,
		worldRotation,
		worldScale
	) {}

	void Initialize() override;
	void DrawSelf() const override;

protected:

private:
	DirectX::XMFLOAT3* points;
	DirectX::XMFLOAT3 normal;
	PM3D::CSommetBloc* sommets;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	std::unique_ptr<PM3D_API::DefaultShader> shader;

	uint16_t index_bloc[6] = {
		1, 2, 3,
		1, 3, 0
	};
};
