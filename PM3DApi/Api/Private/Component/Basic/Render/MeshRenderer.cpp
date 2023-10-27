#include "../../../../Public/Component/Basic/Render/MeshRenderer.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdexcept>

#include "../../../../../../PetitMoteur3D/Core/Public/Core/moteurWindows.h"
#include "../../../../../../PetitMoteur3D/Core/Public/Mesh/CSommetMesh.h"
#include "../../../../../../PetitMoteur3D/Core/Public/Mesh/FastobjChargeur.h"
#include "../../../../../../PetitMoteur3D/Core/Public/Texture/GestionnaireDeTextures.h"
#include "../../../../../../PetitMoteur3D/Core/Public/Util/resource.h"
#include "../../../../../../PetitMoteur3D/Core/Public/Util/Time.h"
#include "../../../../../../PetitMoteur3D/Core/Public/Util/util.h"
#include "../../../../Public/GameHost.h"
#include "../../../../Public/GameObject/GameObject.h"
#include "../../../../Public/Scene/Scene.h"
#include "../../../../Public/Shader/Shader.h"
#include "Api/Public/Util/Util.h"

PM3D_API::MeshRenderer::MeshRenderer(std::unique_ptr<Shader>&& shader, std::string meshName) : Renderer(std::move(shader))
{
	std::cout << "MeshRenderer::MeshRenderer(" << meshName << ")" << std::endl;
	
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

PM3D_API::MeshRenderer::MeshRenderer(std::unique_ptr<Shader>&& shader, PM3D::IChargeur* chargeur) : Renderer(std::move(shader)), chargeur(chargeur)
{
	std::cout << "MeshRenderer::MeshRenderer(chargeur)" << std::endl;

	// Load mesh
	LoadMesh();
}

PM3D_API::MeshRenderer::~MeshRenderer()
{
	std::cout << "MeshRenderer::~MeshRenderer()" << std::endl;
	
	delete chargeur; // Will also delete mesh using fastObjDestroy
}

void PM3D_API::MeshRenderer::Initialize()
{
	std::cout << "MeshRenderer::Initialize()" << std::endl;
}

void PM3D_API::MeshRenderer::DrawSelf() const
{
	LogBeginDrawSelf();
	
	if (!mesh)
		throw std::runtime_error("MeshRenderer::DrawSelf: mesh is null");

	const auto scene = parentObject->GetScene();
	const auto camera = scene->GetMainCamera();

	if (!camera)
	{
		throw std::runtime_error("MeshRenderer::DrawSelf: camera is null");
	}

	// Frustrum culling
	if(!IsVisible())
		return;

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
	constexpr UINT stride = sizeof(CSommetMesh);
	constexpr UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, shader->GetVertexBufferPtr(), &stride, &offset);

	{ // SHADOWS
		pImmediateContext->OMSetRenderTargets(0, nullptr, shader->GetDepthStencilView());
		pImmediateContext->ClearDepthStencilView(shader->GetDepthStencilView(), D3D11_CLEAR_DEPTH,1.0f,0 );
		pDispositif->SetViewPortDimension(512,512);
		const auto pTechnique = shader->GetEffect()->GetTechniqueByName("ShadowMap");
		const auto pPasse = pTechnique->GetPassByIndex(0);
		pImmediateContext->IASetInputLayout(shader->GetShadowVertexLayout());

		shader->LoadLights(pImmediateContext, parentObject);

		const XMMATRIX viewProj = camera->GetMatViewProj();
	
		const auto shaderParameters = shader->PrepareParameters(
			XMMatrixTranspose(parentObject->GetMatWorld() * viewProj),
			XMMatrixTranspose(parentObject->GetMatWorld())
		);

		for (int i = 0; i < mesh->object_count; ++i)
		{
			const auto objGroup = mesh->objects[i];
			const unsigned indexStart = objGroup.index_offset;
		
			unsigned int indexDrawAmount;
			if (mesh->object_count > 1)
			{
				indexDrawAmount = mesh->objects[i + 1].index_offset - indexStart;
			} else
			{
				indexDrawAmount = mesh->index_count;
			}

			if (!indexDrawAmount)
			{
				continue;
			}

			// IMPORTANT pour ajuster les param.
			shader->GetPass()->Apply(0, pImmediateContext);

			shader->ApplyShaderParams();
			pImmediateContext->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, shaderParameters, 0, 0);
		
			pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
		}

		shader->DeleteParameters(shaderParameters);
	}

	//shader->LoadLights(pImmediateContext, parentObject);

	const XMMATRIX viewProj = camera->GetMatViewProj();
	
	const auto shaderParameters = shader->PrepareParameters(
		XMMatrixTranspose(parentObject->GetMatWorld() * viewProj),
		XMMatrixTranspose(parentObject->GetMatWorld())
	);

	// Dessiner les sous-objets non-transparents
	for (int i = 0; i < mesh->object_count; ++i)
	{
		const auto objGroup = mesh->objects[i];
		const unsigned indexStart = objGroup.index_offset;
		
		unsigned int indexDrawAmount;
		if (mesh->object_count > 1)
		{
			indexDrawAmount = mesh->objects[i + 1].index_offset - indexStart;
		} else
		{
			indexDrawAmount = mesh->index_count;
		}

		if (!indexDrawAmount)
		{
			continue;
		}
		
		shader->ApplyMaterialParameters(
			shaderParameters,
			XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Ambient),
			XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Diffuse),
			XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Specular),
			Material[SubmeshMaterialIndex[i]].Puissance,
			Material[SubmeshMaterialIndex[i]].pAlbedoTexture,
			Material[SubmeshMaterialIndex[i]].pNormalmapTexture
		);

		// IMPORTANT pour ajuster les param.
		shader->GetPass()->Apply(0, pImmediateContext);

		shader->ApplyShaderParams();

		pImmediateContext->UpdateSubresource(shader->GetShaderParametersBuffer(), 0, nullptr, shaderParameters, 0, 0);
		
		pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
	}

	shader->DeleteParameters(shaderParameters);

	LogEndDrawSelf();
}

