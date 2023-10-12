#include "Api/Public/GameObject/GameObject.h"

#include <stdexcept>

PM3D_API::GameObject::GameObject(
	const DirectX::XMFLOAT3 worldPosition,
	const DirectX::XMFLOAT3 worldRotation,
	const DirectX::XMFLOAT3 worldScale
) : worldPosition(worldPosition),
    worldScale(worldScale),
    worldRotationEuler(worldRotation),
    worldRotationQuaternion(Quaternion::FromEulerAngles(worldRotation))
{
	GameObject::SetWorldPosition(worldPosition);
	GameObject::SetWorldRotation(worldRotation);
	GameObject::SetWorldScale(worldScale);
}

PM3D_API::GameObject::GameObject(
	const DirectX::XMFLOAT3 worldPosition,
	const Quaternion worldRotation,
	const DirectX::XMFLOAT3 worldScale
) : worldPosition(worldPosition),
	worldScale(worldScale),
	worldRotationEuler(worldRotation.ToEulerAngles()),
	worldRotationQuaternion(worldRotation)
{
	GameObject::SetWorldPosition(worldPosition);
	GameObject::SetWorldRotation(worldRotation.ToEulerAngles());
	GameObject::SetWorldScale(worldScale);
}

void PM3D_API::GameObject::Initialize()
{
	for (const auto component : components)
	{
		component->Initialize();
	}
}

void PM3D_API::GameObject::Update(const double elapsed)
{
	for (const auto component : components)
	{
		component->Update(elapsed);
	}
}

void PM3D_API::GameObject::FixedUpdate(const double elapsed)
{
	for (const auto component : components)
	{
		component->FixedUpdate(elapsed);
	}
}

void PM3D_API::GameObject::Draw() const
{
	DrawSelf();
	for (const auto child : children)
	{
		child->Draw();
	}
}

void PM3D_API::GameObject::DrawSelf() const
{
	for (const auto component : components)
	{
		component->DrawSelf();
	}
}

void PM3D_API::GameObject::SetParent(GameObject* newParent)
{
	if (parent == this)
	{
		throw std::runtime_error("A GameObject cannot be its own parent!");
	}

	const GameObject* newParentParent = newParent->parent;
	while (newParentParent != nullptr)
	{
		if (newParentParent == this)
		{
			throw std::runtime_error("A GameObject cannot be its own child!");
		}
		newParentParent = newParentParent->parent;
	}

	parent = newParent;
}

void PM3D_API::GameObject::AddChild(GameObject* child)
{
	child->parent = this;
	children.emplace_back(child);
}

void PM3D_API::GameObject::AddComponent(Component* component)
{
	component->SetGameObject(this);
	components.emplace_back(component);
}

template <typename T, template_extends<T, PM3D_API::Component>> T* PM3D_API::GameObject::GetComponent()
{
	for (const auto component : components)
	{
		if (typeid(*component) == typeid(T))
		{
			return static_cast<T*>(component);
		}
	}

	return nullptr;
}

void PM3D_API::GameObject::SetLocalPosition(const DirectX::XMFLOAT3 newPosition)
{
	localPosition = newPosition;
	worldPosition = newPosition;

	const GameObject* parentGameObject = parent;
	while (parentGameObject != nullptr)
	{
		const auto parentLocalPosition = parentGameObject->GetLocalPosition();
		worldPosition.x += parentLocalPosition.x;
		worldPosition.y += parentLocalPosition.y;
		worldPosition.z += parentLocalPosition.z;

		parentGameObject = parentGameObject->parent;
	}
}

void PM3D_API::GameObject::SetLocalRotation(const DirectX::XMFLOAT3 newRotation)
{
	localRotationEuler = newRotation;
	worldRotationEuler = newRotation;

	const GameObject* parentGameObject = parent;
	while (parentGameObject != nullptr)
	{
		const auto parentLocalRotation = parentGameObject->GetLocalRotationEuler();
		worldRotationEuler.x += parentLocalRotation.x;
		worldRotationEuler.y += parentLocalRotation.y;
		worldRotationEuler.z += parentLocalRotation.z;

		parentGameObject = parentGameObject->parent;
	}

	localRotationQuaternion = Quaternion::FromEulerAngles(localRotationEuler);
	worldRotationQuaternion = Quaternion::FromEulerAngles(worldRotationEuler);
}

