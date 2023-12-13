#include "SpeedDisplay.h"

#include "Api/Public/GameObject/UICanvas.h"
#include "Core/Public/Core/MoteurWindows.h"
#include "GameTest/Components/SizeModifierComponent.h"
#include "GameTest/Scenes/MainScene.h"

void SpeedDisplay::Initialize()
{
    const auto sphere = static_cast<MainScene*>(canvas->GetScene())->GetShpere();
    sizeModifierComponent = sphere->GetComponent<SizeModifierComponent>();

    const auto pDispositif = PM3D_API::GameHost::GetInstance()->GetDispositif();

    bgTexture = PM3D::CMoteurWindows::GetInstance().GetTextureManager().GetNewTexture(
        L"sprite/game/speed_bar_bg.dds", pDispositif);
    frontTexture = PM3D::CMoteurWindows::GetInstance().GetTextureManager().GetNewTexture(
        L"sprite/game/speed_bar_front.dds", pDispositif);

    const auto position = DirectX::XMFLOAT2(
        50.f / 1920.f * pDispositif->GetLargeur(),
        50.f / 1080.f * pDispositif->GetHauteur()
    );
    const auto scale = DirectX::XMFLOAT2(
        497.f / 1920.f * pDispositif->GetLargeur(),
        200.f / 1080.f * pDispositif->GetHauteur()
    );

    const auto largeur = static_cast<float>(pDispositif->GetLargeur());
    const auto hauteur = static_cast<float>(pDispositif->GetHauteur());

    // Dimension en facteur
    const float facteurX = scale.x * 2.f / largeur;
    const float facteurY = scale.y * 2.f / hauteur;

    // Position en coordonnées logiques
    const float posX = position.x * 2.f / largeur - 1.f;
    const float posY = 1.f - position.y * 2.f / hauteur;

    matWVP = XMMatrixScaling(facteurX, facteurY, 1.0f)
        * XMMatrixTranslation(posX, -posY, 0.0f);

    frontShader = std::make_unique<PM3D_API::SpriteShader>(L"shader/Sprite1.fx");
    bgShader = std::make_unique<PM3D_API::SpeedDisplayShader>(L"shader/SpeedShader.fx");
}

void SpeedDisplay::Update()
{
    if (!sizeModifierComponent) return;

    const auto currentSize = sizeModifierComponent->currentSize;

    percent = currentSize / MAX_SIZE; // TODO

    float r = 0.f;
    float g = 0.f;

    if (currentSize > HALF_MAX_SIZE)
    {
        r = 1.f - (currentSize - HALF_MAX_SIZE) / HALF_MAX_SIZE;
        g = 1.f;
    }
    else
    {
        r = 1.f;
        g = currentSize / HALF_MAX_SIZE;
    }

    color = DirectX::XMFLOAT3(r, g, 0.f);
}

void SpeedDisplay::DrawSelf() const
{
    DrawBg();
    DrawFront();
}

void SpeedDisplay::DrawBg() const
{
    const auto pDispositif = PM3D_API::GameHost::GetInstance()->GetDispositif();
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    constexpr UINT stride = sizeof(PM3D::CSommetSprite);
    constexpr UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, bgShader->GetVertexBufferPtr(), &stride, &offset);

    pImmediateContext->IASetInputLayout(bgShader->GetVertexLayout());

    auto shaderParameters = PM3D_API::SpeedDisplayShader::SpeedDisplayShaderParameters{
        XMMatrixTranspose(matWVP),
        color,
        alpha,
        percent
    };

    bgShader->ApplyMaterialParameters(
        &shaderParameters,
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        0.0f,
        bgTexture->GetD3DTexture(),
        nullptr
    );

    bgShader->ApplyShaderParams();
    pImmediateContext->UpdateSubresource(bgShader->GetShaderParametersBuffer(), 0, nullptr, &shaderParameters, 0, 0);

    bgShader->GetPass()->Apply(0, pImmediateContext);

    pImmediateContext->Draw(6, 0);
}

void SpeedDisplay::DrawFront() const
{
    const auto pDispositif = PM3D_API::GameHost::GetInstance()->GetDispositif();
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    constexpr UINT stride = sizeof(PM3D::CSommetSprite);
    constexpr UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, frontShader->GetVertexBufferPtr(), &stride, &offset);

    pImmediateContext->IASetInputLayout(frontShader->GetVertexLayout());

    auto shaderParameters = PM3D_API::SpriteShader::SpriteShaderParameters{
        XMMatrixTranspose(matWVP),
        alpha
    };

    frontShader->ApplyMaterialParameters(
        &shaderParameters,
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        0.0f,
        frontTexture->GetD3DTexture(),
        nullptr
    );

    frontShader->ApplyShaderParams();
    pImmediateContext->UpdateSubresource(frontShader->GetShaderParametersBuffer(), 0, nullptr, &shaderParameters, 0, 0);

    frontShader->GetPass()->Apply(0, pImmediateContext);

    pImmediateContext->Draw(6, 0);
}
