#pragma once
#include "Horloge.h"
#include "Singleton.h"

namespace PM3D
{
constexpr int ImagePerSecond = 60;
constexpr double EcartTemps = 1.0 / static_cast<double>(ImagePerSecond);

//
//   TEMPLATE : CMoteur
//
//   BUT : Template servant à construire un objet Moteur qui implantera les
//         aspects "génériques" du moteur de jeu
//
//   COMMENTAIRES :
//
//        Comme plusieurs de nos objets représenteront des éléments uniques 
//        du système (ex: le moteur lui-même, le lien vers 
//        le dispositif Direct3D), l'utilisation d'un singleton 
//        nous simplifiera plusieurs aspects.
//
template <class T> class CMoteur : public CSingleton<T>
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

		// * Initialisation de la scène

		// * Initialisation des paramètres de l'animation et 
		//   préparation de la première image
		InitAnimation();

		return 0;
	}

	virtual bool Animation()
	{
		// méthode pour lire l’heure et calculer le
		// temps écoulé
		const int64_t TempsCompteurCourant = GetTimeSpecific();
		const double TempsEcoule = GetTimeIntervalsInSec(TempsCompteurPrecedent, TempsCompteurCourant);
		// Est-il temps de rendre l’image ?
		if (TempsEcoule > EcartTemps)
		{
			// Affichage optimisé
			// pDispositif->Present() ; // On enlevera « // » plus tard
			// On prépare la prochaine image
			// AnimeScene(TempsEcoule) ;
			// On rend l’image sur la surface de travail
			// (tampon d’arrière plan)
			RenderScene();
			// Calcul du temps du prochain affichage
			TempsCompteurPrecedent = TempsCompteurCourant;
		}

		return true;
	}

protected:
	virtual ~CMoteur() = default;

	Horloge m_Horloge;

	// Variables pour le temps de l'animation
	int64_t TempsSuivant = 0;
	int64_t TempsCompteurPrecedent = 0;

	// Spécifiques - Doivent être implantés
	virtual bool RunSpecific() = 0;
	virtual int InitialisationsSpecific() = 0;

	virtual int InitAnimation()
	{
		TempsSuivant = GetTimeSpecific();

		TempsCompteurPrecedent = TempsSuivant;

		// première Image
		RenderScene();

		return true;
	}

	virtual bool RenderScene()
	{
		return true;
	}

	virtual int64_t GetTimeSpecific() =0;
	virtual double GetTimeIntervalsInSec(int64_t start, int64_t stop) = 0;

};

} // namespace PM3D
