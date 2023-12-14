#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>

#include "../Component/Component.h"
#include "../Util/Instanceof.h"
#include "../Util/Quaternion.h"

namespace PM3D_API
{
class Scene;
}

namespace PM3D_API
{
class GameObject
{
public:
	// ============================
	//  Constructors & Destructors
	// ============================
	explicit GameObject(const std::string& name) : GameObject(
		name,
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f}
	) {}

	GameObject(
		const std::string& name,
		const DirectX::XMFLOAT3 worldPosition
	) : GameObject(
		name,
		worldPosition,
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f}
	) {}
	
	GameObject(
		const std::string& name,
		const DirectX::XMFLOAT3 worldPosition,
		const DirectX::XMFLOAT3 worldRotation
	) : GameObject(
		name,
		worldPosition,
		worldRotation,
		{1.0f, 1.0f, 1.0f}
	) {}

	GameObject(
		const std::string& name,
		const DirectX::XMFLOAT3 worldPosition,
		const Quaternion worldRotation
	) : GameObject(
		name,
		worldPosition,
		worldRotation,
		{1.0f, 1.0f, 1.0f}
	) {}
	
	GameObject(
		const std::string& name,
		DirectX::XMFLOAT3 worldPosition,
		DirectX::XMFLOAT3 worldRotation,
		DirectX::XMFLOAT3 worldScale
	);

	GameObject(
		const std::string& name,
		DirectX::XMFLOAT3 worldPosition,
		Quaternion worldRotation,
		DirectX::XMFLOAT3 worldScale
	);

	virtual ~GameObject();

	// ============================
	//  GameObject base methods
	// ============================
	virtual void Initialize();

	virtual void Update();
	virtual void PhysicsUpdate();
	virtual void Draw();

	virtual void AddChild(std::unique_ptr<GameObject>&& child);
	virtual void AddComponent(std::unique_ptr<Component>&& component);

	std::string GetName() const { return name; }

	template <typename T, template_extends<Component, T> = 0>
	T* GetComponent()
	{
		for (const auto& component : components)
		{
			if (!component) continue;
			
			if (typeid(*component.get()) == typeid(T) || IsDerivedFrom<T, Component>::value)
			{
				return static_cast<T*>(component.get());
			}
		}

		return nullptr;
	}

	template <typename T, template_extends<GameObject, T> = 0>
	T* GetChild()
	{
		for (const auto& child : children)
		{
			if (!child) continue;

			if (typeid(*child.get()) == typeid(T))
			{
				return static_cast<T*>(child.get());
			}
		}

		return nullptr;
	}

	template <typename T, template_extends<Component, T> = 0>
	bool HasComponent() const
	{
		for (const auto& component : components)
		{
			if (!component) continue;
			
			if (typeid(*component.get()) == typeid(T))
			{
				return true;
			}
		}

		return false;
	}
	
	template <typename T, template_extends<Component, T> = 0>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> toReturnComponents{};
		for (const auto& component : components)
		{
			if (!component) continue;
			
			if (typeid(*component.get()) == typeid(T))
			{
				toReturnComponents.push_back(static_cast<T*>(component.get()));
			}
		}

		return toReturnComponents;
	}

	const std::vector<std::unique_ptr<GameObject>>& GetChildren() const { return children; }
	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return components; }

	Scene* GetScene() { return scene; }

	// ============================
	// Position, rotation, scale
	// ============================
	virtual const DirectX::XMFLOAT3& GetLocalPosition() const { return localPosition; }
	virtual const DirectX::XMFLOAT3& GetLocalScale() const { return localScale; }
	virtual const DirectX::XMFLOAT3& GetLocalRotationEuler() const { return localRotationEuler; }
	virtual const Quaternion& GetLocalRotationQuaternion() const { return localRotationQuaternion; }
	virtual void SetLocalPosition(DirectX::XMFLOAT3 newPosition);
	virtual void SetLocalScale(DirectX::XMFLOAT3 newScale);
	virtual void SetLocalRotation(DirectX::XMFLOAT3 newRotation);
	virtual void SetLocalRotation(Quaternion newRotation);

	virtual const DirectX::XMFLOAT3& GetWorldPosition() const { return worldPosition; }
	virtual const DirectX::XMFLOAT3& GetWorldScale() const { return worldScale; }
	virtual const DirectX::XMFLOAT3& GetWorldRotationEuler() const { return worldRotationEuler; }
	virtual const Quaternion& GetWorldRotationQuaternion() const { return worldRotationQuaternion; }
	virtual DirectX::XMFLOAT3 GetWorldDirection() const;
	virtual void SetWorldPosition(DirectX::XMFLOAT3 newPosition);
	virtual void SetWorldScale(DirectX::XMFLOAT3 newScale);
	virtual void SetWorldRotation(DirectX::XMFLOAT3 newRotation);
	virtual void SetWorldRotation(Quaternion newRotation);

	void SetWorldPositionViaPhysic(DirectX::XMFLOAT3 newPosition);
	void SetWorldRotationViaPhysic(Quaternion quaternion);

	virtual const DirectX::XMMATRIX& GetMatWorld() const { return matWorld; }

	virtual void DrawDebugInfo() const {}
	int64_t GetBeginDrawSelf() const { return beginDrawSelf; }
	int64_t GetEndDrawSelf() const { return endDrawSelf; }

	std::unique_ptr<GameObject> DetachFromParent();
	void SetParent(GameObject* newParent);
	
	void SetScene(Scene* newScene) { scene = newScene; }
	GameObject* GetParent() const { return parent; }

protected:
	std::string name = "Unnamed GameObject";
	
	DirectX::XMFLOAT3 localPosition;
	DirectX::XMFLOAT3 localScale;
	DirectX::XMFLOAT3 localRotationEuler;
	Quaternion localRotationQuaternion;

	DirectX::XMFLOAT3 worldPosition;
	DirectX::XMFLOAT3 worldScale;
	DirectX::XMFLOAT3 worldRotationEuler;
	Quaternion worldRotationQuaternion;

	DirectX::XMMATRIX matWorld;

	std::vector<std::unique_ptr<GameObject>> children{};
	std::vector<std::unique_ptr<Component>> components{};
	Scene* scene = nullptr;
	GameObject* parent = nullptr;

	virtual void UpdateMatrix(bool updatePhysicRepresentation);
	virtual void DrawSelf() const;

	void LogBeginDrawSelf() const;
	void LogEndDrawSelf() const;
private:
	friend class Scene;

	mutable int64_t beginDrawSelf;
	mutable int64_t endDrawSelf;
};

}
