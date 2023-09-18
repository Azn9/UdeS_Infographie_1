#pragma once

namespace PM3D
{
// Constantes pour mode fenêtré ou plein écran
enum CDS_MODE
{
	CDS_FENETRE,
	CDS_PLEIN_ECRAN
};

//
//  Classe : CDispositif
//
//  BUT : 	Classe servant à construire un objet Dispositif 
//				qui implantera les aspects "génériques" du dispositif de 
//				rendu
//
class CDispositif
{
public:
	virtual ~CDispositif() = default;

	virtual void Present();
	virtual void PresentSpecific() = 0;

	int GetLargeur() const { return largeurEcran; }
	int GetHauteur() const { return hauteurEcran; }

protected:
	int largeurEcran;
	int hauteurEcran;
};

} // namespace PM3D
