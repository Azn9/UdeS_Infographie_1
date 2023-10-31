#include "../../Public/UIObject/UIObject.h"
#include <iostream>
#include "../../Public/Util/Util.h"

using namespace Util;

namespace PM3D_API
{

    UIObject::UIObject(const std::string& name, const DirectX::XMFLOAT2& size, const SizeMode& sizeMode
        , const DirectX::XMFLOAT2& position, const Anchor& anchor, const DirectX::XMFLOAT2& pivot)
        : name(name), position(position), size(size), sizeMode(sizeMode), parentAnchor(anchor), pivot(pivot)
    {
    }

    UIObject::~UIObject()
    {
    }

    void UIObject::Initialize()
    {
        std::cout << "GameObject::Initialize() on " << name << std::endl;
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

        for (auto& child: children)
        {
            const auto childPtr = child.get();

            if (childPtr)
                childPtr->Draw();
        }
    }

    inline void UIObject::DrawSelf() const
    {
    }
    
    void UIObject::AddChild(std::unique_ptr<UIObject>&& child)
    {
        std::cout << "GameObject::AddChild(GameObject*) on " << name << std::endl;
        child->parent = this;
        children.push_back(std::move(child));
    }

    void UIObject::AddComponent(std::unique_ptr<UIComponent>&& component)
    {
        std::cout << "GameObject::AddComponent(Component*) on " << name << std::endl;
        component->SetUIObject(this);
        components.push_back(std::move(component));
    }


    inline void UIObject::LogBeginDrawSelf() const
    {
    }

    inline void UIObject::LogEndDrawSelf() const
    {
    }

    DirectX::XMFLOAT2 UIObject::ResolvePosition() const
    {
        return position + parent->ResolveAnchorPosition(parentAnchor);
    }

    DirectX::XMFLOAT2 UIObject::ResolveSize() const
    {
        
    }

    DirectX::XMFLOAT2 UIObject::ResolveAnchorPosition(const Anchor& anchor) const
    {
        DirectX::XMFLOAT2 pos {0,0};

        if(anchor == Anchor::TOP_LEFT
            || anchor == Anchor::MIDDLE_LEFT
            || anchor == Anchor::BOTTOM_LEFT)
        {
            pos.x -= pivot.x * size.x;
        }

        return  pos + position;
    }

    void UIObject::SetParent(UIObject* newParent)
    {
        parent = newParent;
    }
}
