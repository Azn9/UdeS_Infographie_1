#include "Api/Public/GameObject/GameObject.h"

PM3D_API::GameObject::GameObject(
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation,
	const DirectX::XMFLOAT3 scale
) : position(position), rotation(rotation), scale(scale)
{

}

PM3D_API::GameObject::~GameObject()
{
	for (const Component* component : components)
	{
		delete component;
	}
	
	for (const GameObject* child : children)
	{
		delete child;
	}
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

void PM3D_API::GameObject::AddChild(GameObject* child)
{
	children.emplace_back(child);
}

void PM3D_API::GameObject::AddComponent(Component* component)
{
	component->SetGameObject(this);
	components.emplace_back(component);
}

template <typename T, std::enable_if_t<IsDerivedFromHost<PM3D_API::Component, T>::value, int>>
T* PM3D_API::GameObject::GetComponent()
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
