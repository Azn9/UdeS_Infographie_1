#pragma once

#include <DirectXMath.h>
#include <memory>
#include <string>
#include <vector>

#include "Api/Public/Component/Component.h"
#include "Api/Public/Component/UIComponent.h"
#include "Api/Public/Util/Instanceof.h"

namespace PM3D_API
{
class UICanvas;
    
enum class Anchor
{
    TOP_LEFT, TOP_MIDDLE, TOP_RIGHT,
    MIDDLE_LEFT, MIDDLE, MIDDLE_RIGHT,
    BOTTOM_LEFT, BOTTOM_MIDDLE, BOTTOM_RIGHT
};

enum class SizeMode
{
    BOTH_RELATIVE, NONE_RELATIVE, RELATIVE_X, RELATIVE_Y 
};

class UIObject
{
public:
    explicit UIObject(
        const std::string& name,
        const DirectX::XMFLOAT2& size = {1.f, 1.f},
        const SizeMode& sizeMode = SizeMode::NONE_RELATIVE,
        const DirectX::XMFLOAT2& position = {0.f, 0.f},
        const Anchor& anchor = Anchor::MIDDLE,
        const Anchor& origin = Anchor::MIDDLE
    );

    virtual ~UIObject();

    virtual void Initialize();
    virtual void Update();
    virtual void Draw();
    virtual void AddChild(std::unique_ptr<UIObject>&& child);
    virtual void AddComponent(std::unique_ptr<UIComponent>&& component);

    std::string GetName() const { return name; }
    
    template <typename T, template_extends<T, UIComponent> = 0>
    std::vector<T*> GetComponents()
    {
        std::vector<T*> components{};
        for (const auto component : components)
        {
            if (!component) continue;
			
            if (typeid(*component.get()) == typeid(T))
            {
                components.push_back(static_cast<T*>(component.get()));
            }
        }

        return components;
    }

    const std::vector<std::unique_ptr<UIObject>>& GetChildren() const { return children; }
    const std::vector<std::unique_ptr<UIComponent>>& GetComponents() const { return components; }
    
protected:
    std::string name = "Unnamed UI Object";
    
    /// Anchor of parent
    Anchor anchor;

    /// Origin of the object
    Anchor origin;
    
    /// Position of the origin (relative to anchor)
    DirectX::XMFLOAT2 position;
    
    /// Position on the screen in pixels
    DirectX::XMFLOAT2 screenPosition;
    
    DirectX::XMFLOAT2 size;
    SizeMode sizeMode;

    std::vector<std::unique_ptr<UIObject>> children{};
    std::vector<std::unique_ptr<UIComponent>> components{};

    friend class UICanvas;
    UICanvas* canvas = nullptr;
    UIObject* parent = nullptr;
    
    virtual void DrawSelf() const;

    void LogBeginDrawSelf() const;
    void LogEndDrawSelf() const;

    ///Position on the screen in pixels
    virtual DirectX::XMFLOAT2 ResolvePosition() const;
    virtual DirectX::XMFLOAT2 ResolveSize() const;
    virtual DirectX::XMFLOAT2 ResolveAnchorPosition(const Anchor& anchor) const;

private:
    friend class Scene;
    void SetCanvas(UICanvas* newCanvas) { canvas = newCanvas; }

    void SetParent(UIObject* newParent);

    mutable int64_t beginDrawSelf;
    mutable int64_t endDrawSelf;
};
}
