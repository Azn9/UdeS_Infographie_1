#include "StdAfx.h"
#include "../../Public/Core/Moteur.h"

#include "../../../../PM3DApi/Api/Public/Input/Input.h"
#include "Core/Public/Util/Time.h"

void PM3D::CMoteur::Run()
{
	// Update thread
	std::thread updateThread([=]
	{
		while (running)
		{
			const int64_t currentTime = Time::GetInstance().GetTimeSpecific();

			if (LastUpdateTime == 0)
			{
				LastUpdateTime = currentTime;
				continue;
			}
				
			const double timeElapsed = Time::GetInstance().GetTimeIntervalsInSec(LastUpdateTime, currentTime);

			if (timeElapsed > EcartTemps || shouldStepOneFrameUpdate)
			{
				const auto timeElapsedF = static_cast<float>(timeElapsed);
				if (shouldStepOneFrameUpdate != false)
				{
					Time::GetInstance().SetUpdateDeltaTime(timeElapsedF);
					shouldStepOneFrameUpdate = false;
				} else
				{
					Time::GetInstance().SetUpdateDeltaTime(timeElapsedF * Time::GetInstance().GetTimeScale());
				}

				GestionnaireDeSaisie.SaisirEtatSouris();
				GestionnaireDeSaisie.StatutClavier();

				Input::GetInstance().Tick(GestionnaireDeSaisie);

				if (Time::GetInstance().GetUpdateDeltaTime() != 0)
					gameHost->Update();
				
				LastUpdateTime = currentTime;
			}
		}
	});
	SetThreadName(updateThread, "UpdateThread");
	updateThread.detach();

	// PhysicsUpdate thread
	std::thread physicsUpdateThread([=]
	{
		while (running)
		{
			const int64_t currentTime = Time::GetInstance().GetTimeSpecific();

			if (LastFixedUpdateTime == 0)
			{
				LastFixedUpdateTime = currentTime;
				continue;
			}
				
			const double timeElapsed = Time::GetInstance().GetTimeIntervalsInSec(LastFixedUpdateTime, currentTime);

			if (timeElapsed > FixedEcartTemps || shouldStepOneFramePhysics)
			{
				const auto timeElapsedF = static_cast<float>(timeElapsed);
				if (shouldStepOneFramePhysics != false)
				{
					shouldStepOneFramePhysics = false;
						
					Time::GetInstance().SetPhysicsDeltaTime(timeElapsedF);
				} else
				{
					Time::GetInstance().SetPhysicsDeltaTime(timeElapsedF * Time::GetInstance().GetTimeScale());
				}

				if (Time::GetInstance().GetPhysicsDeltaTime() != 0)
					gameHost->PhysicsUpdate();
				
				LastFixedUpdateTime = currentTime;
			}
		}
	});
	SetThreadName(physicsUpdateThread, "PhysicsThread");
	physicsUpdateThread.detach();

	while (this->running)
	{
		// Propre ? la plateforme - (Conditions d'arr?t, interface, messages)
		if (!RunSpecific())
		{
			this->running = false;
		}
			
		if (!canRender) continue;
				
		if (!Animation())
		{
			this->running = false;
		}
	}
}

void PM3D::CMoteur::StepOneFrame()
{
	shouldStepOneFrameUpdate = true;
	shouldStepOneFramePhysics = true;
}

int PM3D::CMoteur::Initialisations()
{
	// Propre ? la plateforme
	InitialisationsSpecific();

	// * Initialisation du dispositif de rendu
	pDispositif = CreationDispositifSpecific(CDS_FENETRE);
	gameHost->SetDispositif(pDispositif);

	//Resize(1280, 720);
	//ResizeWindow(1280, 720);

	// * Initialisation de la sc?ne
	InitScene();

	// * Initialisation des param?tres de l'animation et 
	//   pr?paration de la premi?re image
	InitAnimation();

	return 0;
}

bool PM3D::CMoteur::Animation()
{
	// m?thode pour lire l'heure et calculer le 
	// temps ?coul?
	const int64_t TempsCompteurCourant = Time::GetInstance().GetTimeSpecific();
	const double TempsEcoule = Time::GetInstance().GetTimeIntervalsInSec(TempsCompteurPrecedent, TempsCompteurCourant);

	// Est-il temps de rendre l'image?
	if (TempsEcoule > EcartTemps)
	{
		uint64_t start = Time::GetInstance().GetTimeSpecific();

		if (canRender)
		{
			// Affichage optimis?
			pDispositif->Present(); // On enlevera ?//? plus tard

			const uint64_t end = Time::GetInstance().GetTimeSpecific();
			presentTime = Time::GetInstance().GetTimeIntervalsInSec(start, end) * 1000.0;

			// On rend l'image sur la surface de travail
			// (tampon d'arri?re plan)
			RenderScene();
		}

		// Calcul du temps du prochain affichage
		TempsCompteurPrecedent = TempsCompteurCourant;
	}

	return true;
}

int PM3D::CMoteur::InitAnimation()
{
	TempsSuivant = Time::GetInstance().GetTimeSpecific();
	TempsCompteurPrecedent = TempsSuivant;

	// premi?re Image
	//RenderScene();

	canRender = true;

	return true;
}

bool PM3D::CMoteur::RenderScene()
{
	if (!canRender)
	{
		ImGui::EndFrame();
		return true;
	}
		
	const uint64_t start = Time::GetInstance().GetTimeSpecific();
		
	BeginRenderSceneSpecific();
		
	gameHost->Draw();
	gameHost->DrawUI();

	EndRenderSceneSpecific();

	const uint64_t end = Time::GetInstance().GetTimeSpecific();

	lastFrameTime = Time::GetInstance().GetTimeIntervalsInSec(start, end) * 1000.0;
		
	return true;
}

void PM3D::CMoteur::Cleanup()
{
	// d?truire les objets
	ListeScene.clear();

	// D?truire le dispositif
	if (pDispositif)
	{
		delete pDispositif;
		pDispositif = nullptr;
	}
}

int PM3D::CMoteur::InitScene()
{
	InitSceneSpecific();
		
	gameHost->InitializeScene();

	return 0;
}
