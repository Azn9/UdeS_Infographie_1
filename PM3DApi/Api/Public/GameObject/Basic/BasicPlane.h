#pragma once
#include "BasicShape.h"
#include "../../../Public/Component/Basic/MeshRenderer.h"
#include "../../../Public/GameObject/GameObject.h"
#include "../../../Public/Shader/Shader.h"

class BasicPlane : public BasicShape
{
public:
    explicit BasicPlane(const std::string& name)
        : BasicShape(name)
    {
    }

    BasicPlane(const std::string& name, const DirectX::XMFLOAT3& worldPosition)
        : BasicShape(name, worldPosition)
    {
    }

    BasicPlane(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation)
        : BasicShape(name, worldPosition, worldRotation)
    {
    }

    BasicPlane(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
        : BasicShape(name, worldPosition, worldRotation)
    {
    }

    BasicPlane(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation,
        const DirectX::XMFLOAT3& worldScale)
        : BasicShape(name, worldPosition, worldRotation, worldScale)
    {
    }

    BasicPlane(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation,
        const DirectX::XMFLOAT3& worldScale)
        : BasicShape(name, worldPosition, worldRotation, worldScale)
    {
    }

    BasicPlane(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader)
        : BasicShape(name, std::move(shader))
    {
    }

    BasicPlane(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition)
        : BasicShape(name, std::move(shader), worldPosition)
    {
    }

    BasicPlane(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation)
        : BasicShape(name, std::move(shader), worldPosition, worldRotation)
    {
    }

    BasicPlane(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
        : BasicShape(name, std::move(shader), worldPosition, worldRotation)
    {
    }

    BasicPlane(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation,
        const DirectX::XMFLOAT3& worldScale)
        : BasicShape(name, std::move(shader), worldPosition, worldRotation, worldScale)
    {
    }

    BasicPlane(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation, const DirectX::XMFLOAT3& worldScale)
        : BasicShape(name, std::move(shader), worldPosition, worldRotation, worldScale)
    {
    }

private:
    std::wstring GetShaderFileName() override { return L"NewShader.fx"; }
    std::string GetMeshFileName() override { return "DefaultObj/plane.obj"; }
};
