#include "Api/Public/Component/Component.h"

PM3D_API::Component::Component(
	const DirectX::XMFLOAT3 position,
	const DirectX::XMFLOAT3 rotation,
	const DirectX::XMFLOAT3 scale
) : position(position), rotation(rotation), scale(scale)
{

}

PM3D_API::Component::~Component()
{
	for (const Component* child : children) {
		delete child;
	}
}

void PM3D_API::Component::Update(float elapsed)
{
	// Do nothing by default
}

void PM3D_API::Component::FixedUpdate(float elapsed)
{
	// Do nothing by default
}

void PM3D_API::Component::Draw() const
{
	DrawSelf();
	for (const auto child : children)
	{
		child->Draw();
	}
}

void PM3D_API::Component::DrawSelf() const
{
	// Do nothing by default
}

void PM3D_API::Component::AddChild(Component* child)
{
	children.push_back(child);
}
