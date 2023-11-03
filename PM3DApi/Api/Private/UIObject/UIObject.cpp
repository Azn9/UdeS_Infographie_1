#include <iostream>
#include "../../Public/UIObject/UIObject.h"

#include <any>

#include "../../../../PetitMoteur3D/Core/Public/Sprite/AfficheurSprite.h"
#include "../../../../PetitMoteur3D/Core/Public/Util/Time.h"
#include "../../Public/Util/Util.h"

using namespace Util;

namespace PM3D_API
{

UIObject::UIObject(
	const std::string& name,
	const DirectX::XMFLOAT2& size,
	const SizeMode& sizeMode,
	const DirectX::XMFLOAT2& position,
	const Anchor& anchor,
	const Anchor& origin
) : name(name), anchor(anchor), origin(origin), position(position), size(size), sizeMode(sizeMode)
{
}

UIObject::~UIObject() = default;

void UIObject::Initialize()
{
	std::cout << "UIObject::Initialize() on " << name << std::endl;
	for (auto& component : components)
	{
		const auto componentPtr = component.get();

		if (componentPtr)
			componentPtr->Initialize();
	}
}

void UIObject::Update()
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

void UIObject::Draw()
{
	DrawSelf();

	for (auto& component : components)
	{
		const auto componentPtr = component.get();

		if (componentPtr)
			componentPtr->DrawSelf();
	}

	for (auto& child : children)
	{
		const auto childPtr = child.get();

		if (childPtr)
			childPtr->Draw();
	}
}

inline void UIObject::DrawSelf() const
{
	// Nothing by default
}

void UIObject::AddChild(std::unique_ptr<UIObject>&& child)
{
	std::cout << "UIObject::AddChild(UIObject*) on " << name << std::endl;
	child->parent = this;
	child->canvas = canvas;
	children.push_back(std::move(child));
}

void UIObject::AddComponent(std::unique_ptr<UIComponent>&& component)
{
	std::cout << "UIObject::AddComponent(UIComponent*) on " << name << std::endl;
	component->SetUIObject(this);
	components.push_back(std::move(component));
}


inline void UIObject::LogBeginDrawSelf() const
{
	beginDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}

inline void UIObject::LogEndDrawSelf() const
{
	endDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}

DirectX::XMFLOAT2 UIObject::ResolvePosition() const
{
	return position + parent->ResolveAnchorPosition(anchor, origin);
}

DirectX::XMFLOAT2 UIObject::ResolveSize() const
{

}

DirectX::XMFLOAT2 UIObject::ResolveAnchorPosition(const Anchor& anchor, const Anchor& origin) const
{
	DirectX::XMFLOAT2 pos{0, 0};

	

	return position + pos;
}

void UIObject::SetParent(UIObject* newParent)
{
	parent = newParent;
}
}
