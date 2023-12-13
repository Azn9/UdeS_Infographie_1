#pragma once

#include "UIObject.h"
#include "Api/Private/Input/MouseClickEvent.h"

namespace PM3D_API
{
    class UICanvas : public UIObject
    {
    public:
        UICanvas();

        ~UICanvas() override;

        void Initialize() override;
        void Update() override;
        void Draw() override;
        void DrawSelf() const override;

        void AddChild(std::unique_ptr<UIObject>&& child);

        void SetScale(const DirectX::XMFLOAT2& newScale) override;
        void UpdateHover(int x, int y) const;

    private:
        bool isDeleted = false;
        std::vector<std::unique_ptr<UIObject>> children{};

        void UpdateClick(const MouseClickEvent& mouseClickEvent);
    };
}
