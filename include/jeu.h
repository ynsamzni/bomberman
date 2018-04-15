#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constantes.h"
#include "clavier.h"
#include "audio.h"


typedef struct {
    int x;
    int y;
} Coordonnes;

typedef enum {
    ON, OFF, LANCEMENT, EXTINCTION, PAUSE
} EtatJeu;

typedef struct {
    Coordonnes coordonnesBombe;
    int tickDePose;
    int etatBombe;
} Bombe;

typedef enum {
    HAUT, BAS, DROITE, GAUCHE
} Direction;

typedef struct {
    char nom[TAILLE_MAX_NOM_COMPTE];
    int nbrVictoires;
    int nbrDefaites;
} CompteJoueur;

typedef struct {
    int humainOuIA;
    CompteJoueur compte;
    int enVie;
    Coordonnes coordonnes;
    Bombe bombe;
    Direction direction;
    int deplacement;
    int modeAleatoire;
    Coordonnes itineraireSuivi[100];
} Joueur;

typedef struct {
    int victoire;
    int defaite;

} Animations;

typedef struct {
    EtatJeu etat;
    int mapJeu[NBR_DE_CASES_HORIZONTALES][NBR_DE_CASES_VERTICALES];
    int nbrDeJoueurs;
    Joueur listeDesJoueurs[4];
    Animations animations;
} StructJeu;

void initMap(StructJeu *jeu);
void initJoueurs(StructJeu *jeu);
void initJeu(StructJeu *jeu);
void calculerJeu(StructJeu *jeu, StructTouchesClavier *clavier, StructAudio *audio);
void exploserBombe(StructJeu *jeu, int indiceJoueur, StructAudio *audio);
void poserBombe(StructJeu *jeu, int indiceJoueur, StructAudio *audio);
void afficherStructureJeu(StructJeu jeu);
int deplacementPossible(int x, int y, Direction direction, StructJeu *jeu);
int contenuCoordonnees(StructJeu *jeu, int y, int x);
int renvoitCaseMatrice(int coordonne);
int randProbaParmi4Nb(int val1, int probaVal1, int val2, int probaVal2, int val3, int probaVal3, int val4, int probaVal4);
void tuerJoueur(StructJeu *jeu, StructAudio *audio);
void checkVictoire(StructJeu *jeu, StructAudio *audio);
void joueurNbrVictoireOuDefaitePlusUn(StructJeu *jeu, int indiceJoueur, int victoireOuDefaite);


#endif // JEU_H_INCLUDED
