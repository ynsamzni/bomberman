#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED

#include "jeu.h"
#include "constantes.h"

void initIA(StructJeu *jeu);

void deplacerIA(StructJeu *jeu);

void randomMove(StructJeu *jeu, int indiceJoueur, Direction direction, int situation, int faceAuMur);

void debugRandomMove(int indiceJoueur, Direction direction, int situation, int faceAuMur);

int ennemiDansAxe(int indiceJoueur, StructJeu *jeu);



#endif // IA_H_INCLUDED
