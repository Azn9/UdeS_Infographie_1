#pragma once
#include <algorithm>
#include <memory>

#include "../../Shader/Basic/DefaultShader.h"
#include "Api/Public/Component/Basic/Render/3D/MeshRenderer.h"

namespace PM3D_API
{
    class BasicShape : public PM3D_API::GameObject
    {
    public:
        explicit BasicShape(const std::string& name)
            : GameObject(name)
        {
        }

        BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition)
            : GameObject(name, worldPosition)
        {
        }

        BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition,
                   const DirectX::XMFLOAT3& worldRotation)
            : GameObject(name, worldPosition, worldRotation)
        {
        }

        BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
            : GameObject(name, worldPosition, worldRotation)
        {
        }

        BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition,
                   const DirectX::XMFLOAT3& worldRotation,
                   const DirectX::XMFLOAT3& worldScale)
            : GameObject(name, worldPosition, worldRotation, worldScale)
        {
        }

        BasicShape(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation,
                   const DirectX::XMFLOAT3& worldScale)
            : GameObject(name, worldPosition, worldRotation, worldScale)
        {
        }

        BasicShape(
            const std::string& name,
            std::unique_ptr<PM3D_API::Shader>&& shader
        ) : GameObject(name), shader(std::move(shader))
        {
        }

        BasicShape(
            const std::string& name,
            std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition
        ) : GameObject(name, worldPosition), shader(std::move(shader))
        {
        }

        BasicShape(
            const std::string& name,
            std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition,
            const DirectX::XMFLOAT3& worldRotation
        ) : GameObject(name, worldPosition, worldRotation), shader(std::move(shader))
        {
        }

        BasicShape(
            const std::string& name,
            std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition,
            const Quaternion& worldRotation
        ) : GameObject(name, worldPosition, worldRotation), shader(std::move(shader))
        {
        }

        BasicShape(
            const std::string& name,
            std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition,
            const DirectX::XMFLOAT3& worldRotation,
            const DirectX::XMFLOAT3& worldScale
        ) : GameObject(name, worldPosition, worldRotation, worldScale), shader(std::move(shader))
        {
        }

        BasicShape(
            const std::string& name,
            std::unique_ptr<PM3D_API::Shader>&& shader,
            const DirectX::XMFLOAT3& worldPosition,
            const Quaternion& worldRotation,
            const DirectX::XMFLOAT3& worldScale
        ) : GameObject(name, worldPosition, worldRotation, worldScale), shader(std::move(shader))
        {
        }

        void Initialize() override
        {
            if (shader)
            {
                ConstructRenderer(std::move(shader));
            }
            else
            {
                ConstructRenderer();
            }
        }

    private:
        std::unique_ptr<PM3D_API::Shader> shader;
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
}
