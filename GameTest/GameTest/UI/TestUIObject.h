#pragma once
#include "../../../PM3DApi/Api/Public/UIObject/UIObject.h"

class TestUIObject : public PM3D_API::UIObject
{
public:
    TestUIObject(
        const std::string& name,
        const DirectX::XMFLOAT2& scale,
        UIObject* parent,
        const PM3D_API::ScaleMode& scaleMode,
        const DirectX::XMFLOAT2& position,
        const PM3D_API::Anchor& anchor,
        const PM3D_API::Anchor& origin
    ) : UIObject(name, parent, scale, position, scaleMode, anchor, origin)
    {
    }

    void Initialize() override;
};
