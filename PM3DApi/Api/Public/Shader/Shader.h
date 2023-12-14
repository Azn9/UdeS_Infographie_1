#pragma once

#include <DirectXMath.h>
#include <string>

#include "Core/Public/Shader/d3dx11effect.h"
#include "Api/Public/GameHost.h"

namespace PM3D_API
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        [[nodiscard]] virtual ID3D11Buffer* GetShaderParametersBuffer() const = 0;
        [[nodiscard]] virtual ID3DX11EffectPass* GetPass() const = 0;
        [[nodiscard]] virtual ID3D11InputLayout* GetVertexLayout() const = 0;
        [[nodiscard]] virtual ID3D11Buffer* GetIndexBuffer() const = 0;

        [[nodiscard]] virtual ID3D11InputLayout* GetShadowVertexLayout() const = 0;
        [[nodiscard]] virtual ID3D11Texture2D* GetDepthTexture() const = 0;
        [[nodiscard]] virtual ID3D11DepthStencilView* GetDepthStencilView() const = 0;
        [[nodiscard]] virtual ID3D11ShaderResourceView* GetDepthShaderResourceView() const = 0;

        [[nodiscard]] virtual ID3DX11Effect* GetEffect() const = 0;

        [[nodiscard]] virtual ID3D11Buffer** GetVertexBufferPtr() const = 0;
        [[nodiscard]] virtual ID3D11Buffer** GetIndexBufferPtr() const = 0;

        virtual void* PrepareParameters(
            DirectX::XMMATRIX matWorldViewProj,
            DirectX::XMMATRIX matWorld
        ) = 0;
        virtual void ApplyMaterialParameters(
            void* shaderParameters,
            DirectX::XMVECTOR materialAmbiant,
            DirectX::XMVECTOR materialDiffuse,
            DirectX::XMVECTOR materialSpecular,
            float specularPower,
            ID3D11ShaderResourceView* albedoTexture,
            ID3D11ShaderResourceView* normalmapTexture
        ) = 0;

        virtual void DeleteParameters(void* shader_parameters) = 0;

        virtual void ApplyShaderParams() const = 0;

        virtual void LoadLights(ID3D11DeviceContext* context, GameObject* gameObject) = 0;

        virtual ID3D11Buffer* GetVertexBuffer() = 0;
        virtual ID3D11Buffer* GetInstanceBuffer() = 0;
        virtual ID3D11Buffer** GetInstanceBufferPtr() = 0;
    };
}
