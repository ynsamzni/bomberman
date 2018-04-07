#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED

#include "jeu.h"
#include "constantes.h"

void initIA(StructJeu *jeu);
void deplacerIA(int indiceJoueur, StructJeu *jeu);
int calculerItineraires(int indiceJoueur, Coordonnes itineraire[300][300], int nbItineraire, int nbDeplacement, StructJeu *jeu);
void suivreItineraire(int indiceJoueur, StructJeu *jeu);
void comparerItineraires(int indiceJoueur, Coordonnes itineraire[300][300], int nbTotalItineraire, StructJeu *jeu, Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES]);
int comparerItineraireEloignementDangerosite(Coordonnes itineraire[300], Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES], int *longueurMeilleurItineraire);
int comparerItineraireRapprochementTarget(int x, int y, Coordonnes itineraire[300], Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES], int *longueurMeilleurItineraire);
int longueurItineraire(Coordonnes itineraire[300]);
int rechercherItineraire(int x, int y, Coordonnes itineraire[300]);
void copierItineraire(Coordonnes itineraireSrc[300], Coordonnes itineraireDest[300]);
void supprimerDeplacementItineraire(int deplacementASupprimer, Coordonnes itineraire[300]);
void calculerCasesDangereuses(Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES], StructJeu *jeu);
int coordonneesDangereuses(int x, int y, Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES]);
int itineraireDangereux(Coordonnes itineraire[300], Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES]);
int ennemiProche(int indiceJoueur, int distanceMax, StructJeu *jeu);
void modifierCoordonnees(Coordonnes *coordonneesAModifier, int x, int y);
void copierCoordonnees(Coordonnes *coordonneesSrc, Coordonnes *coordonneesDest);
int poseBombeDangereuse(int indiceJoueur, StructJeu *jeu);

#endif // IA_H_INCLUDED
