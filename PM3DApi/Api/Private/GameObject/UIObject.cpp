#include <iostream>
#include "../../Public/GameObject/UIObject.h"
#include "../../../../PetitMoteur3D/Core/Public/Util/Time.h"
#include "../../Public/Util/Util.h"
#include "Api/Public/GameHost.h"
#include "Api/Public/GameObject/UICanvas.h"
#include "Api/Public/Util/Axis.h"

using namespace Util;

namespace PM3D_API
{
UIObject::UIObject(
	std::string name,
	const DirectX::XMFLOAT2& scale,
	const DirectX::XMFLOAT2& position,
	const float rotation,
	const bool relativeScale,
	const bool relativePosition
) : name(std::move(name))
    , basePosition(position)
    , baseScale(scale)
    , relativeScale(relativeScale)
    , relativePosition(relativePosition)
    , beginDrawSelf(0)
    , endDrawSelf(0)
	, rotation(rotation)
{
	const auto canvas = GameHost::GetInstance()->GetScene()->GetUICanvas();

	if (!canvas)
	{
		this->position = position;
		this->scale = scale;
		return;
	}

	if (relativePosition)
	{
		this->position = {
			position.x * canvas->GetScale().x,
			position.y * canvas->GetScale().y
		};
	}
	else
	{
		this->position = position;
	}
	this->basePositionComputed = this->position;

	if (relativeScale)
	{
		this->scale = {
			scale.x * canvas->GetScale().x,
			scale.y * canvas->GetScale().y
		};
	}
	else
	{
		this->scale = scale;
	}
	this->baseScaleComputed = this->scale;
}

UIObject::~UIObject() = default;

void UIObject::Initialize()
{
	std::cout << "UIObject::Initialize() on " << name << std::endl;
	for (auto& component : components)
	{
		if (component)
			component->Initialize();
	}
}

void UIObject::Update()
{
	for (auto& component : components)
	{
		if (component)
			component->Update();
	}
}

void UIObject::Draw()
{
	DrawSelf();

	for (auto& component : components)
	{
		if (component)
			component->DrawSelf();
	}
}

inline void UIObject::DrawSelf() const
{
	// Nothing by default
}

void UIObject::AddComponent(std::unique_ptr<UIComponent>&& component)
{
	std::cout << "UIObject::AddComponent(UIComponent*) on " << name << std::endl;
	component->SetUIObject(this);
	components.push_back(std::move(component));
}

void UIObject::SetCentralRotation(const float newRotation)
{
	rotation = newRotation;

	const auto C = DirectX::XMFLOAT2(
		basePositionComputed.x + scale.x / 2.f,
		basePositionComputed.y + scale.y / 2.f
	);

	const auto C2 = DirectX::XMFLOAT2(
		(C.x - basePositionComputed.x) * cosf(rotation) - (C.y - basePositionComputed.y) * sinf(rotation) + basePositionComputed.x,
		(C.x - basePositionComputed.x) * sinf(rotation) + (C.y - basePositionComputed.y) * cosf(rotation) + basePositionComputed.y
	);

	const auto pos2 = DirectX::XMFLOAT2(
		basePositionComputed.x + (C.x - C2.x),
		basePositionComputed.y + (C.y - C2.y)
	);

	position = pos2;
}

void UIObject::LogBeginDrawSelf() const
{
	beginDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}

void UIObject::LogEndDrawSelf() const
{
	endDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
}

void UIObject::UpdateScaleAndPosition()
{
	if (relativePosition)
	{
		this->position = {
			position.x * canvas->GetScale().x,
			position.y * canvas->GetScale().y
		};
	}
	else
	{
		this->position = position;
	}

	if (relativeScale)
	{
		this->scale = {
			scale.x * canvas->GetScale().x,
			scale.y * canvas->GetScale().y
		};
	}
	else
	{
		this->scale = scale;
	}

	this->basePositionComputed = this->position;
	this->baseScaleComputed = this->scale;
}

}
