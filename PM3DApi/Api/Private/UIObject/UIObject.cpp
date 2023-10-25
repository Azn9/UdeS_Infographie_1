#include "../../Public/UIObject/UIObject.h"

namespace PM3D_API
{
    UIObject::UIObject(const std::string& name)
    {
    }

    UIObject::UIObject(const std::string& name, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size,
        const Anchor& anchor, const DirectX::XMFLOAT2& pivot)
    {
    }

    UIObject::~UIObject()
    {
    }

    void UIObject::Initialize()
    {
    }

    void UIObject::Update()
    {
    }

    void UIObject::Draw()
    {
    }

    void UIObject::AddChild(std::unique_ptr<UIObject>&& child)
    {
    }

    void UIObject::AddComponent(std::unique_ptr<UIObject>&& component)
    {
    }

    template <typename T, template_extends<T, Component>>
    std::vector<T*> UIObject::GetComponents()
    {
    }

    inline void UIObject::DrawSelf() const
    {
    }

    inline void UIObject::LogBeginDrawSelf() const
    {
    }

    inline void UIObject::LogEndDrawSelf() const
    {
    }
}
