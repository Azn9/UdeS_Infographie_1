#pragma once
#include "BasicShape.h"
#include "../../../Public/Component/Basic/MeshRenderer.h"
#include "../../../Public/GameObject/GameObject.h"
#include "../../../Public/Shader/Shader.h"

class BasicCone : public BasicShape
{
public:
    explicit BasicCone(const std::string& name)
        : BasicShape(name)
    {
    }

    BasicCone(const std::string& name, const DirectX::XMFLOAT3& worldPosition)
        : BasicShape(name, worldPosition)
    {
    }

    BasicCone(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation)
        : BasicShape(name, worldPosition, worldRotation)
    {
    }

    BasicCone(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
        : BasicShape(name, worldPosition, worldRotation)
    {
    }

    BasicCone(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation,
        const DirectX::XMFLOAT3& worldScale)
        : BasicShape(name, worldPosition, worldRotation, worldScale)
    {
    }

    BasicCone(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation,
        const DirectX::XMFLOAT3& worldScale)
        : BasicShape(name, worldPosition, worldRotation, worldScale)
    {
    }

    BasicCone(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader)
        : BasicShape(name, std::move(shader))
    {
    }

    BasicCone(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition)
        : BasicShape(name, std::move(shader), worldPosition)
    {
    }

    BasicCone(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation)
        : BasicShape(name, std::move(shader), worldPosition, worldRotation)
    {
    }

    BasicCone(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
        : BasicShape(name, std::move(shader), worldPosition, worldRotation)
    {
    }

    BasicCone(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation,
        const DirectX::XMFLOAT3& worldScale)
        : BasicShape(name, std::move(shader), worldPosition, worldRotation, worldScale)
    {
    }

    BasicCone(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation, const DirectX::XMFLOAT3& worldScale)
        : BasicShape(name, std::move(shader), worldPosition, worldRotation, worldScale)
    {
    }

private:
    std::wstring GetShaderFileName() override { return L"NewShader.fx"; }
    std::string GetMeshFileName() override { return "DefaultObj/cone.obj"; }
};
