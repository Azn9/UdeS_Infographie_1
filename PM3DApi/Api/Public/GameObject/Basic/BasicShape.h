#pragma once
#include <algorithm>
#include <memory>

#include "../../Shader/Basic/DefaultShader.h"
#include "../../../Public/Component/Basic/MeshRenderer.h"

class BasicShape : public PM3D_API::GameObject
{
public:
    explicit BasicShape(const std::string& name)
        : GameObject(name)
    {
        ConstructRenderer();
    }

    BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition)
        : GameObject(name, worldPosition)
    {
        ConstructRenderer();
    }

    BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation)
        : GameObject(name, worldPosition, worldRotation)
    {
        ConstructRenderer();
    }

    BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
        : GameObject(name, worldPosition, worldRotation)
    {
        ConstructRenderer();
    }

    BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const DirectX::XMFLOAT3& worldRotation,
        const DirectX::XMFLOAT3& worldScale)
        : GameObject(name, worldPosition, worldRotation, worldScale)
    {
        ConstructRenderer();
    }

    BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation,
        const DirectX::XMFLOAT3& worldScale)
        : GameObject(name, worldPosition, worldRotation, worldScale)
    {
        ConstructRenderer();
    }

    BasicShape(
        const std::string& name,
        std::unique_ptr<PM3D_API::Shader>&& shader
    ) : GameObject(name)
    {
        ConstructRenderer(std::move(shader));
    }

    BasicShape(
        const std::string& name,
        std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition
    ) : GameObject(name, worldPosition)
    {
        ConstructRenderer(std::move(shader));
    }

    BasicShape(
        const std::string& name,
        std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition,
        const DirectX::XMFLOAT3& worldRotation
    ) : GameObject(name, worldPosition, worldRotation)
    {
        ConstructRenderer(std::move(shader));
    }

    BasicShape(
        const std::string& name,
        std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition,
        const Quaternion& worldRotation
    ) : GameObject(name, worldPosition, worldRotation)
    {
        ConstructRenderer(std::move(shader));
    }

    BasicShape(
        const std::string& name,
        std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition,
        const DirectX::XMFLOAT3& worldRotation,
        const DirectX::XMFLOAT3& worldScale
    ) : GameObject(name, worldPosition, worldRotation, worldScale)
    {
        ConstructRenderer(std::move(shader));
    }

    BasicShape(
        const std::string& name,
        std::unique_ptr<PM3D_API::Shader>&& shader,
        const DirectX::XMFLOAT3& worldPosition,
        const Quaternion& worldRotation,
        const DirectX::XMFLOAT3& worldScale
    ) : GameObject(name, worldPosition, worldRotation, worldScale)
    {
        ConstructRenderer(std::move(shader));
    }
    
private:
    virtual std::wstring GetShaderFileName() = 0;
    virtual std::string GetMeshFileName() = 0;
    
    void ConstructRenderer()
    {
        auto shader = std::make_unique<PM3D_API::DefaultShader>(GetShaderFileName());
        ConstructRenderer(std::move(shader));
    }
    
    void ConstructRenderer(std::unique_ptr<PM3D_API::Shader>&& shader)
    {
        auto meshRenderer = std::make_unique<PM3D_API::MeshRenderer>(std::move(shader), GetMeshFileName());
        meshRenderer->Initialize();
        GameObject::AddComponent(std::move(meshRenderer));
    }
};
