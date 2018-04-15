#ifndef AFFICHAGE_H
#define AFFICHAGE_H

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
    SDL_Texture *flecheBasseNoire;
    SDL_Texture *flecheHauteNoire;
    SDL_Texture *flecheGaucheNoire;
    SDL_Texture *flecheDroiteNoire;
} StructTextures;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    StructTextures structTextures;
    StructCouleur structCouleur;
} StructAffichage;

void initAffichage(StructAffichage *affichage, char nomFenetre[]);
void chargerTextures(StructTextures *structTextures, SDL_Renderer *renderer);
void chargerCouleurs(StructCouleur *structCouleur);
void afficherJeu(StructAffichage *affichage, StructJeu *jeu);
void afficherTexte(char texte[], int tailleTexte, SDL_Color couleurTexte, char cheminPoliceEcriture[], int positionX, int positionY, SDL_Renderer *renderer);


#endif // AFFICHAGE_H
