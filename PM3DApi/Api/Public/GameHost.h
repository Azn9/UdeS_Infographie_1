#pragma once

#include "Core/Public/Core/dispositifD3D11.h"
#include "Core/Public/Object/PanneauPE.h"
#include "Core/Public/Texture/Texture.h"
#include "Debug/DebugRenderer.h"
#include "Scene/Scene.h"

namespace PM3D_API
{
    class GameHost
    {
    public:
        // === Singleton ===

        GameHost() = default;
        virtual ~GameHost();
        
        void Update() const;
        void PhysicsUpdate() const;
        void InitializeScene();

        PM3D::CDispositifD3D11* GetDispositif() const { return dispositif; }
        void SetDispositif(PM3D::CDispositifD3D11* dispositif);

        PM3D::CPanneauPE* GetPostEffectPlane() const { return postEffectPlane; }
        void SetPostEffectPlane(PM3D::CPanneauPE* plane) { postEffectPlane = plane; }
        
        float GetAspectRatio() const;
        float GetScreenWidth() const;
        float GetScreenHeight() const;

        GameHost(GameHost&) = delete;
        void operator =(GameHost&) = delete;
        GameHost(GameHost&&) = delete;
        void operator =(GameHost&&) = delete;

        static GameHost* GetInstance();

        // === Interface ===

        virtual void Initialize() = 0;
        virtual void InitializePostProcessParam() const {}
        virtual void Draw();
        virtual void DrawUI();

        Scene* GetScene() const { return scene; }

        // === Other ===
        void AddDebugRenderer(std::unique_ptr<DebugRenderer>&& child) { debugRenderers.push_back(std::move(child)); }
        const std::vector<std::unique_ptr<DebugRenderer>>& GetDebugRenderers() { return debugRenderers; }

        virtual void SetScene(Scene* newScene);
    protected:
        std::vector<std::unique_ptr<DebugRenderer>> debugRenderers;

        PM3D::CPanneauPE* postEffectPlane;

    private:
        Scene* scene;
        PM3D::CDispositifD3D11* dispositif;

    };
}
