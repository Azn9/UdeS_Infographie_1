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
class GameObject : public std::enable_shared_from_this<GameObject>
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

	virtual ~GameObject() = default;

	// ============================
	//  GameObject base methods
	// ============================
	virtual void Initialize();

	virtual void Update(double elapsed);
	virtual void FixedUpdate(double elapsed);
	virtual void Draw() const;

	virtual void AddChild(std::shared_ptr<GameObject> child);
	virtual void AddComponent(std::shared_ptr<Component> component);

	std::string GetName() const { return name; }

	template <typename T, template_extends<T, Component> = 0>
	std::shared_ptr<T>& GetComponent();

	std::vector<std::weak_ptr<GameObject>> GetChildren() const { return children; }

	std::shared_ptr<Scene> GetScene() const { return scene; }

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
	std::string name;
	
	DirectX::XMFLOAT3 localPosition;
	DirectX::XMFLOAT3 localScale;
	DirectX::XMFLOAT3 localRotationEuler;
	Quaternion localRotationQuaternion;

	DirectX::XMFLOAT3 worldPosition;
	DirectX::XMFLOAT3 worldScale;
	DirectX::XMFLOAT3 worldRotationEuler;
	Quaternion worldRotationQuaternion;

	DirectX::XMMATRIX matWorld;

	std::vector<std::weak_ptr<GameObject>> children;
	std::vector<std::weak_ptr<Component>> components;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<GameObject> parent;

	virtual void UpdateMatrix();
	virtual void DrawSelf() const;

private:
	friend class Scene;
	void SetScene(const std::shared_ptr<Scene>& newScene) { scene = newScene; }

	void SetParent(const std::shared_ptr<GameObject>& newParent);
};

}
