#pragma once
#include "BasicShape.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Shader/Shader.h"

namespace PM3D_API
{
    class BasicCube : public BasicShape
    {
    public:
        explicit BasicCube(const std::string& name)
            : BasicShape(name)
        {
        }

        BasicCube(const std::string& name, const DirectX::XMFLOAT3& worldPosition)
            : BasicShape(name, worldPosition)
        {
        }

        BasicCube(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation)
            : BasicShape(name, worldPosition, worldRotation)
        {
        }

        BasicCube(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
            : BasicShape(name, worldPosition, worldRotation)
        {
        }

        BasicCube(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation,
            const DirectX::XMFLOAT3& worldScale)
            : BasicShape(name, worldPosition, worldRotation, worldScale)
        {
        }

        BasicCube(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation,
            const DirectX::XMFLOAT3& worldScale)
            : BasicShape(name, worldPosition, worldRotation, worldScale)
        {
        }

        BasicCube(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader)
            : BasicShape(name, std::move(shader))
        {
        }

        BasicCube(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition)
            : BasicShape(name, std::move(shader), worldPosition)
        {
        }

        BasicCube(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation)
            : BasicShape(name, std::move(shader), worldPosition, worldRotation)
        {
        }

        BasicCube(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
            : BasicShape(name, std::move(shader), worldPosition, worldRotation)
        {
        }

        BasicCube(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation,
            const DirectX::XMFLOAT3& worldScale)
            : BasicShape(name, std::move(shader), worldPosition, worldRotation, worldScale)
        {
        }

        BasicCube(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation, const DirectX::XMFLOAT3& worldScale)
            : BasicShape(name, std::move(shader), worldPosition, worldRotation, worldScale)
        {
        }

    private:
        std::wstring GetShaderFileName() override { return L"shader/NewShader.fx"; }
        std::string GetMeshFileName() override { return "DefaultObj/cube.obj"; }
    };
}