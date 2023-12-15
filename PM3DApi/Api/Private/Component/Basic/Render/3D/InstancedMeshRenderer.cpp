#include "Api/Public/Component/Basic/Render/3D/InstancedMeshRenderer.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdexcept>

#include "Core/Public/Core/moteurWindows.h"
#include "Core/Public/Mesh/CSommetMesh.h"
#include "Core/Public/Mesh/FastobjChargeur.h"
#include "Core/Public/Texture/GestionnaireDeTextures.h"
#include "Core/Public/Util/resource.h"
#include "Core/Public/Util/util.h"
#include "Api/Public/GameHost.h"
#include "Api/Public/GameObject/GameObject.h"
#include "Api/Public/Scene/Scene.h"
#include "Api/Public/Shader/Shader.h"
#include "Api/Public/Util/Util.h"

PM3D_API::InstancedMeshRenderer::InstancedMeshRenderer(
    std::unique_ptr<Shader>&& shader,
    std::string meshName,
    const std::vector<MapImporter::InstanceObject>&& instances
) : Renderer(std::move(shader)), instances(instances)
{
    std::cout << "InstancedMeshRenderer::InstancedMeshRenderer(" << meshName << ")" << std::endl;

    if (meshName.find_last_of(".obj") == std::string::npos)
        meshName += ".obj";

    chargeur = new PM3D::FastobjChargeur();

    PM3D::CParametresChargement params;
    params.NomChemin = "";
    params.NomFichier = meshName;

    chargeur->Chargement(params);

    // Load mesh
    LoadMesh();
}

PM3D_API::InstancedMeshRenderer::InstancedMeshRenderer(
    std::unique_ptr<Shader>&& shader,
    PM3D::IChargeur* chargeur,
    const std::vector<PM3D_API::MapImporter::InstanceObject>&& instances
) : Renderer(std::move(shader)), chargeur(chargeur), instances(instances)
{
    std::cout << "InstancedMeshRenderer::InstancedMeshRenderer(chargeur)" << std::endl;

    // Load mesh
    LoadMesh();
}

PM3D_API::InstancedMeshRenderer::~InstancedMeshRenderer()
{
    std::cout << "InstancedMeshRenderer::~InstancedMeshRenderer()" << std::endl;

    delete chargeur; // Will also delete mesh using fastObjDestroy
}

void PM3D_API::InstancedMeshRenderer::Initialize()
{
    std::cout << "InstancedMeshRenderer::Initialize()" << std::endl;
}

