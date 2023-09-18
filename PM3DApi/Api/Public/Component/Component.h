#pragma once

#include <DirectXMath.h>
#include <vector>

namespace PM3D_API
{
class Component
{
public:
	Component(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
	virtual ~Component();

	virtual void Update(float elapsed);
	virtual void FixedUpdate(float elapsed);
	virtual void Draw() const;

	virtual void AddChild(Component* child);

	virtual DirectX::XMFLOAT3 GetPosition() const { return position; }
	virtual DirectX::XMFLOAT3 GetRotation() const { return rotation; }
	virtual DirectX::XMFLOAT3 GetScale() const { return scale; }
	virtual void SetPosition(const DirectX::XMFLOAT3 newPosition) { position = newPosition; }
	virtual void SetRotation(const DirectX::XMFLOAT3 newRotation) { rotation = newRotation; }
	virtual void SetScale(const DirectX::XMFLOAT3 newScale) { scale = newScale; }

	virtual std::vector<Component*> GetChildren() const { return children; }
protected:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	std::vector<Component*> children;

	virtual void DrawSelf() const;
};
}
