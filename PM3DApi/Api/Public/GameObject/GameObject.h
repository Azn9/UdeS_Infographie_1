#pragma once

#include <DirectXMath.h>
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
	GameObject() : GameObject(
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f}
	) {}

	explicit GameObject(
		const DirectX::XMFLOAT3 worldPosition
	) : GameObject(
		worldPosition,
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f}
	) {}
	
	GameObject(
		const DirectX::XMFLOAT3 worldPosition,
		const DirectX::XMFLOAT3 worldRotation
	) : GameObject(
		worldPosition,
		worldRotation,
		{1.0f, 1.0f, 1.0f}
	) {}

	GameObject(
		const DirectX::XMFLOAT3 worldPosition,
		const Quaternion worldRotation
	) : GameObject(
		worldPosition,
		worldRotation,
		{1.0f, 1.0f, 1.0f}
	) {}
	
	GameObject(
		DirectX::XMFLOAT3 worldPosition,
		DirectX::XMFLOAT3 worldRotation,
		DirectX::XMFLOAT3 worldScale
	);

	GameObject(
		DirectX::XMFLOAT3 worldPosition,
		Quaternion worldRotation,
		DirectX::XMFLOAT3 worldScale
	);

	virtual ~GameObject() = default;

	// ============================
	//  GameObject base methods
	// ============================
	virtual void Initialize();

	virtual void Update(double elapsed);
	virtual void FixedUpdate(double elapsed);
	virtual void Draw() const;

	virtual void AddChild(GameObject* child);

	virtual void AddComponent(Component* component);

	template <typename T, template_extends<T, Component> = 0> T* GetComponent();

	std::vector<GameObject*> GetChildren() const { return children; }

	Scene* GetScene() const { return scene; }

	// ============================
	// Position, rotation, scale
	// ============================
	virtual DirectX::XMFLOAT3 GetLocalPosition() const { return localPosition; }
	virtual DirectX::XMFLOAT3 GetLocalScale() const { return localScale; }
	virtual DirectX::XMFLOAT3 GetLocalRotationEuler() const { return localRotationEuler; }
	virtual Quaternion GetLocalRotationQuaternion() const { return localRotationQuaternion; }
	virtual void SetLocalPosition(DirectX::XMFLOAT3 newPosition);
	virtual void SetLocalScale(DirectX::XMFLOAT3 newScale);
	virtual void SetLocalRotation(DirectX::XMFLOAT3 newRotation);
	virtual void SetLocalRotation(Quaternion newRotation);

	virtual DirectX::XMFLOAT3 GetWorldPosition() const { return worldPosition; }
	virtual DirectX::XMFLOAT3 GetWorldScale() const { return worldScale; }
	virtual DirectX::XMFLOAT3 GetWorldRotationEuler() const { return worldRotationEuler; }
	virtual Quaternion GetWorldRotationQuaternion() const { return worldRotationQuaternion; }
	virtual void SetWorldPosition(DirectX::XMFLOAT3 newPosition);
	virtual void SetWorldScale(DirectX::XMFLOAT3 newScale);
	virtual void SetWorldRotation(DirectX::XMFLOAT3 newRotation);
	virtual void SetWorldRotation(Quaternion newRotation);

	virtual DirectX::XMMATRIX GetMatWorld() const { return matWorld; }

protected:
	DirectX::XMFLOAT3 localPosition;
	DirectX::XMFLOAT3 localScale;
	DirectX::XMFLOAT3 localRotationEuler;
	Quaternion localRotationQuaternion;

	DirectX::XMFLOAT3 worldPosition;
	DirectX::XMFLOAT3 worldScale;
	DirectX::XMFLOAT3 worldRotationEuler;
	Quaternion worldRotationQuaternion;

	DirectX::XMMATRIX matWorld;

	std::vector<GameObject*> children;
	std::vector<Component*> components;
	Scene* scene;
	GameObject* parent;

	virtual void DrawSelf() const;

private:
	friend class Scene;
	void SetScene(Scene* newScene) { scene = newScene; }

	void SetParent(GameObject* newParent);
};

}
