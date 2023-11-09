#pragma once
#include <thread>
#include <string>

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
const int IMAGESPARSECONDE = 60;
const int PHYSICS_PER_SECOND = 60;
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
class CMoteur
{
public:
	CMoteur() = default;
	virtual ~CMoteur()
	{
		CMoteur::Cleanup();
	}
	
	virtual void Run();

	std::atomic_bool shouldStepOneFrameUpdate = false;
	std::atomic_bool shouldStepOneFramePhysics = false;

	void StepOneFrame();

	virtual void Resize(WORD largeur, WORD hauteur) = 0;
	virtual void ResizeWindow(int largeur, int hauteur) = 0;
	
	virtual int Initialisations();

	virtual bool Animation();

	void SetGameHost(PM3D_API::GameHost* newGameHost)
	{
		this->gameHost = newGameHost;
	}

	const XMMATRIX& GetMatView() const { return m_MatView; }
	const XMMATRIX& GetMatProj() const { return m_MatProj; }
	const XMMATRIX& GetMatViewProj() const { return m_MatViewProj; }

	CGestionnaireDeTextures& GetTextureManager() { return TexturesManager; }
	CDIManipulateur& GetGestionnaireDeSaisie() {return GestionnaireDeSaisie;}

	virtual double GetLastFrameTime() const
	{
		return lastFrameTime;
	}

	virtual double GetPresentTime() const
	{
		return presentTime;
	}
protected:

	bool canRender = false;
	bool running = true;
	double lastFrameTime = 0.0;
	double presentTime = 0.0;

	// Sp�cifiques - Doivent �tre implant�s
	virtual bool RunSpecific() = 0;
	virtual int InitialisationsSpecific() = 0;

	virtual CDispositifD3D11* CreationDispositifSpecific(const CDS_MODE cdsMode) = 0;
	virtual void InitSceneSpecific() = 0;
	virtual void BeginRenderSceneSpecific() = 0;
	virtual void EndRenderSceneSpecific() = 0;

	// Autres fonctions
	virtual int InitAnimation();

	// Fonctions de rendu et de pr�sentation de la sc�ne
	virtual bool RenderScene();

	virtual void Cleanup();

	virtual int InitScene();

protected:
	PM3D_API::GameHost* gameHost;

	// Variables pour le temps de l'animation
	int64_t TempsSuivant;
	int64_t TempsCompteurPrecedent;

	int64_t LastUpdateTime;
	int64_t LastFixedUpdateTime;

	// Le dispositif de rendu
	CDispositifD3D11* pDispositif;

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
