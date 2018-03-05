#ifndef CLAVIER_H_INCLUDED
#define CLAVIER_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constantes.h"

typedef struct{

    //Touches joueur 1
    int toucheHautJ1;
    int toucheBasJ1;
    int toucheDroiteJ1;
    int toucheGaucheJ1;
    int toucheBombeJ1;

    //Touches joueur 2
    int toucheHautJ2;
    int toucheBasJ2;
    int toucheDroiteJ2;
    int toucheGaucheJ2;
    int toucheBombeJ2;

    //Touches du menu
    int toucheHaut;
    int toucheBas;
    int toucheDroite;
    int toucheGauche;
    int toucheAction;
    int toucheArriere;
    int toucheQuitter;

}StructTouchesClavier;

void initClavier(StructTouchesClavier *clavier);
void recupererTouchesClavier(StructTouchesClavier *clavier);
void afficherLaStructureDeTouches(StructTouchesClavier clavier);
int cycleToucheClavierRealise(int *etatToucheClavier, StructTouchesClavier *clavier);

#endif // CLAVIER_H_INCLUDED
