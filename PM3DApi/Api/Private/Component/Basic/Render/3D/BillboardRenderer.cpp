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
    const auto rotation = parentObject->GetWorldRotationQuaternion();
    XMFLOAT3 scale = parentObject->GetWorldScale();
    scale.x *= textureSizeX * selfScale.x;
    scale.y *= textureSizeY * selfScale.y;
    scale.z = 1.f;

    const auto cameraPosVec = XMVectorSet(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);
    const auto cameraDirVec = XMVectorSet(cameraDirection.x, cameraDirection.y, cameraDirection.z, 0.0f);
    const auto cameraUpVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    const auto posVec = XMVectorSet(position.x, position.y, position.z, 1.0f);

    const XMMATRIX viewProj = camera->GetMatViewProj();

    XMMATRIX worldMatrix;

    switch (alignment)
    {
    case BillboardAlignment::NONE:
        //worldMatrix = XMMatrixScaling(scale.x, scale.y, 1.0f) * XMMatrixTranslation(position.x, position.y, position.z);
        worldMatrix = XMMatrixLookAtRH(posVec, cameraPosVec, cameraUpVec);
        break;
    case BillboardAlignment::Z:
        // Calcul de la matrice de rotation pour l'alignement Z
        worldMatrix = XMMatrixLookAtRH(posVec, cameraPosVec, XMVectorSet(0, 0, 1, 0));
        break;
    case BillboardAlignment::XZ:
        // Calcul de la matrice de rotation pour l'alignement XZ
        worldMatrix = XMMatrixLookAtRH(posVec, cameraPosVec, XMVectorSet(0, 1, 0, 0));
        break;
    case BillboardAlignment::YZ:
        // Calcul de la matrice de rotation pour l'alignement YZ
        worldMatrix = XMMatrixLookAtRH(posVec, cameraPosVec, XMVectorSet(1, 0, 0, 0));
        break;
    case BillboardAlignment::XYZ:
        worldMatrix = XMMatrixScaling(scale.x, scale.y, 1.0f)
            * XMMatrixRotationQuaternion(rotation.ToXMVector())
            * XMMatrixTranslation(position.x, position.y, position.z);
        break;
    }

    //XMMATRIX worldMatrix = XMMatrixScaling(scale.x, scale.y, 1.0f) * XMMatrixTranslation(position.x, position.y, position.z);
    //XMMATRIX WVP = XMMatrixTranspose(worldMatrix * viewProj);

    const auto shaderParameters = SpriteShader::SpriteShaderParameters{
        XMMatrixTranspose(worldMatrix * viewProj),
        1.f
    };

    const auto variableTexture = shader->GetEffect()->GetVariableByName("textureEntree")->AsShaderResource();
    variableTexture->SetResource(pTexture);

    pImmediateContext->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, &shaderParameters, 0, 0);

    ID3DX11EffectConstantBuffer* pCB = shader->GetEffect()->GetConstantBufferByName("param");
    pCB->SetConstantBuffer(shader->GetShaderParametersBuffer());

    shader->GetPass()->Apply(0, pImmediateContext);

    pImmediateContext->Draw(6, 0);

    LogEndDrawSelf();
}