bool PM3D_API::MeshRenderer::IsVisible() const
{
	const Camera* camera = parentObject->GetScene()->GetMainCamera();
	return camera->getFrustrum().ContainsSphere(DirectX::XMLoadFloat3(&parentObject->GetWorldPosition()), boundingRadius);
}

void PM3D_API::MeshRenderer::LoadMesh()
{
	ID3D11Device* pD3DDevice = GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

	// 1. SOMMETS a) Créations des sommets dans un tableau temporaire
	{
		const size_t nombreSommets = chargeur->GetNombreSommets();

		std::cout << "MeshRenderer::LoadMesh: nombreSommets = " << nombreSommets << std::endl;
		
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
	Material.reserve(chargeur->GetNombreMaterial() + 1);
	Material.emplace_back(CMaterial());

	// 4b) Copie des matériaux dans la version locale
	for (int32_t i = 0; i < chargeur->GetNombreMaterial(); ++i)
	{
		CMaterial mat =	chargeur->GetMaterial(i);
		Material.push_back(mat);
	}

	// 4c) Trouver l’index du materiau pour chaque sous-ensemble
	SubmeshMaterialIndex.reserve(chargeur->GetNombreSubset());
	for (int32_t i = 0; i < chargeur->GetNombreSubset(); ++i)
	{
		int32_t index;
		for (index = 0; index < Material.size(); ++index)
		{
			const auto materialName = Material[index].NomMateriau;
			if (materialName == "" || materialName == chargeur->GetMaterialName(i))
				break;
		}
		if (index >= Material.size()) index = 0; // valeur de défaut
		SubmeshMaterialIndex.push_back(index);
	}

	// 4d) Chargement des textures
	PM3D::CGestionnaireDeTextures& TexturesManager = PM3D::CMoteurWindows::GetInstance().GetTextureManager();
	
	for (uint32_t i = 0; i < Material.size(); ++i)
	{
		if (Material[i].albedoTextureFileName.length() > 0)
		{
			const std::wstring ws(Material[i].albedoTextureFileName.begin(), Material[i].albedoTextureFileName.end());
			Material[i].pAlbedoTexture = TexturesManager.GetNewTexture(ws.c_str(), GameHost::GetInstance()->GetDispositif())->GetD3DTexture();
		} else
		{
			std::cout << "MeshRenderer::LoadMesh: Material[" << i << "].albedoTextureFileName is empty" << std::endl;
		}

		if (Material[i].normalmapTextureFileName.length() > 0)
		{
			const std::wstring ws(Material[i].normalmapTextureFileName.begin(), Material[i].normalmapTextureFileName.end());
			Material[i].pNormalmapTexture = TexturesManager.GetNewTexture(ws.c_str(), GameHost::GetInstance()->GetDispositif())->GetD3DTexture();
		} else
		{
			std::cout << "MeshRenderer::LoadMesh: Material[" << i << "].normalmapTextureFileName is empty" << std::endl;
		}
	}

	mesh = static_cast<fastObjMesh*>(chargeur->GetMesh());
	meshLoaded = true;
}
