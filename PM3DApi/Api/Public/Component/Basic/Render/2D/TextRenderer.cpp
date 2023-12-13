#include "TextRenderer.h"

#include "Core/Public/Core/MoteurWindows.h"
#include "Api/Public/Shader/Basic/TextShader.h"

PM3D_API::TextRenderer::TextRenderer(
    const FontLoader& fontLoader,
    std::string text,
    const DirectX::XMFLOAT2& positionOffset,
    const DirectX::XMFLOAT3& color,
    const float scale
) : text(std::move(text)),
    fontLoader(fontLoader),
    positionOffset(positionOffset),
    color(color),
    scale(scale)
{
    shader = std::make_unique<TextShader>(L"shader/TextShader.fx");
}

void PM3D_API::TextRenderer::Initialize()
{
    RefreshData();
}

void PM3D_API::TextRenderer::RefreshData()
{
    parameters.clear();

    const auto pDispositif = GameHost::GetInstance()->GetDispositif();
    const auto largeur = static_cast<float>(pDispositif->GetLargeur());
    const auto hauteur = static_cast<float>(pDispositif->GetHauteur());
    const auto parentPosition = parentObject->GetPosition();

    int currentPosX = positionOffset.x;
    int currentPosY = positionOffset.y;

    for (const char value : text)
    {
        if (!fontLoader.HasData(value))
        {
            std::cerr << "TextRenderer::Initialize() : character '" << value << "' not found in font!" << std::endl;
            continue;
        }

        const auto [c, x, y, width, height, xoffset, yoffset, xadvance] = fontLoader.GetCharData(value);

        if (width == 0 && height == 0) // Space
        {
            currentPosX += xadvance * scale;
            continue;
        }

        const int finalHeight = height * scale;
        const int finalWidth = width * scale;

        const XMFLOAT2 uvStart = {static_cast<float>(x), static_cast<float>(y)};
        const XMFLOAT2 uvEnd = {static_cast<float>((x + width)), static_cast<float>((y + height))};

        const float facteurX = finalWidth * 2.f / largeur;
        const float facteurY = finalHeight * 2.f / hauteur;

        const float posX = (parentPosition.x + currentPosX) * 2.f / largeur - 1.f;
        const float posY = 1.f - (parentPosition.y + currentPosY) * 2.f / hauteur;

        const auto matWVP = XMMatrixScaling(facteurX, facteurY, 1.0f)
            * XMMatrixTranslation(posX, -posY, 0.0f);

        parameters.push_back(TextShader::TextShaderParameters{
            XMMatrixTranspose(matWVP),
            uvStart,
            uvEnd,
            parentObject->GetAlpha()
        });

        currentPosX += xadvance * scale;
    }
}

void PM3D_API::TextRenderer::DrawSelf() const
{
    if (parentObject->GetAlpha() == 0.f)
        return;

    const auto pDispositif = GameHost::GetInstance()->GetDispositif();
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    constexpr UINT stride = sizeof(PM3D::CSommetSprite);
    constexpr UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, shader->GetVertexBufferPtr(), &stride, &offset);

    pImmediateContext->IASetInputLayout(shader->GetVertexLayout());

    for (TextShader::TextShaderParameters shaderParameters : parameters)
    {
        shader->ApplyMaterialParameters(
            &shaderParameters,
            DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
            DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
            DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
            0.0f,
            fontLoader.GetTexture()->GetD3DTexture(),
            nullptr
        );

        shader->ApplyShaderParams();
        pImmediateContext->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, &shaderParameters, 0, 0);

        shader->GetPass()->Apply(0, pImmediateContext);

        pImmediateContext->Draw(6, 0);
    }
}
