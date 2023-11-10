#include <iostream>
#include "Api/Public/GameObject/UIObject.h"

#include <mutex>

#include "Core/Public/Util/Time.h"
#include "Api/Public/Util/Util.h"
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
	// Calculate the center of the object
	DirectX::XMFLOAT2 center;
	center.x = basePositionComputed.load().x + (baseScaleComputed.load().x / 2.0f);
	center.y = basePositionComputed.load().y + (baseScaleComputed.load().y / 2.0f);

	// Calculate the rotation difference
	float rotationDiff = newRotation - rotation;

	// Rotate the position around the center
	float cosAngle = cos(rotationDiff);
	float sinAngle = sin(rotationDiff);

	// Translate point back to origin
	float translatedX = basePositionComputed.load().x - center.x;
	float translatedY = basePositionComputed.load().y - center.y;

	// Rotate point
	DirectX::XMFLOAT2 rotatedPosition;
	rotatedPosition.x = translatedX * cosAngle - translatedY * sinAngle;
	rotatedPosition.y = translatedX * sinAngle + translatedY * cosAngle;

	// Translate point back
	position.x = rotatedPosition.x + center.x;
	position.y = rotatedPosition.y + center.y;

	// Update the rotation
	rotation = newRotation;
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
		this->basePositionComputed = this->position = {
			basePosition.x * canvas->GetScale().x,
			basePosition.y * canvas->GetScale().y
		};
	}
	else
	{
		this->basePositionComputed = this->position = position;
	}

	if (relativeScale)
	{
		this->baseScaleComputed = this->scale = {
			baseScale.x * canvas->GetScale().x,
			baseScale.y * canvas->GetScale().y
		};
	}
	else
	{
		this->baseScaleComputed = this->scale = scale;
	}
}

}
