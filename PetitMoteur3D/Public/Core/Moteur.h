#pragma once
#include <locale>

#include "Util/Singleton.h"
#include "dispositif.h"

#include <vector>

#include "Object/Objet3D.h"
#include "Texture/GestionnaireDeTextures.h"
#include "Mesh/ObjectMesh.h"
#include "Util/ChargeurOBJ.h"
#include "Mesh/chargeur.h"
#include "Sprite/SpriteTemp.h"
 
namespace PM3D
{

const int IMAGESPARSECONDE = 60;
const double EcartTemps = 1.0 / static_cast<double>(IMAGESPARSECONDE);

//
//   TEMPLATE : CMoteur
//
//   BUT : Template servant à construire un objet Moteur qui implantera les
//         aspects "génériques" du moteur de jeu
//
//   COMMENTAIRES :
//
//        Comme plusieurs de nos objets représenteront des éléments uniques 
//        du système (ex: le moteur lui-même, le lien vers 
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
			// Propre à la plateforme - (Conditions d'arrêt, interface, messages)
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
		// Propre à la plateforme
		InitialisationsSpecific();

		// * Initialisation du dispositif de rendu
		pDispositif = CreationDispositifSpecific(CDS_FENETRE);

		// * Initialisation de la scène
		InitScene();

		// * Initialisation des paramètres de l'animation et 
		//   préparation de la première image
		InitAnimation();

		return 0;
	}

	virtual bool Animation()
	{
		// méthode pour lire l'heure et calculer le 
		// temps écoulé
		const int64_t TempsCompteurCourant = GetTimeSpecific();
		const double TempsEcoule = GetTimeIntervalsInSec(TempsCompteurPrecedent, TempsCompteurCourant);

		// Est-il temps de rendre l'image?
		if (TempsEcoule > EcartTemps)
		{
			// Affichage optimisé
			pDispositif->Present(); // On enlevera «//» plus tard

			// On prépare la prochaine image
			AnimeScene(static_cast<float>(TempsEcoule));

			// On rend l'image sur la surface de travail
			// (tampon d'arrière plan)
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

	// Spécifiques - Doivent être implantés
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

		// première Image
		RenderScene();

		return true;
	}

	// Fonctions de rendu et de présentation de la scène
	virtual bool RenderScene()
	{
		BeginRenderSceneSpecific();

		// Appeler les fonctions de dessin de chaque objet de la scène
		for (const auto& object3D : ListeScene)
		{
			object3D->Draw();
		}

		EndRenderSceneSpecific();
		return true;
	}

	virtual void Cleanup()
	{
		// détruire les objets
		ListeScene.clear();

		// Détruire le dispositif
		if (pDispositif)
		{
			delete pDispositif;
			pDispositif = nullptr;
		}
	}

	virtual int InitScene()
	{
		// Initialisation des objets 3D - création et/ou chargement
		if (!InitObjets()) return 1;

		// Initialisation des matrices View et Proj
		// Dans notre cas, ces matrices sont fixes
		m_MatView = XMMatrixLookAtRH(XMVectorSet(0.0f, -150.0f, 50.0f, 1.0f),
			XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

		const float champDeVision = XM_PI / 4; // 45 degrés
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

		// Calcul de VP à l’avance
		m_MatViewProj = m_MatView * m_MatProj;

		return 0;
	}

	bool InitObjets()
	{
		/*CBlocEffet1* pBloc;

		// Création d’un cube de 2 X 2 X 2 unités
		// Le bloc est créé dans notre programme et sur le dispositif
		pBloc = new CBlocEffet1(2, 2, 2, pDispositif);

		// Lui assigner une texture
		pBloc->SetTexture(TexturesManager.GetNewTexture(L"UneTexture.dds", pDispositif));

		// Puis, il est ajouté à la scène
		ListeScene.emplace_back(static_cast<CObjet3D*>(pBloc));*/

		//CObjetMesh* pMesh;


		// Constructeur avec format binaire
		CChargeurOBJ chargeur;
		CParametresChargement parametresChargement;
		parametresChargement.NomChemin = "";
		parametresChargement.NomFichier = "test6.obj";

		chargeur.Chargement(parametresChargement);

		std::unique_ptr<CObjetMesh> pMesh = std::make_unique<CObjetMesh>(chargeur, pDispositif);

		// Création d’un objet sprite
		auto pSprite = std::make_unique<CSpriteTemp>("tree02s.dds", pDispositif);

		// Puis, il est ajouté à la scène
		ListeScene.emplace_back(std::move(pSprite));


		// Puis, il est ajouté à la scène
		ListeScene.emplace_back(std::move(pMesh));

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

	// La seule scène
	std::vector<std::unique_ptr<CObjet3D>> ListeScene;

	// Les matrices
	XMMATRIX m_MatView;
	XMMATRIX m_MatProj;
	XMMATRIX m_MatViewProj;

	// Le gestionnaire de texture
	CGestionnaireDeTextures TexturesManager;

};

} // namespace PM3D