void PM3D_API::InstancedMeshRenderer::DrawSelf() const
{
    LogBeginDrawSelf();

    if (!mesh)
        throw std::runtime_error("InstancedMeshRenderer::DrawSelf: mesh is null");

    const auto scene = parentObject->GetScene();
    const auto camera = scene->GetMainCamera();

    if (!camera)
    {
        throw std::runtime_error("InstancedMeshRenderer::DrawSelf: camera is null");
    }

    // Frustrum culling
    // TODO: compute bounding box for each object
    /*
    if (!IsVisible())
    {
        LogEndDrawSelf();
        return;
    }
    */

    // Obtenir le contexte
    const auto pDispositif = GameHost::GetInstance()->GetDispositif();
    const auto pD3DDevice = pDispositif->GetD3DDevice();
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // input layout des sommets
    pImmediateContext->IASetInputLayout(shader->GetVertexLayout());

    // Index buffer
    pImmediateContext->IASetIndexBuffer(shader->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

    // Vertex buffer
    UINT strides[2];
    strides[0] = sizeof(CSommetMesh);
    strides[1] = sizeof(UINT);

    UINT offsets[2];
    offsets[0] = 0;
    offsets[1] = 0;

    ID3D11Buffer* buffers[2];
    buffers[0] = shader->GetVertexBuffer();
    buffers[1] = shader->GetInstanceBuffer();

    pImmediateContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);

    pDispositif->ResetViewportDimension();

    shader->LoadLights(pImmediateContext, parentObject);

    const XMMATRIX viewProj = camera->GetMatViewProj();

    const auto shaderParameters = shader->PrepareParameters(XMMATRIX(), XMMATRIX());

    auto drawInstances = std::vector<DrawInstance>();
    auto instancesIndexes = std::vector<UINT>();

    for (size_t i = 0; i < instances.size(); ++i)
    {
        const auto& instance = instances[i];

        const auto position = instance.position;
        const auto rotation = instance.rotation;
        const auto scale = instance.scale;

        if (!IsVisible(position, scale))
        {
            continue;
        }

        const auto matWorld = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
            DirectX::XMMatrixRotationQuaternion(Quaternion(rotation).ToXMVector()) *
            DirectX::XMMatrixTranslation(position.x, position.y, position.z);

        const auto matWorldViewProj = matWorld * viewProj;

        drawInstances.emplace_back(DrawInstance{
            DirectX::XMMatrixTranspose(matWorldViewProj),
            DirectX::XMMatrixTranspose(matWorld)
        });
        instancesIndexes.emplace_back(static_cast<UINT>(i));
    }

    const auto instancesCount = drawInstances.size();

    if (instancesCount == 0)
    {
        LogEndDrawSelf();
        return;
    }

    {
        if (shader->GetInstanceBuffer())
        {
            shader->GetInstanceBuffer()->Release();
        }

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(UINT) * static_cast<UINT>(instancesCount);

        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = instancesIndexes.data();
        PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, shader->GetInstanceBufferPtr()),
                        DXE_CREATIONINDEXBUFFER);
    }

    D3D11_BUFFER_DESC instancesBufferDesc;
    ZeroMemory(&instancesBufferDesc, sizeof(instancesBufferDesc));
    instancesBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    instancesBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    instancesBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    instancesBufferDesc.StructureByteStride = sizeof(DrawInstance);
    instancesBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    instancesBufferDesc.ByteWidth = sizeof(DrawInstance) * static_cast<UINT>(instancesCount);

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = drawInstances.data();

    ID3D11Buffer* instancesBuffer = nullptr;
    PM3D::DXEssayer(pD3DDevice->CreateBuffer(&instancesBufferDesc, &initData, &instancesBuffer));

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.ElementWidth = static_cast<UINT>(instancesCount);

    ID3D11ShaderResourceView* resource_view = nullptr;
    PM3D::DXEssayer(pD3DDevice->CreateShaderResourceView(instancesBuffer, &srvDesc, &resource_view));

    ID3DX11EffectShaderResourceVariable* resource_variable;
    resource_variable = shader->GetEffect()->GetVariableByName("instances")->AsShaderResource();
    resource_variable->SetResource(resource_view);

    // Dessiner les sous-objets non-transparents
    for (unsigned int i = 0; i < mesh->group_count; ++i)
    {
        const auto [name, face_count, face_offset, index_offset] = mesh->groups[i];
        const unsigned indexStart = index_offset;

        unsigned int indexDrawAmount;
        if (mesh->group_count > 1 && i + 1 < mesh->group_count)
        {
            indexDrawAmount = mesh->groups[i + 1].index_offset - indexStart;
        }
        else
        {
            indexDrawAmount = mesh->index_count;
        }

        if (!indexDrawAmount)
        {
            continue;
        }

        const auto material = Material[SubmeshMaterialIndex[i]];

        shader->ApplyMaterialParameters(
            shaderParameters,
            XMLoadFloat4(&material.Ambient),
            XMLoadFloat4(&material.Diffuse),
            XMLoadFloat4(&material.Specular),
            material.Puissance,
            material.pAlbedoTexture,
            material.pNormalmapTexture
        );

        // IMPORTANT pour ajuster les param.
        shader->GetPass()->Apply(0, pImmediateContext);

        shader->ApplyShaderParams();

        pImmediateContext->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, shaderParameters, 0, 0);

        pImmediateContext->DrawInstanced(indexDrawAmount, static_cast<UINT>(instancesCount), indexStart, 0);
    }

    shader->DeleteParameters(shaderParameters);
    resource_variable->Release();
    instancesBuffer->Release();
    resource_view->Release();

    LogEndDrawSelf();
}

