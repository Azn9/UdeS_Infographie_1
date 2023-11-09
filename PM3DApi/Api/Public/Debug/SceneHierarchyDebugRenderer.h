#pragma once
#include "DebugRenderer.h"
#include "Api/Public/GameObject/GameObject.h"

namespace PM3D_API
{
    class SceneHierarchyDebugRenderer final : public DebugRenderer
    {
    public:
        SceneHierarchyDebugRenderer() = default;
        ~SceneHierarchyDebugRenderer() override = default;
        void Draw() override;

    private:
        const PM3D_API::GameObject* selectedObject = nullptr;

        void DisplayChild(const PM3D_API::GameObject* gameObject);
    };
}
