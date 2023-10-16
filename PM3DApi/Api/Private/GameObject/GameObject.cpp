#include "../../Public/GameObject/GameObject.h"

#include <iostream>
#include <stdexcept>

PM3D_API::GameObject::GameObject(
	const std::string& name,
	const DirectX::XMFLOAT3 worldPosition,
	const DirectX::XMFLOAT3 worldRotation,
	const DirectX::XMFLOAT3 worldScale
) : name(name),
	worldPosition(worldPosition),
    worldScale(worldScale),
    worldRotationEuler(worldRotation),
    worldRotationQuaternion(Quaternion::FromEulerAngles(worldRotation))
{
	GameObject::SetWorldPosition(worldPosition);
	GameObject::SetWorldRotation(worldRotation);
	GameObject::SetWorldScale(worldScale);
}

PM3D_API::GameObject::GameObject(
	const std::string& name,
	const DirectX::XMFLOAT3 worldPosition,
	const Quaternion worldRotation,
	const DirectX::XMFLOAT3 worldScale
) : name(name),
	worldPosition(worldPosition),
	worldScale(worldScale),
	worldRotationEuler(worldRotation.ToEulerAngles()),
	worldRotationQuaternion(worldRotation)
{
	GameObject::SetWorldPosition(worldPosition);
	GameObject::SetWorldRotation(worldRotation.ToEulerAngles());
	GameObject::SetWorldScale(worldScale);
}

PM3D_API::GameObject::~GameObject()
{
	std::cout << "GameObject::~GameObject() on " << name << std::endl;
	children.clear();
	components.clear();
}

void PM3D_API::GameObject::Initialize()
{
	std::cout << "GameObject::Initialize() on " << name << std::endl;
	for (auto& component : components)
	{
		const auto componentPtr = component.get();

		if (componentPtr)
			componentPtr->Initialize();
	}
}

void PM3D_API::GameObject::Update(const double elapsed)
{
	for (auto& component : components)
	{
		const auto componentPtr = component.get();

		if (componentPtr)
			componentPtr->Update(elapsed);
	}
}

void PM3D_API::GameObject::FixedUpdate(const double elapsed)
{
	for (auto& component : components)
	{
		const auto componentPtr = component.get();

		if (componentPtr)
			componentPtr->FixedUpdate(elapsed);
	}
}

void PM3D_API::GameObject::UpdateMatrix()
{
	matWorld = DirectX::XMMatrixScaling(worldScale.x, worldScale.y, worldScale.z) *
		DirectX::XMMatrixRotationQuaternion(worldRotationQuaternion.ToXMVector()) *
		DirectX::XMMatrixTranslation(worldPosition.x, worldPosition.y, worldPosition.z);
}

void PM3D_API::GameObject::Draw() const
{
	DrawSelf();

	for (auto& child: children)
	{
		const auto childPtr = child.get();

		if (childPtr)
			childPtr->Draw();
	}
}

void PM3D_API::GameObject::DrawSelf() const
{
	for (auto& component : components)
	{
		const auto componentPtr = component.get();

		if (componentPtr)
			componentPtr->DrawSelf();
	}
}

void PM3D_API::GameObject::SetParent(const GameObject* newParent)
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

void PM3D_API::GameObject::AddChild(std::unique_ptr<GameObject>&& child)
{
	std::cout << "GameObject::AddChild(GameObject*) on " << name << std::endl;
	child->parent = this;
	children.push_back(std::move(child));
}

void PM3D_API::GameObject::AddComponent(std::unique_ptr<Component>&& component)
{
	std::cout << "GameObject::AddComponent(Component*) on " << name << std::endl;
	component->SetGameObject(this);
	components.push_back(std::move(component));
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

	UpdateMatrix();
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

	UpdateMatrix();
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

	UpdateMatrix();
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

	UpdateMatrix();
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

	UpdateMatrix();
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

	UpdateMatrix();
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

	UpdateMatrix();
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

	UpdateMatrix();
}
