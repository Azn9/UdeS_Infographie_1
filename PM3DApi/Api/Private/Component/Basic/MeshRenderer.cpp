#include "../../../Public/Component/Basic/MeshRenderer.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdexcept>

#include "../../../../../PetitMoteur3D/Core/Public/Core/moteurWindows.h"
#include "../../../../../PetitMoteur3D/Core/Public/Mesh/CSommetMesh.h"
#include "../../../../../PetitMoteur3D/Core/Public/Mesh/FastobjChargeur.h"
#include "../../../../../PetitMoteur3D/Core/Public/Texture/GestionnaireDeTextures.h"
#include "../../../../../PetitMoteur3D/Core/Public/Util/resource.h"
#include "../../../../../PetitMoteur3D/Core/Public/Util/util.h"
#include "../../../Public/GameHost.h"
#include "../../../Public/GameObject/GameObject.h"
#include "../../../Public/Scene/Scene.h"

PM3D_API::MeshRenderer::MeshRenderer(std::string meshName)
{
	std::cout << "MeshRenderer::MeshRenderer(" << meshName << ")" << std::endl;
	
	if (meshName.find_last_of(".obj") == std::string::npos)
		meshName += ".obj";

	chargeur = new PM3D::FastobjChargeur();

	PM3D::CParametresChargement params;
	params.NomChemin = "";
	params.NomFichier = meshName;
	
	chargeur->Chargement(params);

	// Init effect
	InitEffect();
	
	// Load mesh
	LoadMesh();
}

PM3D_API::MeshRenderer::MeshRenderer(PM3D::IChargeur* chargeur) : chargeur(chargeur)
{
	std::cout << "MeshRenderer::MeshRenderer(chargeur)" << std::endl;

	// Init effect
	InitEffect();
	
	// Load mesh
	LoadMesh();
}

PM3D_API::MeshRenderer::~MeshRenderer()
{
	delete chargeur;
	
	if (mesh != nullptr)
		fast_obj_destroy(mesh);

	PM3D::DXRelacher(pConstantBuffer);
	PM3D::DXRelacher(pSampleState);
	PM3D::DXRelacher(pEffet);
	PM3D::DXRelacher(pVertexLayout);
	PM3D::DXRelacher(pIndexBuffer);
	PM3D::DXRelacher(pVertexBuffer);
}

void PM3D_API::MeshRenderer::Initialize()
{
	std::cout << "MeshRenderer::Initialize()" << std::endl;
}

struct ShadersParams
{
	XMMATRIX matWorldViewProj; // la matrice totale
	XMMATRIX matWorld; // matrice de transformation dans le monde
	XMVECTOR vLumiere; // la position de la source d’éclairage (Point)
	XMVECTOR vCamera; // la position de la caméra
	XMVECTOR vAEcl; // la valeur ambiante de l’éclairage
	XMVECTOR vAMat; // la valeur ambiante du matériau
	XMVECTOR vDEcl; // la valeur diffuse de l’éclairage
	XMVECTOR vDMat; // la valeur diffuse du matériau
	XMVECTOR vSEcl; // la valeur spéculaire de l’éclairage
	XMVECTOR vSMat; // la valeur spéculaire du matériau
	float puissance; // la puissance de spécularité
	int bTex; // Texture ou materiau
	XMFLOAT2 remplissage;
};

void PM3D_API::MeshRenderer::DrawSelf() const
{
	std::cout << "MeshRenderer::DrawSelf()" << std::endl;
	
	if (!mesh)
		throw std::runtime_error("MeshRenderer::DrawSelf: mesh is null");

	const auto scene = parentObject->GetScene();
	const auto camera = scene->GetMainCamera();
	const auto directionnalLight = scene->GetDirectionalLight();

	if (!camera || !directionnalLight)
		throw std::runtime_error("MeshRenderer::DrawSelf: camera or directionnalLight is null");

	// Obtenir le contexte
	ID3D11DeviceContext* pImmediateContext = GameHost::GetInstance()->GetDispositif()->GetImmediateContext();

	// Choisir la topologie des primitives
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// input layout des sommets
	pImmediateContext->IASetInputLayout(pVertexLayout);

	// Index buffer
	pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Vertex buffer
	UINT stride = sizeof(CSommetMesh);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Initialiser et sélectionner les « constantes » de l’effet
	ShadersParams sp;
	XMMATRIX viewProj = camera->GetMatViewProj();
	sp.matWorldViewProj = XMMatrixTranspose(parentObject->GetMatWorld() * viewProj);
	sp.matWorld = XMMatrixTranspose(parentObject->GetMatWorld());

	if (directionnalLight)
	{
		auto rotation = directionnalLight->GetLocalRotationEuler();
		sp.vLumiere = XMVectorSet(rotation.x, rotation.y, rotation.z, 1.0f);
	}
	else
	{
		sp.vLumiere = XMVectorSet(0.0f, -1.0f, 0.0f, 1.0f);
	}

	auto cameraPosition = camera->GetWorldPosition();
	sp.vCamera = XMVectorSet(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f);
	
	sp.vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
	sp.vDEcl = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	sp.vSEcl = XMVectorSet(0.6f, 0.6f, 0.6f, 1.0f);

	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, pSampleState);

	// Dessiner les sous-objets non-transparents
	for (int i = 0; i < mesh->object_count; ++i)
	{
		auto objGroup = mesh->objects[i];
		unsigned indexStart = objGroup.index_offset;
		int indexDrawAmount = mesh->objects[i + 1].index_offset - indexStart;

		if (!indexDrawAmount)
		{
			continue;
		}

		sp.vAMat = DirectX::XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Ambient);
		sp.vDMat = DirectX::XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Diffuse);
		sp.vSMat = DirectX::XMLoadFloat4(&Material[SubmeshMaterialIndex[i]].Specular);
		sp.puissance = Material[SubmeshMaterialIndex[i]].Puissance;

		// Activation de la texture ou non
		if (Material[SubmeshMaterialIndex[i]].pTextureD3D != nullptr)
		{
			ID3DX11EffectShaderResourceVariable* variableTexture;
			variableTexture =
			pEffet->GetVariableByName("textureEntree")->AsShaderResource();
			variableTexture->SetResource(Material[SubmeshMaterialIndex[i]].pTextureD3D);
			sp.bTex = 1;
		}
		else
		{
			sp.bTex = 0;
		}

		// IMPORTANT pour ajuster les param.
		pPasse->Apply(0, pImmediateContext);

		// Nous n’avons qu’un seul CBuffer
		ID3DX11EffectConstantBuffer* pCB = pEffet->GetConstantBufferByName("param");
		pCB->SetConstantBuffer(pConstantBuffer);

		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &sp, 0, 0);
		pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
	}
}

