#include "ShadowProcessor.h"

#include <algorithm>

#include "Api/Public/GameHost.h"
#include "Core/Public/Util/resource.h"
#include "Core/Public/Util/Util.h"

enum
{
    SHADOW_MAP_SIZE = 2048
};

ShadowProcessor::~ShadowProcessor()
{
    depthStencilView->Release();
    pDepthTexture->Release();
    pDepthTextureResourceView->Release();
}

void ShadowProcessor::Initialize()
{
    const auto pD3DDevice = PM3D_API::GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

    D3D11_TEXTURE2D_DESC depthTextureDesc;
    ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
    depthTextureDesc.Width = SHADOW_MAP_SIZE;
    depthTextureDesc.Height = SHADOW_MAP_SIZE;
    depthTextureDesc.MipLevels = 1;
    depthTextureDesc.ArraySize = 1;
    depthTextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthTextureDesc.SampleDesc.Count = 1;
    depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

    PM3D::DXEssayer(
        pD3DDevice->CreateTexture2D(&depthTextureDesc, nullptr, &pDepthTexture),
        DXE_ERREURCREATIONTEXTURE
    );

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
    ZeroMemory(&descDSView, sizeof(descDSView));
    descDSView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSView.Texture2D.MipSlice = 0;

    PM3D::DXEssayer(
        pD3DDevice->CreateDepthStencilView(pDepthTexture, &descDSView, &depthStencilView),
        DXE_ERREURCREATIONDEPTHSTENCILTARGET
    );

    D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
    ZeroMemory(&sr_desc, sizeof(sr_desc));
    sr_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    sr_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    sr_desc.Texture2D.MipLevels = 1;
    PM3D::DXEssayer(pD3DDevice->CreateShaderResourceView(pDepthTexture, &sr_desc, &pDepthTextureResourceView));
}

std::vector<PM3D_API::ShaderLightDefaultParameters> ShadowProcessor::ProcessLights()
{
    const auto& lights = PM3D_API::GameHost::GetInstance()->GetScene()->GetLights();
    const auto lightCount = lights.size();

    auto finalLights = std::vector<PM3D_API::Light*>{};

    if (lightCount > static_cast<std::vector<std::unique_ptr<PM3D_API::Light>>::size_type>(10))
    {
        std::vector<PM3D_API::Light*> ambiantLights{};
        std::vector<PM3D_API::Light*> directionalLights{};
        std::vector<PM3D_API::Light*> otherLights{};

        // Trop de lumières, on prend 1. Ambiant puis 2. Directional puis 3 par distance

        for (auto& light : lights)
        {
            if (light->GetType() == PM3D_API::LightType::AMBIANT)
            {
                ambiantLights.push_back(light);
            }
            else if (light->GetType() == PM3D_API::LightType::DIRECTIONAL)
            {
                directionalLights.push_back(light);
            }
            else
            {
                // TODO: Sort by distance
                otherLights.push_back(light);
            }
        }

        if (ambiantLights.size() > 10) // ... ????
        {
            // Copy the first 10 ambiant lights
            std::copy_n(ambiantLights.begin(), 10, finalLights.begin());
        }
        else
        {
            int remaining = 10 - static_cast<int>(ambiantLights.size());

            if (directionalLights.size() > remaining)
            {
                // Adds only the first remaining directional lights
                std::copy_n(directionalLights.begin(), remaining, finalLights.begin() + ambiantLights.size());
            }
            else
            {
                remaining -= static_cast<int>(directionalLights.size());

                // Adds the remaining lights
                std::copy_n(otherLights.begin(), remaining,
                            finalLights.begin() + ambiantLights.size() + directionalLights.size());
            }
        }
    }
    else
    {
        std::ranges::for_each(lights.begin(), lights.end(), [&finalLights](PM3D_API::Light* light)
        {
            finalLights.push_back(light);
        });
    }

    std::vector<PM3D_API::ShaderLightDefaultParameters> shaderLightsParameters{};
    std::ranges::for_each(finalLights, [&shaderLightsParameters](PM3D_API::Light* light)
    {
        if (light->GetType() == PM3D_API::LightType::DIRECTIONAL)
        {
            const auto& cameraPlayer = PM3D_API::GameHost::GetInstance()->GetScene()->GetMainCamera();

            const auto lightDirection = light->GetWorldDirection();

            const auto positionCameraPlayer = cameraPlayer->GetWorldPosition();
            const auto focusPointCameraPlayer = cameraPlayer->GetFocusPoint();
            const auto nearDistCameraPlayer = cameraPlayer->getNearDist();
            const auto farDistCameraPlayer = cameraPlayer->getFarDist();

            auto directionCameraPlayer = DirectX::XMFLOAT3(
                focusPointCameraPlayer.m128_f32[0] - positionCameraPlayer.x,
                focusPointCameraPlayer.m128_f32[1] - positionCameraPlayer.y,
                focusPointCameraPlayer.m128_f32[2] - positionCameraPlayer.z
            );

            const auto normDirection = sqrt(
                directionCameraPlayer.x * directionCameraPlayer.x + directionCameraPlayer.y * directionCameraPlayer.y +
                directionCameraPlayer.z * directionCameraPlayer.z);

            directionCameraPlayer.x = directionCameraPlayer.x / normDirection;
            directionCameraPlayer.y = directionCameraPlayer.y / normDirection;
            directionCameraPlayer.z = directionCameraPlayer.z / normDirection;

            const auto focusPoint = DirectX::XMVectorSet(
                positionCameraPlayer.x + directionCameraPlayer.x * viewHeightDirectionnalLight / 3.f,
                positionCameraPlayer.y + directionCameraPlayer.y * viewHeightDirectionnalLight / 3.f,
                positionCameraPlayer.z + directionCameraPlayer.z * viewHeightDirectionnalLight / 3.f,
                1.0f
            );

            const auto worldPosition = DirectX::XMFLOAT3(
                focusPoint.m128_f32[0] - 200.f * lightDirection.x,
                focusPoint.m128_f32[1] - 200.f * lightDirection.y,
                focusPoint.m128_f32[2] - 200.f * lightDirection.z
            );

            light->SetWorldPosition(worldPosition);

            const auto mVLight = DirectX::XMMatrixLookAtRH(
                DirectX::XMVectorSet(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f), // position
                focusPoint, // lookAt
                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f) // upvector
            );

            constexpr float nearPlane = 0.2f;
            constexpr float farPlane = 500.0f;

            const auto mPLight = DirectX::XMMatrixOrthographicRH(
                viewWidthDirectionnalLight,
                viewHeightDirectionnalLight,
                nearPlane,
                farPlane
            );

            const DirectX::XMMATRIX mvp = mVLight * mPLight;

            shaderLightsParameters.push_back(light->GetShaderLightDefaultParameters(mvp));
        }
        else if (light->GetType() == PM3D_API::LightType::POINT)
        {
            shaderLightsParameters.push_back(light->GetShaderLightDefaultParameters(nullptr));
        }
        else
        {
            shaderLightsParameters.push_back(light->GetShaderLightDefaultParameters(nullptr));
        }
    });

    for (int i = 0; i < 10 - lightCount; ++i)
    {
        shaderLightsParameters.push_back(PM3D_API::DefaultShaderLightDefaultParameters);
    }

    return shaderLightsParameters;
}

