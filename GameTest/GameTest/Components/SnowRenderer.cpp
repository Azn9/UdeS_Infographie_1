#include "SnowRenderer.h"

#include "SnowMover.h"
#include "Core/Public/Core/MoteurWindows.h"
#include "Core/Public/Mesh/CSommetMesh.h"
#include "Core/Public/Util/resource.h"
#include "Core/Public/Util/util.h"
#include "GameTest/Shader/SnowShader.h"

#define TEXTURE_SCALE 512

void SnowRenderer::Initialize()
{
    MeshRenderer::Initialize();

    const auto dispositif = PM3D_API::GameHost::GetInstance()->GetDispositif();

    sparklesTexture = PM3D::CMoteurWindows::GetInstance().GetTextureManager().
                                                          GetNewTexture(L"sparkles.dds", dispositif);

    const auto pD3DDevice = dispositif->GetD3DDevice();

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = TEXTURE_SCALE;
    textureDesc.Height = TEXTURE_SCALE;
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

    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
    textureDesc.Usage = D3D11_USAGE_STAGING;
    textureDesc.BindFlags = 0;
    textureDesc.MiscFlags = 0;

    stagingTexture = nullptr;
    PM3D::DXEssayer(pD3DDevice->CreateTexture2D(&textureDesc, nullptr, &stagingTexture));

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    PM3D::DXEssayer(pD3DDevice->CreateRenderTargetView(snowRVT, &renderTargetViewDesc, &snowRVTRTV));
}

void SnowRenderer::DrawSelf() const
{
    DrawRVT();

    const auto effect = shader->GetEffect();

    const auto snowRvtResource = effect->GetVariableByName("snowRVT")->AsShaderResource();
    PM3D::DXEssayer(snowRvtResource->SetResource(snowRVTResourceView));

    const auto sparkleTexture = effect->GetVariableByName("sparkleTexture")->AsShaderResource();
    PM3D::DXEssayer(sparkleTexture->SetResource(sparklesTexture->GetD3DTexture()));

    MeshRenderer::DrawSelf();

    snowRvtResource->SetResource(nullptr);
}

