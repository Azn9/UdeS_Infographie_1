#include "Api/Public/Component/Basic/Render/3D/LODRenderer.h"
#include <algorithm>
#include <ranges>
#include <string>
#include "Api/Public/Util/Util.h"

using namespace Util;

void PM3D_API::LODRenderer::Initialize()
{
    for (auto& renderer : subRenderers | std::views::values)
    {
        renderer->Initialize();
    }
}

void PM3D_API::LODRenderer::Update()
{
    if(subRenderers.empty())
        return;

    DirectX::XMFLOAT3 pos = parentObject->GetWorldPosition();
    DirectX::XMFLOAT3 camPos = parentObject->GetScene()->GetMainCamera()->GetWorldPosition();

    float camDistance = magnitude(pos - camPos);

    currentRenderer = nullptr;
    currentLOD = -1;
    
    int i = 0;
    for (auto& [maxDist, renderer] : subRenderers)
    {
        if(camDistance < maxDist)
        {
            currentRenderer = renderer.get();
            currentLOD = i;
            break;
        }
        
        ++i;
    }

    if(currentRenderer)
        currentRenderer->Update();
    
}

void PM3D_API::LODRenderer::PhysicsUpdate()
{
    if(!currentRenderer)
        return;
    
    currentRenderer->PhysicsUpdate();
}

void PM3D_API::LODRenderer::DrawSelf() const
{
    if(!currentRenderer)
        return;
    
    currentRenderer->DrawSelf();
}

void PM3D_API::LODRenderer::DrawDebugInfo() const
{
    if(!currentRenderer)
        return;

    ImGui::Text(("LOD Renderer: LOD" + std::to_string(currentLOD)).c_str());
    currentRenderer->DrawDebugInfo();
}

void PM3D_API::LODRenderer::AddRenderer(const float& maxDist, std::unique_ptr<Renderer>&& renderer)
{
    subRenderers.insert(std::make_pair(maxDist, std::move(renderer)));
}

void PM3D_API::LODRenderer::SetGameObject(GameObject* gameObject)
{
    Component::SetGameObject(gameObject);

    for (auto& renderer : subRenderers | std::views::values)
    {
        renderer->SetGameObject(parentObject);
    }
}
