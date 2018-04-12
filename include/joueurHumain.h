#ifndef JOUEURHUMAIN_H_INCLUDED
#define JOUEURHUMAIN_H_INCLUDED

#include "jeu.h"
#include "clavier.h"


void initJoueursHumains(StructJeu *jeu);
void deplacerJoueurJ1(StructTouchesClavier *clavier, StructJeu *jeu, int indiceJoueur);
void deplacerJoueurJ2(StructTouchesClavier *clavier, StructJeu *jeu, int indiceJoueur);

void poserBombe(StructJeu *jeu, int indiceJoueur, StructAudio *audio);

#endif // JOUEURHUMAIN_H_INCLUDED
