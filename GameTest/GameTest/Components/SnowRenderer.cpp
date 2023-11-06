#include "SnowRenderer.h"

void SnowRenderer::Initialize()
{
    MeshRenderer::Initialize();

    
}

void SnowRenderer::DrawSelf() const
{
    DrawRVT();
    // TODO : add snow RVT
    
    MeshRenderer::DrawSelf();
}

void SnowRenderer::DrawRVT() const
{
    
}
