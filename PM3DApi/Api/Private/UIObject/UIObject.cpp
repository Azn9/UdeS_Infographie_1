#include <iostream>
#include "../../Public/UIObject/UIObject.h"
#include "../../../../PetitMoteur3D/Core/Public/Util/Time.h"
#include "../../Public/Util/Util.h"

using namespace Util;

namespace PM3D_API
{
    UIObject::UIObject(
        const std::string& name,
        UIObject* parent,
        const DirectX::XMFLOAT2& scale,
        const DirectX::XMFLOAT2& position,
        const ScaleMode& scaleMode,
        const Anchor& anchor,
        const Anchor& origin
    ) : name(name)
        , anchor(anchor)
        , origin(origin)
        , relativePosition(position)
        , scaleMode(scaleMode)
        , screenScale(scale)
        , parent(parent)
        , beginDrawSelf(0)
        , endDrawSelf(0)
    {
        SetRelativePosition(position);
        SetRelativeScale(scale);
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

        for (auto& child : children)
        {
            if (child)
                child->Update();
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

        for (auto& child : children)
        {
            if (child)
                child->Draw();
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

    void UIObject::LogBeginDrawSelf() const
    {
        beginDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
    }

    void UIObject::LogEndDrawSelf() const
    {
        endDrawSelf = PM3D::Time::GetInstance().GetTimeSpecific();
    }

    void UIObject::SetScreenPosition(const DirectX::XMFLOAT2& newOriginPosition)
    {
        screenPosition = newOriginPosition;

        if (!parent)
        {
            relativePosition = newOriginPosition;
        }
        else
        {
            const auto parentPosition = parent->GetScreenPosition();

            relativePosition = {
                newOriginPosition.x - parentPosition.x,
                newOriginPosition.y - parentPosition.y
            };
        }

        UpdateTopLeftPosition();

        for (const auto& child : children)
        {
            if (!child) continue;

            child->UpdatePosition();
        }
    }

    void UIObject::SetRelativePosition(const DirectX::XMFLOAT2& newOriginPosition)
    {
        relativePosition = newOriginPosition;

        if (!parent)
        {
            screenPosition = newOriginPosition;

            UpdateTopLeftPosition();

            for (const auto& child : children)
            {
                if (!child) continue;

                child->UpdatePosition();
            }
        }
        else
        {
            UpdatePosition();
        }
    }

    void UIObject::SetScreenScale(const DirectX::XMFLOAT2& newScale)
    {
        screenScale = newScale;

        if (scaleMode != ScaleMode::NONE_RELATIVE && parent == nullptr)
            throw std::runtime_error(
                "Cannot set screen scale on a UIObject that is relative to its parent without parent");

        if (scaleMode == ScaleMode::BOTH_RELATIVE)
            throw std::runtime_error("Cannot set screen scale on a UIObject that is relative to both axis");

        DirectX::XMFLOAT2 finalScale{};
        switch (scaleMode)
        {
        case ScaleMode::NONE_RELATIVE:
            finalScale = newScale;
            break;
        case ScaleMode::RELATIVE_X:
            finalScale.y = screenScale.y;

            if (newScale.x < 0.f || newScale.x > 1.f)
                throw std::runtime_error("Relative scale must be between 0 and 1");

            finalScale.x = newScale.x * parent->GetScreenScale().x;
            break;
        case ScaleMode::RELATIVE_Y:
            finalScale.x = screenScale.x;

            if (newScale.y < 0.f || newScale.y > 1.f)
                throw std::runtime_error("Relative scale must be between 0 and 1");

            finalScale.y = newScale.y * parent->GetScreenScale().y;
            break;
        }

        relativeScale = finalScale;

        UpdatePosition();
        UpdateTopLeftPosition();

        for (const auto& child : children)
        {
            if (!child) continue;

            child->UpdateScale();
            child->UpdatePosition();
        }
    }

    void UIObject::SetRelativeScale(const DirectX::XMFLOAT2& newScale)
    {
        if (scaleMode != ScaleMode::NONE_RELATIVE && parent == nullptr)
            throw std::runtime_error("Cannot set relative scale on a UIObject without parent");

        relativeScale = newScale;

        DirectX::XMFLOAT2 finalScale{};
        switch (scaleMode)
        {
        case ScaleMode::NONE_RELATIVE:
            finalScale = newScale;
            break;
        case ScaleMode::RELATIVE_X:
            finalScale.y = screenScale.y;
            finalScale.x = newScale.x * parent->GetScreenScale().x;
            break;
        case ScaleMode::RELATIVE_Y:
            finalScale.x = screenScale.x;
            finalScale.y = newScale.y * parent->GetScreenScale().y;
            break;
        case ScaleMode::BOTH_RELATIVE:
            finalScale.x = newScale.x * parent->GetScreenScale().x;
            finalScale.y = newScale.y * parent->GetScreenScale().y;
            break;
        }

        screenScale = finalScale;

        UpdatePosition();
        UpdateTopLeftPosition();

        for (const auto& child : children)
        {
            if (!child) continue;

            child->UpdateScale();
            child->UpdatePosition();
        }
    }

    void UIObject::UpdateScale()
    {
        // Recalculate screen scale from relative scale and parent screen size
        DirectX::XMFLOAT2 finalScale = screenScale;

        switch (scaleMode)
        {
        case ScaleMode::NONE_RELATIVE:
            // Nothing to do
            break;
        case ScaleMode::BOTH_RELATIVE:
            finalScale = {
                relativeScale.x * parent->GetScreenScale().x,
                relativeScale.y * parent->GetScreenScale().y
            };
            break;
        case ScaleMode::RELATIVE_X:
            finalScale = {
                relativeScale.x * parent->GetScreenScale().x,
                screenScale.y
            };
            break;
        case ScaleMode::RELATIVE_Y:
            finalScale = {
                screenScale.x,
                relativeScale.y * parent->GetScreenScale().y
            };
            break;
        default: ;
        }

        screenScale = finalScale;
        UpdatePosition();

        for (const auto& child : children)
        {
            if (!child) continue;

            child->UpdateScale();
        }
    }

    void UIObject::UpdatePosition()
    {
        std::cout << "UIObject::UpdatePosition() on " << name << std::endl;
        if (!parent)
        {
            std::cout << "parent is null" << std::endl;
            return;
        }
        // Recalculate screen position from relative position and parent screen position
        const DirectX::XMFLOAT2 parentTopLeftPosition = parent->GetTopLeftPosition();
        const DirectX::XMFLOAT2 parentScreenSize = parent->GetScreenScale();

        std::cout << "parentTopLeftPosition: " << parentTopLeftPosition.x << ", " << parentTopLeftPosition.y << std::endl;
        std::cout << "parentScreenSize: " << parentScreenSize.x << ", " << parentScreenSize.y << std::endl;

        static const std::vector<std::string> anchorNames = { "TOP_LEFT", "TOP_MIDDLE", "TOP_RIGHT", "MIDDLE_LEFT", "MIDDLE", "MIDDLE_RIGHT", "BOTTOM_LEFT", "BOTTOM_MIDDLE", "BOTTOM_RIGHT" };
        std::cout << "anchor: " << anchorNames[static_cast<int>(anchor)] << std::endl;
        std::cout << "origin: " << anchorNames[static_cast<int>(origin)] << std::endl;

        DirectX::XMFLOAT2 finalPosition{};

        switch (anchor)
        {
        case Anchor::TOP_LEFT:
            finalPosition = parentTopLeftPosition;
            break;
        case Anchor::TOP_MIDDLE:
            finalPosition = {
                parentTopLeftPosition.x + parentScreenSize.x / 2.f,
                parentTopLeftPosition.y
            };
            break;
        case Anchor::TOP_RIGHT:
            finalPosition = {
                parentTopLeftPosition.x + parentScreenSize.x,
                parentTopLeftPosition.y
            };
            break;
        case Anchor::MIDDLE_LEFT:
            finalPosition = {
                parentTopLeftPosition.x,
                parentTopLeftPosition.y + parentScreenSize.y / 2.f
            };
            break;
        case Anchor::MIDDLE:
            finalPosition = {
                parentTopLeftPosition.x + parentScreenSize.x / 2.f,
                parentTopLeftPosition.y + parentScreenSize.y / 2.f
            };
            break;
        case Anchor::MIDDLE_RIGHT:
            finalPosition = {
                parentTopLeftPosition.x + parentScreenSize.x,
                parentTopLeftPosition.y + parentScreenSize.y / 2.f
            };
            break;
        case Anchor::BOTTOM_LEFT:
            finalPosition = {
                parentTopLeftPosition.x,
                parentTopLeftPosition.y + parentScreenSize.y
            };
            break;
        case Anchor::BOTTOM_MIDDLE:
            finalPosition = {
                parentTopLeftPosition.x + parentScreenSize.x / 2.f,
                parentTopLeftPosition.y + parentScreenSize.y
            };
            break;
        case Anchor::BOTTOM_RIGHT:
            finalPosition = {
                parentTopLeftPosition.x + parentScreenSize.x,
                parentTopLeftPosition.y + parentScreenSize.y
            };
            break;
        default: ;
        }

        switch (origin)
        {
        case Anchor::TOP_LEFT:
            // Nothing to do
            break;
        case Anchor::TOP_MIDDLE:
            finalPosition = {
                finalPosition.x + screenScale.x / 2.f,
                finalPosition.y
            };
            break;
        case Anchor::TOP_RIGHT:
            finalPosition = {
                finalPosition.x + screenScale.x,
                finalPosition.y
            };
            break;
        case Anchor::MIDDLE_LEFT:
            finalPosition = {
                finalPosition.x,
                finalPosition.y + screenScale.y / 2.f
            };
            break;
        case Anchor::MIDDLE:
            finalPosition = {
                finalPosition.x + screenScale.x / 2.f,
                finalPosition.y + screenScale.y / 2.f
            };
            break;
        case Anchor::MIDDLE_RIGHT:
            finalPosition = {
                finalPosition.x + screenScale.x,
                finalPosition.y + screenScale.y / 2.f
            };
            break;
        case Anchor::BOTTOM_LEFT:
            finalPosition = {
                finalPosition.x,
                finalPosition.y + screenScale.y
            };
            break;
        case Anchor::BOTTOM_MIDDLE:
            finalPosition = {
                finalPosition.x + screenScale.x / 2.f,
                finalPosition.y + screenScale.y
            };
            break;
        case Anchor::BOTTOM_RIGHT:
            finalPosition = {
                finalPosition.x + screenScale.x,
                finalPosition.y + screenScale.y
            };
            break;
        default: ;
        }

        finalPosition.x += relativePosition.x;
        finalPosition.y += relativePosition.y;

        screenPosition = finalPosition;
        
        UpdateTopLeftPosition();

        for (const auto& child : children)
        {
            if (!child) continue;

            child->UpdatePosition();
        }
    }

    void UIObject::UpdateTopLeftPosition()
    {
        switch (origin)
        {
        case Anchor::TOP_LEFT:
            topLeftPosition = screenPosition;
            break;
        case Anchor::TOP_MIDDLE:
            topLeftPosition = {
                screenPosition.x - screenScale.x / 2.f,
                screenPosition.y
            };
            break;
        case Anchor::TOP_RIGHT:
            topLeftPosition = {
                screenPosition.x - screenScale.x,
                screenPosition.y
            };
            break;
        case Anchor::MIDDLE_LEFT:
            topLeftPosition = {
                screenPosition.x,
                screenPosition.y - screenScale.y / 2.f
            };
            break;
        case Anchor::MIDDLE:
            topLeftPosition = {
                screenPosition.x - screenScale.x / 2.f,
                screenPosition.y - screenScale.y / 2.f
            };
            break;
        case Anchor::MIDDLE_RIGHT:
            topLeftPosition = {
                screenPosition.x - screenScale.x,
                screenPosition.y - screenScale.y / 2.f
            };
            break;
        case Anchor::BOTTOM_LEFT:
            topLeftPosition = {
                screenPosition.x,
                screenPosition.y - screenScale.y
            };
            break;
        case Anchor::BOTTOM_MIDDLE:
            topLeftPosition = {
                screenPosition.x - screenScale.x / 2.f,
                screenPosition.y - screenScale.y
            };
            break;
        case Anchor::BOTTOM_RIGHT:
            topLeftPosition = {
                screenPosition.x - screenScale.x,
                screenPosition.y - screenScale.y
            };
            break;
        default: ;
        }

        std::cout << "topLeftPosition: " << topLeftPosition.x << ", " << topLeftPosition.y << " on " << name << std::endl;
    }
}
