#include "Api/Public/Component/Basic/Render/3D/BillboardRenderer.h"

#include "Core/Public/Core/MoteurWindows.h"
#include "Core/Public/Texture/GestionnaireDeTextures.h"
#include "Core/Public/Util/util.h"

PM3D_API::BillboardRenderer::~BillboardRenderer()
{
}

void PM3D_API::BillboardRenderer::Initialize()
{
    const auto pDispositif = GameHost::GetInstance()->GetDispositif();
    texture = PM3D::CMoteurWindows::GetInstance().GetTextureManager().GetNewTexture(textureName, pDispositif);

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
}

void PM3D_API::BillboardRenderer::DrawSelf() const
{
    LogBeginDrawSelf();

    const auto pTexture = texture->GetD3DTexture();

    // Obtenir le contexte
    const auto pDispositif = GameHost::GetInstance()->GetDispositif();
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(shader->GetVertexLayout());

    // Index buffer
    pImmediateContext->IASetIndexBuffer(shader->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

    // Vertex buffer
    constexpr UINT stride = sizeof(PM3D::CSommetSprite);
    constexpr UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, shader->GetVertexBufferPtr(), &stride, &offset);

    const auto camera = GameHost::GetInstance()->GetScene()->GetMainCamera();
    const auto cameraPosition = camera->GetWorldPosition();
    const auto cameraDirection = camera->GetWorldDirection();

    const auto position = parentObject->GetWorldPosition();
    XMFLOAT3 scale = parentObject->GetWorldScale();
    scale.x *= textureSizeX * selfScale.x;
    scale.y *= textureSizeY * selfScale.y;
    scale.z = 1.f;

    XMMATRIX worldMatrix;

    const auto cameraPosVec = XMVectorSet(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);
    const auto cameraDirVec = XMVectorSet(cameraDirection.x, cameraDirection.y, cameraDirection.z, 0.0f);
    const auto cameraUpVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    switch (alignment)
    {
    case BillboardAlignment::Z:
        worldMatrix = XMMatrixScaling(scale.x, scale.y, 1.0f) * XMMatrixLookAtRH(
            XMVectorSet(position.x, position.y, position.z, 1.0f), cameraPosVec, cameraUpVec) * XMMatrixTranslation(
            position.x, position.y, position.z);
        break;
    case BillboardAlignment::XZ:
        worldMatrix = XMMatrixScaling(scale.x, scale.y, 1.0f) * XMMatrixLookAtRH(
            XMVectorSet(position.x, cameraPosition.y, position.z, 1.0f), cameraPosVec,
            cameraUpVec) * XMMatrixTranslation(position.x, position.y, position.z);
        break;
    case BillboardAlignment::YZ:
        worldMatrix = XMMatrixScaling(1.0f, scale.y, scale.z) * XMMatrixLookAtRH(
            XMVectorSet(cameraPosition.x, position.y, position.z, 1.0f), cameraPosVec,
            cameraUpVec) * XMMatrixTranslation(position.x, position.y, position.z);
        break;
    case BillboardAlignment::XYZ:
        worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixLookAtRH(
            XMVectorSet(position.x, position.y, position.z, 1.0f), cameraPosVec, cameraUpVec) * XMMatrixTranslation(
            position.x, position.y, position.z);
        break;
    default: ;
    }

    const XMMATRIX viewProj = camera->GetMatViewProj();

    /*
    const auto shaderParameters = shader->PrepareParameters(
        XMMatrixTranspose(worldMatrix * viewProj),
        XMMatrixTranspose(worldMatrix)
    );
    */

    auto shaderParameters = SpriteShader::SpriteShaderParameters{
        XMMatrixTranspose(worldMatrix * viewProj),
        1.f
    };

    shader->ApplyMaterialParameters(
        &shaderParameters,
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
        0.0f,
        pTexture,
        nullptr
    );

    shader->ApplyShaderParams();
    pImmediateContext->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, &shaderParameters, 0, 0);

    shader->GetPass()->Apply(0, pImmediateContext);

    pImmediateContext->Draw(6, 0);

    //shader->DeleteParameters(shaderParameters);

    LogEndDrawSelf();
}
