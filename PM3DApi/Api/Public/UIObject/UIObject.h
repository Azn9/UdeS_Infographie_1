#pragma once

#include <DirectXMath.h>
#include <memory>
#include <string>
#include <vector>

#include "../../Public/Component/UIComponent.h"
#include "../../Public/Util/Instanceof.h"

namespace PM3D_API
{
    class UICanvas;

    //@formatter:off
    enum class Anchor
    {
        TOP_LEFT,       TOP_MIDDLE,     TOP_RIGHT,
        MIDDLE_LEFT,    MIDDLE,         MIDDLE_RIGHT,
        BOTTOM_LEFT,    BOTTOM_MIDDLE,  BOTTOM_RIGHT
    };

    enum class ScaleMode
    {
        BOTH_RELATIVE, NONE_RELATIVE, RELATIVE_X, RELATIVE_Y 
    };
    //@formatter:on

    class UIObject
    {
    public:
        UIObject(
            const std::string& name,
            UIObject* parent,
            const DirectX::XMFLOAT2& scale,
            const DirectX::XMFLOAT2& position = {0.f, 0.f},
            const ScaleMode& scaleMode = ScaleMode::NONE_RELATIVE,
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

        template <typename T, template_extends<T, UIComponent>  = 0>
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

        DirectX::XMFLOAT2 GetScreenPosition() const { return screenPosition; }
        DirectX::XMFLOAT2 GetRelativePosition() const { return relativePosition; }
        DirectX::XMFLOAT2 GetTopLeftPosition() const { return topLeftPosition; }

        void SetScreenPosition(const DirectX::XMFLOAT2& newOriginPosition);
        void SetRelativePosition(const DirectX::XMFLOAT2& newOriginPosition);

        DirectX::XMFLOAT2 GetScreenScale() const { return screenScale; }
        DirectX::XMFLOAT2 GetRelativeScale() const { return relativeScale; }

        void SetScreenScale(const DirectX::XMFLOAT2& newScale);
        void SetRelativeScale(const DirectX::XMFLOAT2& newScale);

    protected:
        std::string name = "Unnamed UI Object";

        Anchor anchor;
        Anchor origin;

        DirectX::XMFLOAT2 relativePosition;
        DirectX::XMFLOAT2 screenPosition;
        DirectX::XMFLOAT2 topLeftPosition;

        ScaleMode scaleMode;
        DirectX::XMFLOAT2 relativeScale;

        DirectX::XMFLOAT2 screenScale;

        std::vector<std::unique_ptr<UIObject>> children{};
        std::vector<std::unique_ptr<UIComponent>> components{};

        friend class UICanvas;
        UICanvas* canvas = nullptr;
        UIObject* parent = nullptr;

        virtual void DrawSelf() const;

        void LogBeginDrawSelf() const;
        void LogEndDrawSelf() const;

        void UpdateScale();
        void UpdatePosition();

    private:
        friend class Scene;
        void SetCanvas(UICanvas* newCanvas) { canvas = newCanvas; }

        mutable int64_t beginDrawSelf;
        mutable int64_t endDrawSelf;
        
        void UpdateTopLeftPosition();
    };
}
