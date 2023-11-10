#pragma once
#include "Api/Public/GameObject/UIObject.h"

class TestUIObject : public PM3D_API::UIObject
{
public:
    TestUIObject(
        const std::string& name,
        const DirectX::XMFLOAT2& scale,
        const DirectX::XMFLOAT2& position,
        const bool relativeScale = false,
        const bool relativePosition = false
    ) : UIObject(name, scale, position, 0, relativeScale, relativePosition)
    {
    }

    void Initialize() override;
    void Update() override;
};