void PM3D_API::InstancedMeshRenderer::DrawShadowSelf(const PM3D_API::Camera& camera) const
{
    if (!mesh)
        throw std::runtime_error("MeshRenderer::DrawSelf: mesh is null");

    // Frustrum culling
    /*if (!IsVisible(camera))
    {
        LogEndDrawSelf();
        return;
    }*/

    //Obtenir le contexte
    const auto pDispositif = GameHost::GetInstance()->GetDispositif();
    ID3D11DeviceContext* pImmediateContext = pDispositif->GetImmediateContext();

    // Choisir la topologie des primitives
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // input layout des sommets
    //pImmediateContext->IASetInputLayout(shader->GetShadowVertexLayout());
    pImmediateContext->IASetInputLayout(shader->GetVertexLayout());

    // Index buffer
    pImmediateContext->IASetIndexBuffer(shader->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

    // Vertex buffer
    constexpr UINT stride = sizeof(CSommetMesh);
    constexpr UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, shader->GetVertexBufferPtr(), &stride, &offset);

    const auto pTechnique = shader->GetEffect()->GetTechniqueByName("ShadowMap");
    const auto pPasse = pTechnique->GetPassByIndex(0);

    const XMMATRIX viewProj = camera.GetMatViewProj();

    const auto shaderParameters = shader->PrepareParameters(
        XMMatrixTranspose(parentObject->GetMatWorld() * viewProj),
        XMMatrixTranspose(parentObject->GetMatWorld())
    );

    pPasse->Apply(0, pImmediateContext);
    pImmediateContext->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, shaderParameters, 0, 0);
    shader->ApplyShaderParams();


    for (int i = 0; i < mesh->group_count; ++i)
    {
        const auto objGroup = mesh->groups[i];
        const unsigned indexStart = objGroup.index_offset;

        unsigned int indexDrawAmount;
        if (mesh->group_count > 1)
        {
            if (i + 1 < mesh->group_count)
            {
                indexDrawAmount = mesh->groups[i + 1].index_offset - indexStart;
            }
            else
            {
                indexDrawAmount = mesh->index_count - indexStart;
            }
        }
        else
        {
            indexDrawAmount = mesh->index_count;
        }

        if (!indexDrawAmount)
        {
            continue;
        }

        // IMPORTANT pour ajuster les param.pPasse->Apply(0, pImmediateContext);
        shader->GetPass()->Apply(0, pImmediateContext);

        shader->ApplyShaderParams();

        pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
    }

    shader->DeleteParameters(shaderParameters);
}

bool PM3D_API::InstancedMeshRenderer::IsVisible(const XMFLOAT3 position, const XMFLOAT3 scale) const
{
    const Camera* camera = parentObject->GetScene()->GetMainCamera();
    const float maxScale = max(max(scale.x, scale.y), scale.z);
    const DirectX::XMVECTOR worldPos = DirectX::XMLoadFloat3(&position);
    const DirectX::XMVECTOR viewPos = -DirectX::XMVector3Transform(worldPos, camera->GetMatView());
    return camera->getFrustrum().ContainsSphere(viewPos, boundingRadius * maxScale);
}

//bool PM3D_API::InstancedMeshRenderer::IsVisible(const Camera& camera) const
//{
//    return true;
//}

bool PM3D_API::InstancedMeshRenderer::IsVisible() const
{
    const Camera* camera = parentObject->GetScene()->GetMainCamera();
    const float maxScale =
        max(max(parentObject->GetWorldScale().x, parentObject->GetWorldScale().y), parentObject->GetWorldScale().z);
    const DirectX::XMVECTOR worldPos = DirectX::XMLoadFloat3(&parentObject->GetWorldPosition());
    const DirectX::XMVECTOR viewPos = -DirectX::XMVector3Transform(worldPos, camera->GetMatView());
    return camera->getFrustrum().ContainsSphere(viewPos, boundingRadius * maxScale);
}

