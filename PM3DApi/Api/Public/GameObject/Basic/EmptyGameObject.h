#pragma once
#include "Api/Public/GameObject/GameObject.h"

namespace PM3D_API
{
    class EmptyGameObject final : public PM3D_API::GameObject
    {
    public:
        explicit EmptyGameObject(const std::string& name)
            : GameObject(name)
        {
        }

        EmptyGameObject(const std::string& name, const DirectX::XMFLOAT3& worldPosition)
            : GameObject(name, worldPosition)
        {
        }

        EmptyGameObject(const std::string& name, const DirectX::XMFLOAT3& worldPosition,
            const DirectX::XMFLOAT3& worldRotation)
            : GameObject(name, worldPosition, worldRotation)
        {
        }

        EmptyGameObject(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation)
            : GameObject(name, worldPosition, worldRotation)
        {
        }

        EmptyGameObject(const std::string& name, const DirectX::XMFLOAT3& worldPosition,
            const DirectX::XMFLOAT3& worldRotation, const DirectX::XMFLOAT3& worldScale)
            : GameObject(name, worldPosition, worldRotation, worldScale)
        {
        }

        EmptyGameObject(const std::string& name, const DirectX::XMFLOAT3& worldPosition, const Quaternion& worldRotation,
            const DirectX::XMFLOAT3& worldScale)
            : GameObject(name, worldPosition, worldRotation, worldScale)
        {
        }
    };
}