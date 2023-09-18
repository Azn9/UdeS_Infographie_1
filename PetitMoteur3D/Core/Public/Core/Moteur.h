#pragma once
#include <chrono>
#include <locale>
#include <thread>

#include "Core/Public/Util/Singleton.h"
#include "dispositif.h"

#include <vector>

#include "../../../../PM3DApi/Api/Public/GameHost.h"
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
		drawThread.detach();

		// Update thread
		std::thread updateThread([this]
		{
			while (this->running)
			{
				const int64_t currentTime = GetTimeSpecific();
				const double timeElapsed = GetTimeIntervalsInSec(LastUpdateTime, currentTime);

				gameHost->Update(timeElapsed);
				
				LastUpdateTime = currentTime;
			}
		});
		updateThread.detach();

		// FixedUpdate thread
		std::thread fixedUpdateThread([this]
		{
			while (this->running)
			{
				const int64_t currentTime = GetTimeSpecific();
				const double timeElapsed = GetTimeIntervalsInSec(LastFixedUpdateTime, currentTime);

				gameHost->FixedUpdate(timeElapsed);
				
				LastFixedUpdateTime = currentTime;
			}
		});
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

		return true;
	}

	bool AnimeScene(float tempsEcoule)
	{
		
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

};

} // namespace PM3D
