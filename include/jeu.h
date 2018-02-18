#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constantes.h"
#include "clavier.h"

typedef struct{
    int x;
    int y;
}Coordonnes;

typedef struct{
    Coordonnes coordonnesBombe;
    int tickDePose;
    int etatBombe;

}Bombe;

typedef enum{
    HAUT, BAS, DROITE, GAUCHE
}Direction;

typedef struct{
    int enVie;
    Coordonnes coordonnes;
    Bombe bombe;
    Direction direction;
    int deplacement;
    Coordonnes coordonnesSprite;
}Joueur;

typedef struct{
    int mapJeu[NBR_DE_CASES_HORIZONTALES][NBR_DE_CASES_VERTICALES];
    int nbrDeJoueurs;
    Joueur listeDesJoueurs[4];
}StructJeu;



void initJeu(StructJeu *jeu, int nbrPlayers);
void calculerJeu(StructJeu *jeu, StructTouchesClavier *clavier);

void deplacerJoueur(StructJeu *jeu, StructTouchesClavier *clavier);
void poserBombe(StructJeu *jeu);
void exploserBombe(StructJeu *jeu);
void animerDeplacement(StructJeu *jeu, int Indicejoueur);

void afficherStructureJeu(StructJeu jeu);


int contenuCaseMatrice(StructJeu *jeu, int y, int x);
int renvoitCaseMatrice(int coordonne);
int randProbaParmi4Nb(int val1, int probaVal1, int val2, int probaVal2, int val3, int probaVal3, int val4, int probaVal4);



#endif // JEU_H_INCLUDED
