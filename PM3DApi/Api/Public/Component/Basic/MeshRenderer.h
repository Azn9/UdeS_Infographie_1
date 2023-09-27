#pragma once

#include "../../Component/Component.h"

#include <string>

#define FAST_OBJ_IMPLEMENTATION
#include "../../Util/fast_obj.h"

#include <d3d11.h>

#include "../../../../../PetitMoteur3D/Core/Public/Shader/d3dx11effect.h"

namespace PM3D_API
{
class MeshRenderer final : public Component
{
public:
	MeshRenderer(std::string meshName);
	MeshRenderer(fastObjMesh* mesh);

	~MeshRenderer() override;

	void Initialize() override;
	void DrawSelf() const override;

private:
	fastObjMesh* mesh;

	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3D11SamplerState* pSampleState;
	ID3D11Buffer* pConstantBuffer;
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique;
	ID3DX11EffectPass* pPasse;
	ID3D11InputLayout* pVertexLayout;

};
}
