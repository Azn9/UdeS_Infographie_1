#include "../../Public/GameObject/GameObject.h"

#include <iostream>
#include <stdexcept>

#include "../../../../PetitMoteur3D/Core/Public/Core/MoteurWindows.h"
#include "Api/Public/Component/Basic/Physics/Rigidbody.h"

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
	// Force recalculation
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
	// Force recalculation
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

void PM3D_API::GameObject::Update()
{
	for (auto& component : components)
	{
		const auto componentPtr = component.get();

		if (componentPtr)
			componentPtr->Update();
	}

	for (auto& child : children)
	{
		if (!child)
			continue;
		
		const auto childPtr = child.get();

		if (!childPtr)
			continue;

		childPtr->Update();
	}
}

void PM3D_API::GameObject::PhysicsUpdate()
{
	for (auto& component : components)
	{
		const auto componentPtr = component.get();

		if (componentPtr)
			componentPtr->PhysicsUpdate();
	}

	for (auto& child : children)
	{
		if (!child)
			continue;
		
		const auto childPtr = child.get();

		if (!childPtr)
			continue;

		childPtr->PhysicsUpdate();
	}
}

void PM3D_API::GameObject::UpdateMatrix(bool updatePhysicRepresentation)
{
	matWorld = DirectX::XMMatrixScaling(worldScale.x, worldScale.y, worldScale.z) *
		DirectX::XMMatrixRotationQuaternion(worldRotationQuaternion.ToXMVector()) *
		DirectX::XMMatrixTranslation(worldPosition.x, worldPosition.y, worldPosition.z);

	if (updatePhysicRepresentation)
	{
		if (const auto rigidBody = GetComponent<Rigidbody>())
			rigidBody->UpdateGlobalPose();
	}
}

void PM3D_API::GameObject::Draw()
{
	DrawSelf();

	for (auto& component : components)
	{
		const auto componentPtr = component.get();

		if (componentPtr)
			componentPtr->DrawSelf();
	}

	for (auto& child: children)
	{
		const auto childPtr = child.get();

		if (childPtr)
			childPtr->Draw();
	}
}

void PM3D_API::GameObject::DrawSelf() const
{
	// Do nothing by default
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

	if (parent != nullptr)
	{
		worldPosition.x = parent->GetWorldPosition().x + localPosition.x;
		worldPosition.y = parent->GetWorldPosition().y + localPosition.y;
		worldPosition.z = parent->GetWorldPosition().z + localPosition.z;
	}

	UpdateMatrix(true);
}

void PM3D_API::GameObject::SetLocalRotation(const DirectX::XMFLOAT3 newRotation)
{
	localRotationEuler = newRotation;
	worldRotationEuler = newRotation;

	if (parent != nullptr)
	{
		worldRotationEuler.x = parent->GetWorldRotationEuler().x + localRotationEuler.x;
		worldRotationEuler.y = parent->GetWorldRotationEuler().y + localRotationEuler.y;
		worldRotationEuler.z = parent->GetWorldRotationEuler().z + localRotationEuler.z;
	}

	localRotationQuaternion = Quaternion::FromEulerAngles(localRotationEuler);
	worldRotationQuaternion = Quaternion::FromEulerAngles(worldRotationEuler);

	UpdateMatrix(true);
}

void PM3D_API::GameObject::SetLocalRotation(const Quaternion newRotation)
{
	localRotationQuaternion = newRotation;
	worldRotationQuaternion = newRotation;

	if (parent != nullptr)
	{
		worldRotationQuaternion = worldRotationQuaternion * parent->GetWorldRotationQuaternion();
	}

	localRotationEuler = localRotationQuaternion.ToEulerAngles();
	worldRotationEuler = worldRotationQuaternion.ToEulerAngles();

	UpdateMatrix(true);
}

void PM3D_API::GameObject::SetLocalScale(const DirectX::XMFLOAT3 newScale)
{
	localScale = newScale;
	worldScale = newScale;

	if (parent != nullptr)
	{
		worldScale.x = parent->GetWorldScale().x * localScale.x;
		worldScale.y = parent->GetWorldScale().y * localScale.y;
		worldScale.z = parent->GetWorldScale().z * localScale.z;
	}

	UpdateMatrix(true);
}

void PM3D_API::GameObject::SetWorldPosition(const DirectX::XMFLOAT3 newPosition)
{
	worldPosition = newPosition;
	localPosition = newPosition;

	if (parent != nullptr)
	{
		localPosition.x = worldPosition.x - parent->GetWorldPosition().x;
		localPosition.y = worldPosition.y - parent->GetWorldPosition().y;
		localPosition.z = worldPosition.z - parent->GetWorldPosition().z;
	}

	UpdateMatrix(true);
}

void PM3D_API::GameObject::SetWorldRotation(const DirectX::XMFLOAT3 newRotation)
{
	worldRotationEuler = newRotation;
	localRotationEuler = newRotation;

	if (parent != nullptr)
	{
		localRotationEuler.x = worldRotationEuler.x - parent->GetWorldRotationEuler().x;
		localRotationEuler.y = worldRotationEuler.y - parent->GetWorldRotationEuler().y;
		localRotationEuler.z = worldRotationEuler.z - parent->GetWorldRotationEuler().z;
	}

	worldRotationQuaternion = Quaternion::FromEulerAngles(worldRotationEuler);
	localRotationQuaternion = Quaternion::FromEulerAngles(localRotationEuler);

	UpdateMatrix(true);
}

void PM3D_API::GameObject::SetWorldRotation(const Quaternion newRotation)
{
	worldRotationQuaternion = newRotation;
	localRotationQuaternion = newRotation;

	if (parent != nullptr)
	{
		localRotationQuaternion = localRotationQuaternion * parent->GetWorldRotationQuaternion();
	}

	worldRotationEuler = worldRotationQuaternion.ToEulerAngles();
	localRotationEuler = localRotationQuaternion.ToEulerAngles();

	UpdateMatrix(true);
}

void PM3D_API::GameObject::SetWorldPositionViaPhysic(const DirectX::XMFLOAT3 newPosition)
{
	worldPosition = newPosition;
	localPosition = newPosition;

	if (parent != nullptr)
	{
		localPosition.x = worldPosition.x - parent->GetWorldPosition().x;
		localPosition.y = worldPosition.y - parent->GetWorldPosition().y;
		localPosition.z = worldPosition.z - parent->GetWorldPosition().z;
	}

	UpdateMatrix(false);
}

void PM3D_API::GameObject::SetWorldRotationViaPhysic(const Quaternion newRotation)
{
	worldRotationQuaternion = newRotation;
	localRotationQuaternion = newRotation;

	if (parent != nullptr)
	{
		localRotationQuaternion = localRotationQuaternion * parent->GetWorldRotationQuaternion();
	}

	worldRotationEuler = worldRotationQuaternion.ToEulerAngles();
	localRotationEuler = localRotationQuaternion.ToEulerAngles();

	UpdateMatrix(false);
}

void PM3D_API::GameObject::SetWorldScale(const DirectX::XMFLOAT3 newScale)
{
	worldScale = newScale;
	localScale = newScale;

	if (parent != nullptr)
	{
		localScale.x = worldScale.x / parent->GetWorldScale().x;
		localScale.y = worldScale.y / parent->GetWorldScale().y;
		localScale.z = worldScale.z / parent->GetWorldScale().z;
	}

	UpdateMatrix(true);
}

void PM3D_API::GameObject::LogBeginDrawSelf() const
{
	beginDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}

void PM3D_API::GameObject::LogEndDrawSelf() const
{
	endDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}