void PM3D_API::InstancedMeshRenderer::LoadMesh()
{
    mesh = static_cast<fastObjMesh*>(chargeur->GetMesh());

    ID3D11Device* pD3DDevice = GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

    // 1. SOMMETS a) Créations des sommets dans un tableau temporaire
    {
        const size_t nombreSommets = chargeur->GetNombreSommets();

        std::cout << "InstancedMeshRenderer::LoadMesh: nombreSommets = " << nombreSommets << std::endl;

        std::unique_ptr<CSommetMesh[]> ts(new CSommetMesh[nombreSommets]);
        boundingRadius = 0.f;
        for (uint32_t i = 0; i < nombreSommets; ++i)
        {
            ts[i].position = chargeur->GetPosition(i);
            boundingRadius = max(Util::magnitude(ts[i].position), boundingRadius);
            ts[i].normal = chargeur->GetNormale(i);
            ts[i].coordTex = chargeur->GetCoordTex(i);
            ts[i].binormal = chargeur->GetBiNormale(i);
            ts[i].tangent = chargeur->GetTangent(i);
        }

        // 1. SOMMETS b) Création du vertex buffer et copie des sommets
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = static_cast<uint32_t>(sizeof(CSommetMesh) *
            nombreSommets);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = ts.get();
        PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, shader->GetVertexBufferPtr()),
                        DXE_CREATIONVERTEXBUFFER);
    }


    // 2. INDEX - Création de l’index buffer et copie des indices
    // Les indices étant habituellement des entiers, j’ai
    // pris directement ceux du chargeur, mais attention au
    // format si vous avez autre chose que DXGI_FORMAT_R32_UINT
    {
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = static_cast<uint32_t>(sizeof(uint32_t) *
            chargeur->GetNombreIndex());

        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = chargeur->GetIndexData();
        PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, shader->GetIndexBufferPtr()),
                        DXE_CREATIONINDEXBUFFER);
    }

    // 3. Les sous-objets
    NombreSubmesh = chargeur->GetNombreSubset();

    // Début de chaque sous-objet et un pour la fin
    SubmeshIndex.reserve(NombreSubmesh);
    chargeur->CopieSubsetIndex(SubmeshIndex);

    // 4. MATERIAUX

    // 4a) Créer un matériau de défaut en index 0
    // Vous pourriez changer les valeurs, j’ai conservé
    // celles du constructeur
    //Material.reserve(chargeur->GetNombreMaterial());
    //Material.emplace_back(CMaterial());

    // 4b) Copie des matériaux dans la version locale
    for (int32_t i = 0; i < chargeur->GetNombreMaterial(); ++i)
    {
        CMaterial mat = chargeur->GetMaterial(i);
        Material.push_back(mat);
    }

    if (Material.empty())
    {
        Material.emplace_back(CMaterial());
    }

    // 4c) Trouver l’index du materiau pour chaque sous-ensemble
    SubmeshMaterialIndex.reserve(mesh->group_count);
    for (unsigned i = 0; i < mesh->group_count; ++i)
    {
        const auto group = mesh->groups[i];
        SubmeshMaterialIndex.push_back(mesh->face_materials[group.face_offset]);

        /*
        int32_t index;
        for (index = 0; index < Material.size(); ++index)
        {
            const auto materialName = Material[index].NomMateriau;
            if (materialName == chargeur->GetMaterialName(i))
                break;
        }
        if (index >= Material.size()) index = 0; // valeur de défaut
        SubmeshMaterialIndex.push_back(index);
        */
    }

    //std::reverse(SubmeshMaterialIndex.begin(), SubmeshMaterialIndex.end());

    // 4d) Chargement des textures
    PM3D::CGestionnaireDeTextures& TexturesManager = PM3D::CMoteurWindows::GetInstance().GetTextureManager();

    for (uint32_t i = 0; i < Material.size(); ++i)
    {
        if (Material[i].albedoTextureFileName.length() > 0)
        {
            const std::wstring ws(Material[i].albedoTextureFileName.begin(), Material[i].albedoTextureFileName.end());
            Material[i].pAlbedoTexture = TexturesManager.GetNewTexture(
                ws.c_str(), GameHost::GetInstance()->GetDispositif())->GetD3DTexture();
        }
        else
        {
            std::cout << "InstancedMeshRenderer::LoadMesh: Material[" << i << "].albedoTextureFileName is empty" <<
                std::endl;
        }

        if (Material[i].normalmapTextureFileName.length() > 0)
        {
            const std::wstring ws(Material[i].normalmapTextureFileName.begin(),
                                  Material[i].normalmapTextureFileName.end());
            Material[i].pNormalmapTexture = TexturesManager.GetNewTexture(
                ws.c_str(), GameHost::GetInstance()->GetDispositif())->GetD3DTexture();
        }
        else
        {
            std::cout << "InstancedMeshRenderer::LoadMesh: Material[" << i << "].normalmapTextureFileName is empty" <<
                std::endl;
        }
    }

    meshLoaded = true;
}
