#pragma once

#include <DirectXMath.h>
#include <memory>
#include <string>
#include <vector>

#include "Api/Public/Component/Component.h"
#include "Api/Public/UICanvas/UICanvas.h"
#include "Api/Public/Util/Instanceof.h"

namespace PM3D_API
{
    
enum class Anchor
{
    TOP_LEFT, TOP_MIDDLE, TOP_RIGHT,
    MIDDLE_LEFT, MIDDLE, MIDDLE_RIGHT,
    BOTTOM_LEFT, BOTTOM_MIDDLE, BOTTOM_RIGHT
};

enum class SizeMode
{
    RELATIVE, ABSOLUTE, RELATIVE_X, RELATIVE_Y 
};

class UIObject
{
public:

    UIObject(const std::string& name);
    
    UIObject(const std::string& name
        , const DirectX::XMFLOAT2& position
        , const DirectX::XMFLOAT2& size
        , const Anchor& anchor
        , const DirectX::XMFLOAT2& pivot);


    virtual ~UIObject();

    virtual void Initialize();
    virtual void Update();
    virtual void Draw();
    virtual void AddChild(std::unique_ptr<UIObject>&& child);
    virtual void AddComponent(std::unique_ptr<UIObject>&& component);

    std::string GetName() const { return name; }
    
    template <typename T, template_extends<T, Component> = 0>
    std::vector<T*> GetComponents();

    const std::vector<std::unique_ptr<UIObject>>& GetChildren() const { return children; }
    const std::vector<std::unique_ptr<Component>>& GetComponents() const { return components; }
    
protected:

    std::string name = "Unnamed GameObject";
    
    /// Anchor of parent
    Anchor anchor;
    /// Position of the pivot on the object. pivot=(0,0.5) -> the pivot will be at the left side and at half it's height
    DirectX::XMFLOAT2 pivot; 
    /// Position of the origin (relative to anchor)
    DirectX::XMFLOAT2 position; 
    
    DirectX::XMFLOAT2 size;
    SizeMode size_mode;

    std::vector<std::unique_ptr<UIObject>> children{};
    std::vector<std::unique_ptr<Component>> components{};
    UICanvas* canvas = nullptr;
    UIObject* parent = nullptr;
    
    virtual void DrawSelf() const;

    void LogBeginDrawSelf() const;
    void LogEndDrawSelf() const;

private:
    friend class Scene;
    void SetCanvas(UICanvas* newCanvas) { canvas = newCanvas; }

    void SetParent(GameObject* newParent);

    mutable int64_t beginDrawSelf;
    mutable int64_t endDrawSelf;
};
}
