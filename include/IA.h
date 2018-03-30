#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED

#include "jeu.h"
#include "constantes.h"

void initIA(StructJeu *jeu);
void deplacerIA(int indiceJoueur, StructJeu *jeu);
int calculerItineraires(int indiceJoueur, Coordonnes itineraire[300][300], int nbItineraire, int nbDeplacement, StructJeu *jeu);
void suivreItineraire(int indiceJoueur, StructJeu *jeu);
void comparerItineraires(int indiceJoueur, Coordonnes itineraire[300][300], int nbTotalItineraire, StructJeu *jeu, Coordonnes casesDangereuses[148]);
int comparerItineraireEloignementDangerosite(Coordonnes itineraire[300], Coordonnes casesDangereuses[148], int *longueurMeilleurItineraire);
int comparerItineraireRapprochementTarget(int x, int y, Coordonnes itineraire[300], Coordonnes casesDangereuses[148], int *longueurMeilleurItineraire);
int longueurItineraire(Coordonnes itineraire[300]);
int rechercherItineraire(int x, int y, Coordonnes itineraire[300]);
void copierItineraire(Coordonnes itineraireSrc[300], Coordonnes itineraireDest[300], int longueurACopier);
void supprimerDeplacementItineraire(int deplacementASupprimer, Coordonnes itineraire[300]);
void calculerCasesDangereuses(Coordonnes casesDangereuses[148], StructJeu *jeu);
int coordonneesDangereuses(int x, int y, Coordonnes casesDangereuses[148]);
int itineraireDangereux(Coordonnes itineraire[300], Coordonnes casesDangereuses[148]);
int ennemiDansAxe(int indiceJoueur, StructJeu *jeu);
void modifierCoordonnees(Coordonnes *coordonneesAModifier, int x, int y);
void copierCoordonnees(Coordonnes *coordonneesSrc, Coordonnes *coordonneesDest);


#endif // IA_H_INCLUDED
