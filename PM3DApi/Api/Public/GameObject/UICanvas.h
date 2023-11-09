#pragma once

#include "UIObject.h"

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
    private:
        bool isDeleted = false;
        std::vector<std::unique_ptr<UIObject>> children{};
    };
}
