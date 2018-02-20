#ifndef AFFICHAGE_INCLUDED
#define AFFICHAGE_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constantes.h"
#include "jeu.h"
#include "clavier.h"


typedef struct{
    SDL_Texture *feuilleSprites;
    SDL_Texture *murIndestructible;
    SDL_Texture *murDestructible;
    SDL_Texture *bombe;
    SDL_Texture *explosion;
}StructTextures;


typedef struct{

    SDL_Window *window;
    SDL_Renderer *renderer;
    StructTextures structTextures;

}StructAffichage;



void initAffichage(StructAffichage *affichage, char nomFenetre[]);
void chargementTextures(StructTextures *structTextures, SDL_Renderer *renderer);

void afficherJeu(StructAffichage *affichage, StructJeu *jeu);




#endif // AFFICHAGE_INCLUDED
