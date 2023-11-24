#include "ShadowProcessor.h"

#include "Api/Public/GameHost.h"

void ShadowProcessor::ProcessShadow()
{
	if (!scene)
		return;

	const auto pDispositif = PM3D_API::GameHost::GetInstance()->GetDispositif();
	const auto pImmediateContext = pDispositif->GetImmediateContext();

	// TODO : calculer les lights
	
	pImmediateContext->OMSetRenderTargets(0, nullptr, depthStencilView);
	pImmediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	pDispositif->SetViewportDimension(512,512);
	
	scene->DrawShadow();
	
	ID3D11RenderTargetView* tabRTV[1];
	tabRTV[0] = pDispositif->GetRenderTargetView();
	pImmediateContext->OMSetRenderTargets(1, tabRTV, pDispositif->GetDepthStencilView());
}
