#ifndef AFFICHAGE_INCLUDED
#define AFFICHAGE_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constantes.h"
#include "jeu.h"
#include "clavier.h"


typedef struct{
    SDL_Texture *texturePerso;
    SDL_Texture *textureMur;
    SDL_Texture *textureMur2;
    SDL_Texture *textureBomb;
    SDL_Texture *textureExplosion;
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
