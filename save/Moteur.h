#pragma once
#include <memory>
#include <vector>

#include "Singleton.h"
#include "dispositif.h"
#include "Objet3D.h"
#include "Bloc.h"

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
template <typename T, typename TClasseDispositif> class CMoteur : public CSingleton<T>
{
public:
	const XMMATRIX& GetMatView() const { return matView; }
	const XMMATRIX& GetMatProj() const { return matProj; }
	const XMMATRIX& GetMatViewProj() const { return matViewProj; }

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

	void AnimeScene(double temps_ecoule)
	{
		for (auto& object3D : ListeScene)
		{
			object3D->Anime(static_cast<float>(temps_ecoule));
		}
		//return true;
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
			AnimeScene(TempsEcoule);

			// On rend l'image sur la surface de travail
			// (tampon d'arrière plan)
			RenderScene();

			// Calcul du temps du prochain affichage
			TempsCompteurPrecedent = TempsCompteurCourant;
		}

		return true;
	}

protected:
	// Les matrices
	XMMATRIX matView;
	XMMATRIX matProj;
	XMMATRIX matViewProj;
	
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
		for (auto& object3D : ListeScene)
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

		// à suivre

		// Détruire le dispositif
		if (pDispositif)
		{
			delete pDispositif;
			pDispositif = nullptr;
		}
	}

	// Variables pour le temps de l'animation
	int64_t TempsSuivant;
	int64_t TempsCompteurPrecedent;

	// Le dispositif de rendu
	TClasseDispositif* pDispositif;

	// La seule scène
	std::vector<std::unique_ptr<CObjet3D>> ListeScene;

	virtual int InitScene()
	{
		// Initialisation des objets 3D - création et/ou chargement
		if (!InitObjets()) return 1;

		// Initialisation des matrices View et Proj
		// Dans notre cas, ces matrices sont fixes
		matView = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
			XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		float champDeVision = XM_PI / 4; // 45 degrés
		float ratioDAspect = 1.0; // horrible, il faudra corriger ça

		float planRapproche = 2.0;
		float planEloigne = 20.0;
		matProj = XMMatrixPerspectiveFovLH(
			champDeVision,
			ratioDAspect,
			planRapproche,
			planEloigne);
		// Calcul de VP à l’avance
		matViewProj = matView * matProj;

		return 0;
	}

	bool InitObjets()
	{
		// Puis, il est ajouté à la scène
		ListeScene.emplace_back(std::make_unique<CBloc>(2.0f, 2.0f, 2.0f, pDispositif));

		return true;
	}
};

} // namespace PM3D
