#pragma once

#include "Component/ContainerComponent.h"

namespace PM3D_API
{
    class GameHost
    {
    public:
        // === Singleton ===

        GameHost() = default;
        virtual ~GameHost();

        GameHost(GameHost&) = delete;
        void operator =(GameHost&) = delete;
        GameHost(GameHost&&) = delete;
        void operator =(GameHost&&) = delete;

        static GameHost* GetInstance();

        // === Interface ===

        virtual void Initialize() = 0;
        virtual void Draw();

    protected:
        virtual void SetBaseComponent(ContainerComponent* component);

    private:
        ContainerComponent* baseComponent;
    };
}
