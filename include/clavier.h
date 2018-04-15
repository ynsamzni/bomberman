#ifndef CLAVIER_H
#define CLAVIER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "constantes.h"


typedef struct {
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
    int toucheSupprimer;
    int toucheArriere;
    int toucheQuitter;
} StructClavier;

void initClavier(StructClavier *clavier);
void recupererEtatTouchesClavier(StructClavier *clavier);
int cycleToucheClavierRealise(int *etatToucheClavier, StructClavier *clavier);
void debugAfficherEtatTouchesClavier(StructClavier clavier);


#endif // CLAVIER_H
