#pragma once
#include <chrono>
#include <locale>

#include "Core/Public/Util/Singleton.h"
#include "dispositif.h"

#include <vector>

#include "Core/Public/Object/Objet3D.h"
#include "Core/Public/Texture/GestionnaireDeTextures.h"
#include "Core/Public/Mesh/ObjectMesh.h"
#include "Core/Public/Mesh/chargeur.h"
#include "Core/Public/Mesh/FastobjChargeur.h"
#include "Core/Public/Sprite/AfficheurSprite.h"
#include "Core/Public/Sprite/SpriteTemp.h"
#include "Core/Public/Util/ChargeurOBJ.h"

namespace PM3D
{

const int IMAGESPARSECONDE = 60;
const double EcartTemps = 1.0 / static_cast<double>(IMAGESPARSECONDE);

//
//   TEMPLATE�: CMoteur
//
//   BUT�: Template servant � construire un objet Moteur qui implantera les
//         aspects "g�n�riques" du moteur de jeu
//
//   COMMENTAIRES�:
//
//        Comme plusieurs de nos objets repr�senteront des �l�ments uniques 
//        du syst�me (ex: le moteur lui-m�me, le lien vers 
//        le dispositif Direct3D), l'utilisation d'un singleton 
//        nous simplifiera plusieurs aspects.
//
template <class T, class TClasseDispositif> class CMoteur : public CSingleton<T>
{
public:
	virtual void Run()
	{
		bool bBoucle = true;

		while (bBoucle)
		{
			// Propre � la plateforme - (Conditions d'arr�t, interface, messages)
			bBoucle = RunSpecific();

			// appeler la fonction d'animation
			if (bBoucle)
			{
				bBoucle = Animation();
			}
		}
	}

	virtual int Initialisations()
	{
		// Propre � la plateforme
		InitialisationsSpecific();

		// * Initialisation du dispositif de rendu
		pDispositif = CreationDispositifSpecific(CDS_FENETRE);

		// * Initialisation de la sc�ne
		InitScene();

		// * Initialisation des param�tres de l'animation et 
		//   pr�paration de la premi�re image
		InitAnimation();

		return 0;
	}

	virtual bool Animation()
	{
		// m�thode pour lire l'heure et calculer le 
		// temps �coul�
		const int64_t TempsCompteurCourant = GetTimeSpecific();
		const double TempsEcoule = GetTimeIntervalsInSec(TempsCompteurPrecedent, TempsCompteurCourant);

		// Est-il temps de rendre l'image?
		if (TempsEcoule > EcartTemps)
		{
			// Affichage optimis�
			pDispositif->Present(); // On enlevera �//� plus tard

			// On pr�pare la prochaine image
			AnimeScene(static_cast<float>(TempsEcoule));

			// On rend l'image sur la surface de travail
			// (tampon d'arri�re plan)
			RenderScene();

			// Calcul du temps du prochain affichage
			TempsCompteurPrecedent = TempsCompteurCourant;
		}

		return true;
	}

	const XMMATRIX& GetMatView() const { return m_MatView; }
	const XMMATRIX& GetMatProj() const { return m_MatProj; }
	const XMMATRIX& GetMatViewProj() const { return m_MatViewProj; }

	CGestionnaireDeTextures& GetTextureManager() { return TexturesManager; }

protected:
	virtual ~CMoteur()
	{
		Cleanup();
	}

	// Sp�cifiques - Doivent �tre implant�s
	virtual bool RunSpecific() = 0;
	virtual int InitialisationsSpecific() = 0;

	virtual int64_t GetTimeSpecific() const = 0;
	virtual double GetTimeIntervalsInSec(int64_t start, int64_t stop) const = 0;

	virtual TClasseDispositif* CreationDispositifSpecific(const CDS_MODE cdsMode) = 0;
	virtual void BeginRenderSceneSpecific() = 0;
	virtual void EndRenderSceneSpecific() = 0;

	// Autres fonctions
	virtual int InitAnimation()
	{
		TempsSuivant = GetTimeSpecific();
		TempsCompteurPrecedent = TempsSuivant;

		// premi�re Image
		RenderScene();

		return true;
	}

	// Fonctions de rendu et de pr�sentation de la sc�ne
	virtual bool RenderScene()
	{
		BeginRenderSceneSpecific();

		// Appeler les fonctions de dessin de chaque objet de la sc�ne
		for (const auto& object3D : ListeScene)
		{
			object3D->Draw();
		}

		EndRenderSceneSpecific();
		return true;
	}

	virtual void Cleanup()
	{
		// d�truire les objets
		ListeScene.clear();

		// D�truire le dispositif
		if (pDispositif)
		{
			delete pDispositif;
			pDispositif = nullptr;
		}
	}

	virtual int InitScene()
	{
		// Initialisation des objets 3D - cr�ation et/ou chargement
		if (!InitObjets()) return 1;

		// Initialisation des matrices View et Proj
		// Dans notre cas, ces matrices sont fixes
		m_MatView = XMMatrixLookAtRH(XMVectorSet(0.0f, -15.0f, 5.0f, 1.0f),
			XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

		const float champDeVision = XM_PI / 4; // 45 degr�s
		const float planRapproche = 0.05f;
		const float planEloigne = 400.0f;
		const float ratioDAspect = static_cast<float>(pDispositif->GetLargeur()) / static_cast<float>(pDispositif->
			GetHauteur());

		m_MatProj = XMMatrixPerspectiveFovRH(
			champDeVision,
			ratioDAspect,
			planRapproche,
			planEloigne
		);

		// Calcul de VP � l�avance
		m_MatViewProj = m_MatView * m_MatProj;

		return 0;
	}

	bool InitObjets()
	{
		/*CBlocEffet1* pBloc;

		// Cr�ation d�un cube de 2 X 2 X 2 unit�s
		// Le bloc est cr�� dans notre programme et sur le dispositif
		pBloc = new CBlocEffet1(2, 2, 2, pDispositif);

		// Lui assigner une texture
		pBloc->SetTexture(TexturesManager.GetNewTexture(L"UneTexture.dds", pDispositif));

		// Puis, il est ajout� � la sc�ne
		ListeScene.emplace_back(static_cast<CObjet3D*>(pBloc));*/

		
		// Cr�ation d�un objet sprite
		// auto pSprite = std::make_unique<CSpriteTemp>("tree02s.dds", pDispositif);
		// pSprite->SetPosDim(200, 400);

		// Cr�ation de l�afficheur de sprites et ajout des sprites
		/*std ::unique_ptr<CAfficheurSprite> pAfficheurSprite =
		std ::make_unique<CAfficheurSprite>(pDispositif);

		pAfficheurSprite->AjouterSprite("tree02s.dds", 200,400);
		pAfficheurSprite->AjouterSprite("tree02s.dds", 500,500, 100, 100);
		pAfficheurSprite->AjouterSprite("tree02s.dds", 800,200, 100, 100);

		// Puis, il est ajout� � la sc�ne
		ListeScene.emplace_back(std::move(pAfficheurSprite));*/

		CParametresChargement param;
		param.NomChemin = "E:\\";
		param.NomFichier = "police.obj";

		cout << "=== OBJ ===" << endl;
		const auto startChargeurObj = std::chrono::high_resolution_clock::now();

		CChargeurOBJ chargeur2;
		chargeur2.Chargement(param);
		std::unique_ptr<CObjetMesh> pMesh2 = std::make_unique<CObjetMesh>(chargeur2, pDispositif);
		pMesh2->SetPosition({-2.0f, 0.0f, 0.0f});
		ListeScene.emplace_back(std::move(pMesh2));

		const auto endChargeurObj = std::chrono::high_resolution_clock::now();

		cout << "Temps de chargement OBJ : " << std::chrono::duration_cast<std::chrono::milliseconds>(endChargeurObj - startChargeurObj).count() << "ms" << endl;

		cout << "=== Fastobj ===" << endl;
		const auto startChargeurFastObj = std::chrono::high_resolution_clock::now();

		FastobjChargeur chargeur;
		chargeur.Chargement(param);
		std::unique_ptr<CObjetMesh> pMesh = std::make_unique<CObjetMesh>(chargeur, pDispositif);
		pMesh->SetPosition({2.0f, 0.0f, 0.0f});
		ListeScene.emplace_back(std::move(pMesh));

		const auto endChargeurFastObj = std::chrono::high_resolution_clock::now();
		cout << "Temps de chargement Fastobj : " << std::chrono::duration_cast<std::chrono::milliseconds>(endChargeurFastObj - startChargeurFastObj).count() << "ms" << endl;

		return true;
	}

	bool AnimeScene(float tempsEcoule)
	{
		for (auto& object3D : ListeScene)
		{
			object3D->Anime(tempsEcoule);
		}

		return true;
	}

protected:
	// Variables pour le temps de l'animation
	int64_t TempsSuivant;
	int64_t TempsCompteurPrecedent;

	// Le dispositif de rendu
	TClasseDispositif* pDispositif;

	// La seule sc�ne
	std::vector<std::unique_ptr<CObjet3D>> ListeScene;

	// Les matrices
	XMMATRIX m_MatView;
	XMMATRIX m_MatProj;
	XMMATRIX m_MatViewProj;

	// Le gestionnaire de texture
	CGestionnaireDeTextures TexturesManager;

};

} // namespace PM3D
