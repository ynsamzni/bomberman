#ifndef AFFICHAGE_INCLUDED
#define AFFICHAGE_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "constantes.h"
#include "jeu.h"
#include "clavier.h"
#include "menu.h"


typedef struct{
	SDL_Color blanc;
	SDL_Color noir;
}StructCouleur;

typedef struct{
    SDL_Texture *feuilleSprites;
    SDL_Texture *murIndestructible;
    SDL_Texture *murDestructible;
    SDL_Texture *bombe;
    SDL_Texture *explosion;
    SDL_Texture *victoire;
    SDL_Texture *defaite;
    SDL_Texture *flecheBasse;
    SDL_Texture *flecheHaute;
}StructTextures;


typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    StructTextures structTextures;
    StructCouleur structCouleur;
    int numeroFenetre;
}StructAffichage;



void initAffichage(StructAffichage *affichage, char nomFenetre[]);
void chargementTextures(StructTextures *structTextures, SDL_Renderer *renderer);
void chargementCouleurs(StructCouleur *structCouleur);

void afficherJeu(StructAffichage *affichage, StructJeu *jeu);
void afficherTexte(char texte[], int tailleTexte, SDL_Color couleurTexte, char cheminPoliceEcriture[], int positionX, int positionY, SDL_Renderer *renderer);

void afficherMenuPrincipal(StructAffichage *affichage, StructTouchesClavier *clavier, StructMenu *menu);
void afficherMenuSelectionProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu);
void afficherMenuCreationProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu);

void animationVictoire(StructAffichage *affichage);
void animationDefaite(StructAffichage *affichage);



#endif // AFFICHAGE_INCLUDED
