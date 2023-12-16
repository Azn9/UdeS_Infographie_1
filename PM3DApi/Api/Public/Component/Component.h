#pragma once
#include <memory>

namespace PM3D_API
{
    class GameObject;

    class Component
    {
    public:
        virtual ~Component();

        virtual void Initialize();
        virtual void Update();
        virtual void PhysicsUpdate();
        virtual void DrawSelf() const;
        virtual void DrawShadowSelf(const class Camera& camera) const;

        virtual void DrawDebugInfo() const
        {
        }

        uint64_t GetBeginDrawSelf() const { return beginDrawSelf; }
        uint64_t GetEndDrawSelf() const { return endDrawSelf; }

    protected:
        GameObject* parentObject = nullptr;

        void LogBeginDrawSelf() const;
        void LogEndDrawSelf() const;

    private:
        friend class GameObject;
        void SetGameObject(GameObject* gameObject) { parentObject = gameObject; }

        mutable uint64_t beginDrawSelf;
        mutable uint64_t endDrawSelf;
    };
}
