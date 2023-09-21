#pragma once

#include <DirectXMath.h>
#include <vector>

#include "../Util/Util.h"
#include "../Component/Component.h"

namespace PM3D_API
{
class Scene;
}

namespace PM3D_API
{
class GameObject
{
public:
	GameObject(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
	virtual ~GameObject();

	virtual void Update(double elapsed);
	virtual void FixedUpdate(double elapsed);
	virtual void Draw() const;

	virtual void AddChild(GameObject* child);

	virtual void AddComponent(Component* component);

	template <typename T, std::enable_if_t<IsDerivedFromHost<Component, T>::value, int>  = 0> T* GetComponent();

	virtual DirectX::XMFLOAT3 GetPosition() const { return position; }
	virtual DirectX::XMFLOAT3 GetRotation() const { return rotation; }
	virtual DirectX::XMFLOAT3 GetScale() const { return scale; }
	virtual void SetPosition(const DirectX::XMFLOAT3 newPosition) { position = newPosition; }
	virtual void SetRotation(const DirectX::XMFLOAT3 newRotation) { rotation = newRotation; }
	virtual void SetScale(const DirectX::XMFLOAT3 newScale) { scale = newScale; }

	virtual std::vector<GameObject*> GetChildren() const { return children; }

	virtual Scene* GetScene() const { return scene; }

protected:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	Scene* scene;

	std::vector<GameObject*> children;
	std::vector<Component*> components;

	virtual void DrawSelf() const;

private:
	friend class Scene;
	void SetScene(Scene* newScene) { scene = newScene; }
};

}
