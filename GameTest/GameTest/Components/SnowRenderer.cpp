#include "SnowRenderer.h"

#include "../../../PetitMoteur3D/Core/Public/Util/resource.h"
#include "../../../PetitMoteur3D/Core/Public/Util/util.h"

void SnowRenderer::Initialize()
{
    MeshRenderer::Initialize();

    const auto pD3DDevice = PM3D_API::GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = 512;
    textureDesc.Height = 512;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    PM3D::DXEssayer(pD3DDevice->CreateTexture2D(&textureDesc, nullptr, &snowRVT), DXE_ERREURCREATIONTEXTURE);

    D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
    ZeroMemory(&sr_desc, sizeof(sr_desc));
    sr_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sr_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    sr_desc.Texture2D.MostDetailedMip = 0;
    sr_desc.Texture2D.MipLevels = 1;
    PM3D::DXEssayer(pD3DDevice->CreateShaderResourceView(snowRVT, &sr_desc, &snowRVTResourceView));

    D3D11_TEXTURE2D_DESC stagingDesc;
    ZeroMemory(&stagingDesc, sizeof(stagingDesc));
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.BindFlags = 0;
    stagingDesc.MiscFlags = 0;

    stagingTexture = nullptr;
    PM3D::DXEssayer(pD3DDevice->CreateTexture2D(&stagingDesc, nullptr, &stagingTexture));
}

void SnowRenderer::DrawSelf() const
{
    DrawRVT();

    MeshRenderer::DrawSelf();
}

void SnowRenderer::DrawRVT() const
{
    const auto context = PM3D_API::GameHost::GetInstance()->GetDispositif()->GetImmediateContext();
    context->CopyResource(stagingTexture, snowRVT);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    PM3D::DXEssayer(context->Map(stagingTexture, 0, D3D11_MAP_READ_WRITE, 0, &mappedResource));

    for (int i = 0; i < 512; ++i)
    {
        for (int j = 0; j < 512; ++j)
        {
            const auto pixel = static_cast<unsigned char*>(mappedResource.pData)
                + (i * mappedResource.RowPitch) + (j * 4);

            const auto value = pixel[0]; // gray so all channels have the same value
            if (value == 0) continue;

            pixel[0] = value - 1; // red
            pixel[1] = value - 1; // green
            pixel[2] = value - 1; // blue
            pixel[3] = 0xFF; // alpha
        }
    }

    context->Unmap(stagingTexture, 0);
    context->CopyResource(snowRVT, stagingTexture);

    // TODO : set snowRVT as render target
    // TODO : render snowRVT
    // TODO : set back default render target
    // TODO : set snowRVT as shader resource view
}
