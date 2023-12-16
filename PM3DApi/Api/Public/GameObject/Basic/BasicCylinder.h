#pragma once
#include "BasicShape.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Shader/Shader.h"

namespace PM3D_API
{
    class BasicCylinder : public BasicShape
    {
    public:
        explicit BasicCylinder(const std::string& name)
            : BasicShape(name)
        {
        }

        BasicCylinder(const std::string& name, const DirectX::XMFLOAT3& worldPosition)
            : BasicShape(name, worldPosition)
        {
        }

        BasicCylinder(const std::string& name, const DirectX::XMFLOAT3& worldPosition,
                      const DirectX::XMFLOAT3& worldRotation)
            : BasicShape(name, worldPosition, worldRotation)
        {
        }

        BasicCylinder(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
            : BasicShape(name, worldPosition, worldRotation)
        {
        }

        BasicCylinder(const std::string& name, const DirectX::XMFLOAT3& worldPosition,
                      const DirectX::XMFLOAT3& worldRotation,
                      const DirectX::XMFLOAT3& worldScale)
            : BasicShape(name, worldPosition, worldRotation, worldScale)
        {
        }

        BasicCylinder(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation,
                      const DirectX::XMFLOAT3& worldScale)
            : BasicShape(name, worldPosition, worldRotation, worldScale)
        {
        }

        BasicCylinder(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader)
            : BasicShape(name, std::move(shader))
        {
        }

        BasicCylinder(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
                      const DirectX::XMFLOAT3& worldPosition)
            : BasicShape(name, std::move(shader), worldPosition)
        {
        }

        BasicCylinder(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
                      const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation)
            : BasicShape(name, std::move(shader), worldPosition, worldRotation)
        {
        }

        BasicCylinder(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
                      const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
            : BasicShape(name, std::move(shader), worldPosition, worldRotation)
        {
        }

        BasicCylinder(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
                      const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation,
                      const DirectX::XMFLOAT3& worldScale)
            : BasicShape(name, std::move(shader), worldPosition, worldRotation, worldScale)
        {
        }

        BasicCylinder(const std::string& name, std::unique_ptr<PM3D_API::Shader>&& shader,
                      const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation,
                      const DirectX::XMFLOAT3& worldScale)
            : BasicShape(name, std::move(shader), worldPosition, worldRotation, worldScale)
        {
        }

    private:
        std::wstring GetShaderFileName() override { return L"shader/NewShader.fx"; }
        std::string GetMeshFileName() override { return "DefaultObj/cylinder.obj"; }
    };
}
