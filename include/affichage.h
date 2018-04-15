#ifndef AFFICHAGE_INCLUDED
#define AFFICHAGE_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "constantes.h"
#include "jeu.h"
#include "clavier.h"


typedef struct {
	SDL_Color blanc;
	SDL_Color noir;
} StructCouleur;

typedef struct {
    SDL_Texture *feuilleSprites;
    SDL_Texture *murIndestructible;
    SDL_Texture *murDestructible;
    SDL_Texture *bombe;
    SDL_Texture *etincelle;
    SDL_Texture *explosion;
    SDL_Texture *victoire;
    SDL_Texture *defaite;
    SDL_Texture *flecheBasse;
    SDL_Texture *flecheHaute;
    SDL_Texture *flecheGauche;
    SDL_Texture *flecheDroite;
    SDL_Texture *flecheGaucheBlanche;
    SDL_Texture *flecheDroiteBlanche;
} StructTextures;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    StructTextures structTextures;
    StructCouleur structCouleur;
} StructAffichage;

void initAffichage(StructAffichage *affichage, char nomFenetre[]);
void chargementTextures(StructTextures *structTextures, SDL_Renderer *renderer);
void chargementCouleurs(StructCouleur *structCouleur);
void afficherJeu(StructAffichage *affichage, StructJeu *jeu);


#endif // AFFICHAGE_INCLUDED
