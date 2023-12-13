#pragma once
#include <memory>

namespace PM3D_API
{
    class UIObject;

    class UIComponent
    {
    public:
        virtual ~UIComponent();

        virtual void Initialize();
        virtual void Update();
        virtual void UpdateMatrix();
        virtual void DrawSelf() const;

        virtual void DrawDebugInfo() const
        {
        }

        uint64_t GetBeginDrawSelf() const { return beginDrawSelf; }
        uint64_t GetEndDrawSelf() const { return endDrawSelf; }

    protected:
        UIObject* parentObject = nullptr;

        void LogBeginDrawSelf() const;
        void LogEndDrawSelf() const;

    private:
        friend class UIObject;
        void SetUIObject(UIObject* gameObject) { parentObject = gameObject; }

        mutable uint64_t beginDrawSelf;
        mutable uint64_t endDrawSelf;
    };
}