void ShadowProcessor::ProcessShadow()
{
    if (!scene)
        return;

    const auto pDispositif = PM3D_API::GameHost::GetInstance()->GetDispositif();
    const auto pImmediateContext = pDispositif->GetImmediateContext();

    auto lightParameters = ProcessLights();

    for (auto& [matWorldViewProj, position, direction, ambient, diffuse, specular, specularPower, innerAngle, outerAngle
             ,
             initialized, lightType, padding] : lightParameters)
    {
        if (!initialized)
        {
            continue;
        }

        // TODO : switch DSV
        pImmediateContext->OMSetRenderTargets(0, nullptr, depthStencilView);
        pImmediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
        pDispositif->SetViewportDimension(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

        const auto orthographic = lightType == static_cast<int>(PM3D_API::LightType::DIRECTIONAL);

        const auto cameraPosition = DirectX::XMFLOAT3{
            position.m128_f32[0],
            position.m128_f32[1],
            position.m128_f32[2]
        };

        const auto cameraFocusPoint = DirectX::XMVectorSet(
            position.m128_f32[0] + direction.m128_f32[0],
            position.m128_f32[1] + direction.m128_f32[1],
            position.m128_f32[2] + direction.m128_f32[2],
            1.0f
        );
        /*const auto cameraFocusPoint = DirectX::XMVectorSet(
            positionCameraPlayer.x,
            positionCameraPlayer.y,
            positionCameraPlayer.z,
            1.0f
        );*/

        auto camera = PM3D_API::Camera(
            "Temp camera",
            orthographic ? PM3D_API::Camera::ORTHOGRAPHIC : PM3D_API::Camera::CameraType::PERSPECTIVE,
            cameraPosition,
            cameraFocusPoint,
            {0, 1, 0, 0}
        );

        camera.SetFarDist(500.f);

        if (!orthographic)
        {
            camera.SetFieldOfView(90); // TODO
        }
        else
        {
            camera.SetViewHeight(viewHeightDirectionnalLight); // change the field of view of the orthographic camera
            camera.SetViewWidth(viewWidthDirectionnalLight);
        }

        camera.UpdateInternalMatrices();

        scene->DrawShadow(camera);
    }

    pDispositif->ResetViewportDimension();
    pDispositif->SetNormalRSState();

    ID3D11RenderTargetView* tabRTV[1];
    tabRTV[0] = pDispositif->GetRenderTargetView();
    pImmediateContext->OMSetRenderTargets(1, tabRTV, pDispositif->GetDepthStencilView());
}
