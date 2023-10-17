#pragma once
#include <thread>
#include <string>

#include "../../Public/Util/Singleton.h"
#include "dispositif.h"

#include <vector>

#include "CDIManipulateur.h"
#include "../../../../PM3DApi/Api/Public/GameHost.h"
#include "../../Public/Object/Objet3D.h"
#include "../../Public/Texture/GestionnaireDeTextures.h"
#include "../../Public/Sprite/AfficheurSprite.h"
#include "../../Public/Sprite/SpriteTemp.h"
namespace PM3D
{

const int IMAGESPARSECONDE = 3;
const int PHYSICS_PER_SECOND = 3;
const double EcartTemps = 1.0 / static_cast<double>(IMAGESPARSECONDE);
const double FixedEcartTemps = 1.0 / static_cast<double>(PHYSICS_PER_SECOND);

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
		// Draw thread
		std::thread drawThread([this]
		{
			while (this->running)
			{
				if (!Animation())
				{
					this->running = false;
				}
			}
		});
		SetThreadName(drawThread, "DrawThread");
		drawThread.detach();

		// Update thread
		std::thread updateThread([this]
		{
			while (this->running)
			{
				const int64_t currentTime = GetTimeSpecific();
				const double timeElapsed = GetTimeIntervalsInSec(LastUpdateTime, currentTime);

				if (timeElapsed > EcartTemps)
				{
					gameHost->Update(timeElapsed);
				
					LastUpdateTime = currentTime;
				}
			}
		});
		SetThreadName(updateThread, "UpdateThread");
		updateThread.detach();

		// FixedUpdate thread
		std::thread fixedUpdateThread([this]
		{
			while (this->running)
			{
				const int64_t currentTime = GetTimeSpecific();
				const double timeElapsed = GetTimeIntervalsInSec(LastFixedUpdateTime, currentTime);

				if (timeElapsed > FixedEcartTemps)
				{
					gameHost->FixedUpdate(timeElapsed);
				
					LastFixedUpdateTime = currentTime;
				}
			}
		});
		SetThreadName(fixedUpdateThread, "FixedUpdateThread");
		fixedUpdateThread.detach();

		while (this->running)
		{
			// Propre � la plateforme - (Conditions d'arr�t, interface, messages)
			if (!RunSpecific())
			{
				this->running = false;
			}
		}
	}

	virtual int Initialisations()
	{
		// Propre � la plateforme
		InitialisationsSpecific();

		// * Initialisation du dispositif de rendu
		pDispositif = CreationDispositifSpecific(CDS_FENETRE);
		gameHost->SetDispositif(pDispositif);

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

			// On rend l'image sur la surface de travail
			// (tampon d'arri�re plan)
			RenderScene();

			// Calcul du temps du prochain affichage
			TempsCompteurPrecedent = TempsCompteurCourant;
		}

		return true;
	}

	void SetGameHost(PM3D_API::GameHost* newGameHost)
	{
		this->gameHost = newGameHost;
	}

	const XMMATRIX& GetMatView() const { return m_MatView; }
	const XMMATRIX& GetMatProj() const { return m_MatProj; }
	const XMMATRIX& GetMatViewProj() const { return m_MatViewProj; }

	CGestionnaireDeTextures& GetTextureManager() { return TexturesManager; }
	CDIManipulateur& GetGestionnaireDeSaisie() {return GestionnaireDeSaisie;}

protected:
	virtual ~CMoteur()
	{
		CMoteur::Cleanup();
	}

	bool running = true;

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

		gameHost->Draw();

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
		gameHost->InitializeScene();

		return 0;
	}

	bool InitObjets()
	{
/*
		FastobjChargeur chargeur;
		chargeur.Chargement(param);
		std::unique_ptr<CObjetMesh> pMesh = std::make_unique<CObjetMesh>(chargeur, pDispositif);
		pMesh->SetPosition({0.0f, 0.0f, 0.0f});
		ListeScene.emplace_back(std::move(pMesh));

		std ::unique_ptr<CAfficheurSprite> pAfficheurSprite = std ::make_unique<CAfficheurSprite>(pDispositif);

		pAfficheurSprite->AjouterSprite("DebugTexture.dds", 200, 400, 10, 10);
		pAfficheurSprite->AjouterPanneau("DebugTexture.dds", XMFLOAT3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);

		CAfficheurTexte::Init();
        const Gdiplus::FontFamily oFamily(L"Arial", nullptr);
        auto pPolice = std::make_unique<Gdiplus::Font>(&oFamily, 16.0f,
        Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
        auto pTexte1 = std::make_unique<CAfficheurTexte>(pDispositif, 256, 256, pPolice.get());
        pAfficheurSprite->AjouterSpriteTexte(pTexte1->GetTextureView(), 0, 257);

		pTexte1->Ecrire(L"Test de texte");

		ListeScene.emplace_back(std::move(pAfficheurSprite));
		*/
		return true;
	}

	bool AnimeScene(float tempsEcoule)
	{
		// Prendre en note le statut du clavier
		GestionnaireDeSaisie.StatutClavier();

		// Prendre en note l’état de la souris
		GestionnaireDeSaisie.SaisirEtatSouris();
		
		return true;
	}

protected:
	PM3D_API::GameHost* gameHost;

	// Variables pour le temps de l'animation
	int64_t TempsSuivant;
	int64_t TempsCompteurPrecedent;

	int64_t LastUpdateTime;
	int64_t LastFixedUpdateTime;

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

	CDIManipulateur GestionnaireDeSaisie;

	virtual void SetThreadName(std::thread&, const std::string&) {}
};

} // namespace PM3D
