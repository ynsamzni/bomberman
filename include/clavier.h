#ifndef CLAVIER_H_INCLUDED
#define CLAVIER_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constantes.h"

typedef struct{

    int toucheHaut;
    int toucheBas;
    int toucheDroite;
    int toucheGauche;
    int toucheAction;
    int toucheQuitter;

}StructTouchesClavier;

void initClavier(StructTouchesClavier *clavier);
void recupererTouchesClavier(StructTouchesClavier *clavier);
void afficherLaStructureDeTouches(StructTouchesClavier clavier);
int cycleToucheClavierRealise(int *etatToucheClavier, StructTouchesClavier *clavier);

#endif // CLAVIER_H_INCLUDED
