#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Core/Public/Core/MoteurWindows.h"
#include "Core/Public/Util/Util.h"
#include "Api/Public/Shader/Basic/SpriteShader.h"

PM3D_API::SpriteRenderer::SpriteRenderer(const std::wstring& textureName) : matWVP(XMMatrixIdentity())
{
    texture = PM3D::CMoteurWindows::GetInstance().GetTextureManager().GetNewTexture(
        textureName, GameHost::GetInstance()->GetDispositif());
    shader = std::make_unique<SpriteShader>(L"shader/Sprite1.fx");

    if (!texture)
        throw std::exception("SpriteRenderer::SpriteRenderer() : texture is null");
}

void PM3D_API::SpriteRenderer::Initialize()
{
    ID3D11Resource* pResource;
    ID3D11Texture2D* pTextureInterface = nullptr;

    texture->GetD3DTexture()->GetResource(&pResource);
    pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);

    D3D11_TEXTURE2D_DESC desc;
    pTextureInterface->GetDesc(&desc);

    PM3D::DXRelacher(pResource);
    PM3D::DXRelacher(pTextureInterface);

    textureSizeX = static_cast<float>(desc.Width);
    textureSizeY = static_cast<float>(desc.Height);

    UpdateMatrix();
}

void PM3D_API::SpriteRenderer::Update()
{
    UpdateMatrix();
}

void PM3D_API::SpriteRenderer::UpdateMatrix()
{
    const auto pDispositif = GameHost::GetInstance()->GetDispositif();

    const auto position = parentObject->GetPosition() + positionOffset;
    const auto scale = parentObject->GetScale();

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
}

void PM3D_API::SpriteRenderer::DrawSelf() const
{
    if (!texture)
        return;

    if (parentObject->GetAlpha() == 0.f)
        return;

    const auto pDispositif = GameHost::GetInstance()->GetDispositif();
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    constexpr UINT stride = sizeof(PM3D::CSommetSprite);
    constexpr UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, shader->GetVertexBufferPtr(), &stride, &offset);

    pImmediateContext->IASetInputLayout(shader->GetVertexLayout());

    auto shaderParameters = SpriteShader::SpriteShaderParameters{
        XMMatrixTranspose(matWVP),
        parentObject->GetAlpha()
    };

    shader->ApplyMaterialParameters(
        &shaderParameters,
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        0.0f,
        texture->GetD3DTexture(),
        nullptr
    );

    shader->ApplyShaderParams();
    pImmediateContext->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, &shaderParameters, 0, 0);

    shader->GetPass()->Apply(0, pImmediateContext);

    pImmediateContext->Draw(6, 0);
}