void PM3D_API::MeshRenderer::LoadMesh()
{
	ID3D11Device* pD3DDevice = GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

	// 1. SOMMETS a) Créations des sommets dans un tableau temporaire
	{
		const size_t nombreSommets = chargeur->GetNombreSommets();
		
		std::unique_ptr<CSommetMesh[]> ts(new CSommetMesh[nombreSommets]);
		for (uint32_t i = 0; i < nombreSommets; ++i)
		{
			ts[i].position = chargeur->GetPosition(i);
			ts[i].normal = chargeur->GetNormale(i);
			ts[i].coordTex = chargeur->GetCoordTex(i);
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
		pVertexBuffer = nullptr;
		PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer),
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
		pIndexBuffer = nullptr;
		PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer),
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
		CMaterial mat;
		chargeur->GetMaterial(i, mat.NomFichierTexture,
			mat.NomMateriau,
			mat.Ambient,
			mat.Diffuse,
			mat.Specular,
			mat.Puissance);
		Material.push_back(mat);
	}

	// 4c) Trouver l’index du materiau pour chaque sous-ensemble
	SubmeshMaterialIndex.reserve(chargeur->GetNombreSubset());
	for (int32_t i = 0; i < chargeur->GetNombreSubset(); ++i)
	{
		int32_t index;
		for (index = 0; index < Material.size(); ++index)
		{
			if (Material[index].NomMateriau == chargeur->GetMaterialName(i))
				break;
		}
		if (index >= Material.size()) index = 0; // valeur de défaut
		SubmeshMaterialIndex.push_back(index);
	}

	// 4d) Chargement des textures
	PM3D::CGestionnaireDeTextures& TexturesManager = PM3D::CMoteurWindows::GetInstance().GetTextureManager();
	
	for (uint32_t i = 0; i < Material.size(); ++i)
	{
		if (Material[i].NomFichierTexture.length() > 0)
		{
			const std::wstring ws(Material[i].NomFichierTexture.begin(),
				Material[i].NomFichierTexture.end());
			Material[i].pTextureD3D = TexturesManager.GetNewTexture(ws.c_str(),
				GameHost::GetInstance()->GetDispositif())->GetD3DTexture();
		}
	}

	mesh = static_cast<fastObjMesh*>(chargeur->GetMesh());
	meshLoaded = true;
}

void PM3D_API::MeshRenderer::InitEffect()
{
	// Compilation et chargement du vertex shader
	ID3D11Device* pD3DDevice = GameHost::GetInstance()->GetDispositif()->GetD3DDevice();

	// Création d’un tampon pour les constantes du VS
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ShadersParams);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	PM3D::DXEssayer(pD3DDevice->CreateBuffer(&bd, nullptr, &pConstantBuffer));

	// Pour l’effet
	ID3DBlob* pFXBlob = nullptr;

	PM3D::DXEssayer(D3DCompileFromFile(L"MiniPhong.fx", 0, 0, 0,
			"fx_5_0", 0, 0, &pFXBlob, 0),
		DXE_ERREURCREATION_FX);

	D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);

	pFXBlob->Release();

	pTechnique = pEffet->GetTechniqueByIndex(0);
	pPasse = pTechnique->GetPassByIndex(0);

	// Créer l’organisation des sommets pour le VS de notre effet
	D3DX11_PASS_SHADER_DESC effectVSDesc;
	pPasse->GetVertexShaderDesc(&effectVSDesc);

	D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
	effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

	const void* vsCodePtr = effectVSDesc2.pBytecode;
	const uint32_t vsCodeLen = effectVSDesc2.BytecodeLength;
	pVertexLayout = nullptr;

	CSommetMesh::numElements = ARRAYSIZE(CSommetMesh::layout);

	PM3D::DXEssayer(pD3DDevice->CreateInputLayout(CSommetMesh::layout,
			CSommetMesh::numElements,
			vsCodePtr,
			vsCodeLen,
			&pVertexLayout),
		DXE_CREATIONLAYOUT);

	// Initialisation des paramètres de sampling de la texture
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Création de l’état de sampling
	pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);
} // InitEffet