void SnowRenderer::DrawRVT() const
{
    const auto dispositif = PM3D_API::GameHost::GetInstance()->GetDispositif();
    const auto context = dispositif->GetImmediateContext();

    dispositif->ActiverMelangeAlpha();

    // Improvement possible : update fade only every 1/4th frame to save performance
    // Or disable the whole following block if we don't want the snow to fade back
    {
        context->CopyResource(stagingTexture, snowRVT);

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        PM3D::DXEssayer(context->Map(stagingTexture, 0, D3D11_MAP_READ_WRITE, 0, &mappedResource));

        for (int i = 0; i < TEXTURE_SCALE; ++i)
        {
            for (int j = 0; j < TEXTURE_SCALE; ++j)
            {
                const auto pixel = static_cast<unsigned char*>(mappedResource.pData)
                    + (i * mappedResource.RowPitch) + (j * 4);

                auto value = pixel[0]; // gray so all channels have the same value
                if (value == 0) continue;

                value -= 1;

                pixel[0] = value; // red
                pixel[1] = value; // green
                pixel[2] = value; // blue
            }
        }

        context->Unmap(stagingTexture, 0);
        context->CopyResource(snowRVT, stagingTexture);
    }

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // input layout des sommets
    context->IASetInputLayout(shader->GetVertexLayout());

    // Index buffer
    context->IASetIndexBuffer(shader->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

    constexpr UINT stride = sizeof(CSommetMesh);
    constexpr UINT offset = 0;
    context->IASetVertexBuffers(0, 1, shader->GetVertexBufferPtr(), &stride, &offset);

    context->OMSetRenderTargets(0, nullptr, nullptr);
    shader->GetPass()->Apply(0, context);

    // Bind ID3D11Texture2D as ID3D11RenderTargetView
    ID3D11RenderTargetView* tabRTV[1];
    tabRTV[0] = snowRVTRTV;
    context->OMSetRenderTargets(1, tabRTV, nullptr);

    dispositif->SetViewportDimension(TEXTURE_SCALE, TEXTURE_SCALE);

    const auto pos = parentObject->GetWorldPosition();

    const XMMATRIX viewProj = XMMatrixLookAtRH(
        XMVectorSet(pos.x, pos.y + 10.0f, pos.z, 1.0f),
        XMVectorSet(pos.x, pos.y, pos.z, 1.0f),
        XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)
    ) * XMMatrixOrthographicRH(
        20,
        20,
        0.1f,
        100.0f
    );

    const auto parameters = new SnowShader::SnowShaderParameters{
        XMMatrixTranspose(parentObject->GetMatWorld() * viewProj),
        XMMatrixTranspose(parentObject->GetMatWorld()),
        XMVectorSet(pos.x, pos.y + 10.0f, pos.z, 1.0f),
        XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f),
        XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
        XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        200.0f,
        1.0f,
        false,
        false,
        true,
    };

    const auto effect = shader->GetEffect();

    ID3DX11EffectConstantBuffer* pCB = effect->GetConstantBufferByName("param");
    pCB->SetConstantBuffer(shader->GetShaderParametersBuffer());

    context->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, parameters, 0, 0);

    std::vector<PM3D_API::ShaderLightDefaultParameters> shaderLightsParameters{};

    for (const auto& child : parentObject->GetScene()->GetChildren())
    {
        if (!child) continue;

        // TODO : use only objects that are near the snow
        // TODO : use only objects that are in the snow
        // TODO : use only objects that are in the camera's view
        // TODO : list the available objects and skip the whole render if there are none

        if (child->HasComponent<SnowMover>())
        {
            shaderLightsParameters.push_back(PM3D_API::ShaderLightDefaultParameters{
                XMMatrixIdentity(),
                XMVectorSet(child->GetWorldPosition().x, child->GetWorldPosition().y + 1.0f,
                            child->GetWorldPosition().z, 1.0f),
                XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
                XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),
                XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
                XMVectorSet(0.6f, 0.6f, 0.6f, 1.0f),
                3.0f,
                0.0f,
                0.0f,
                true,
                static_cast<int>(PM3D_API::LightType::POINT),
                XMFLOAT3(0.0f, 0.0f, 0.0f)
            });
        }
    }

    if (shaderLightsParameters.empty())
    {
        //std::cerr << "No snow mover found" << std::endl;
    }

    for (int i = 0; i < 10 - static_cast<int>(shaderLightsParameters.size()); ++i)
    {
        shaderLightsParameters.push_back(PM3D_API::ShaderLightDefaultParameters{});
    }

    D3D11_BUFFER_DESC lightParametersBufferDesc;
    ZeroMemory(&lightParametersBufferDesc, sizeof(lightParametersBufferDesc));
    lightParametersBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightParametersBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    lightParametersBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    lightParametersBufferDesc.StructureByteStride = sizeof(PM3D_API::ShaderLightDefaultParameters);
    lightParametersBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightParametersBufferDesc.ByteWidth = sizeof(PM3D_API::ShaderLightDefaultParameters) * static_cast<UINT>(
        shaderLightsParameters.size());

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = shaderLightsParameters.data();

    ID3D11Buffer* lightBuffer = nullptr;
    const auto pD3DDevice = PM3D_API::GameHost::GetInstance()->GetDispositif()->GetD3DDevice();
    PM3D::DXEssayer(pD3DDevice->CreateBuffer(&lightParametersBufferDesc, &initData, &lightBuffer));

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.ElementWidth = static_cast<UINT>(shaderLightsParameters.size());

    ID3D11ShaderResourceView* lightSRV = nullptr;
    PM3D::DXEssayer(pD3DDevice->CreateShaderResourceView(lightBuffer, &srvDesc, &lightSRV));

    ID3DX11EffectShaderResourceVariable* variableTexture;
    variableTexture = effect->GetVariableByName("lights")->AsShaderResource();
    variableTexture->SetResource(lightSRV);

    shader->GetPass()->Apply(0, context);

    for (unsigned int i = 0; i < mesh->object_count; ++i)
    {
        const auto [name, face_count, face_offset, index_offset] = mesh->objects[i];
        const unsigned indexStart = index_offset;

        unsigned int indexDrawAmount;
        if (mesh->object_count > 1)
        {
            indexDrawAmount = mesh->objects[i + 1].index_offset - indexStart;
        }
        else
        {
            indexDrawAmount = mesh->index_count;
        }

        if (!indexDrawAmount)
        {
            continue;
        }

        context->DrawIndexed(indexDrawAmount, indexStart, 0);
    }

    dispositif->ResetViewportDimension();

    ID3D11RenderTargetView* nullRTV = nullptr;
    context->OMSetRenderTargets(1, &nullRTV, nullptr);
    shader->GetPass()->Apply(0, context);

    tabRTV[0] = dispositif->GetRenderTargetView();
    context->OMSetRenderTargets(1, tabRTV, dispositif->GetDepthStencilView());

    dispositif->DesactiverMelangeAlpha();

    // Release resources
    lightSRV->Release();
    lightBuffer->Release();
    variableTexture->Release();
}