void PM3D_API::GameObject::SetLocalRotation(const Quaternion newRotation)
{
	localRotationQuaternion = newRotation;
	worldRotationQuaternion = newRotation;

	const GameObject* parentGameObject = parent;
	while (parentGameObject != nullptr)
	{
		const auto parentLocalRotation = parentGameObject->GetLocalRotationQuaternion();
		worldRotationQuaternion = worldRotationQuaternion * parentLocalRotation;

		parentGameObject = parentGameObject->parent;
	}

	localRotationEuler = localRotationQuaternion.ToEulerAngles();
	worldRotationEuler = worldRotationQuaternion.ToEulerAngles();
}

void PM3D_API::GameObject::SetLocalScale(const DirectX::XMFLOAT3 newScale)
{
	localScale = newScale;
	worldScale = newScale;

	const GameObject* parentGameObject = parent;
	while (parentGameObject != nullptr)
	{
		const auto parentLocalScale = parentGameObject->GetLocalScale();
		worldScale.x *= parentLocalScale.x;
		worldScale.y *= parentLocalScale.y;
		worldScale.z *= parentLocalScale.z;

		parentGameObject = parentGameObject->parent;
	}
}

void PM3D_API::GameObject::SetWorldPosition(const DirectX::XMFLOAT3 newPosition)
{
	worldPosition = newPosition;
	localPosition = newPosition;

	const GameObject* parentGameObject = parent;
	while (parentGameObject != nullptr)
	{
		const auto parentWorldPosition = parentGameObject->GetWorldPosition();
		localPosition.x -= parentWorldPosition.x;
		localPosition.y -= parentWorldPosition.y;
		localPosition.z -= parentWorldPosition.z;

		parentGameObject = parentGameObject->parent;
	}
}

void PM3D_API::GameObject::SetWorldRotation(const DirectX::XMFLOAT3 newRotation)
{
	worldRotationEuler = newRotation;
	localRotationEuler = newRotation;

	const GameObject* parentGameObject = parent;
	while (parentGameObject != nullptr)
	{
		const auto parentWorldRotation = parentGameObject->GetWorldRotationEuler();
		localRotationEuler.x -= parentWorldRotation.x;
		localRotationEuler.y -= parentWorldRotation.y;
		localRotationEuler.z -= parentWorldRotation.z;

		parentGameObject = parentGameObject->parent;
	}

	worldRotationQuaternion = Quaternion::FromEulerAngles(worldRotationEuler);
	localRotationQuaternion = Quaternion::FromEulerAngles(localRotationEuler);
}

void PM3D_API::GameObject::SetWorldRotation(const Quaternion newRotation)
{
	worldRotationQuaternion = newRotation;
	localRotationQuaternion = newRotation;

	const GameObject* parentGameObject = parent;
	while (parentGameObject != nullptr)
	{
		const auto parentWorldRotation = parentGameObject->GetWorldRotationQuaternion();
		localRotationQuaternion = localRotationQuaternion * parentWorldRotation;

		parentGameObject = parentGameObject->parent;
	}

	worldRotationEuler = worldRotationQuaternion.ToEulerAngles();
	localRotationEuler = localRotationQuaternion.ToEulerAngles();
}

void PM3D_API::GameObject::SetWorldScale(const DirectX::XMFLOAT3 newScale)
{
	worldScale = newScale;
	localScale = newScale;

	const GameObject* parentGameObject = parent;
	while (parentGameObject != nullptr)
	{
		const auto parentWorldScale = parentGameObject->GetWorldScale();
		localScale.x /= parentWorldScale.x;
		localScale.y /= parentWorldScale.y;
		localScale.z /= parentWorldScale.z;

		parentGameObject = parentGameObject->parent;
	}
}
