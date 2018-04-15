#include <stdio.h>
#include <stdlib.h>

#include "../include/joueurHumain.h"


void deplacerJoueurJ1(StructTouchesClavier *clavier, StructJeu *jeu, int indiceJoueur)
{
    int x, y;

    // Si le joueur est en cours de déplacement
    if(clavier->toucheHautJ1 || clavier->toucheBasJ1 || clavier->toucheGaucheJ1 || clavier->toucheDroiteJ1)
    {
        x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x;
        y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y;

        if(clavier->toucheHautJ1)
        {
            printf("Appui Haut J1\n");
            jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
            if(deplacementPossible(x, y, HAUT, jeu))
                y -= VITESSE_DES_JOUEURS;
        }
        if(clavier->toucheBasJ1)
        {
            printf("Appui Bas J1\n");
            jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
            if(deplacementPossible(x, y, BAS, jeu))
                y += VITESSE_DES_JOUEURS;
        }
        if(clavier->toucheDroiteJ1)
        {
            printf("Appui Droite J1\n");
            jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
            if(deplacementPossible(x, y, DROITE, jeu))
                x += VITESSE_DES_JOUEURS;
        }
        if(clavier->toucheGaucheJ1)
        {
            printf("Appui Gauche J1\n");
            jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
            if(deplacementPossible(x, y, GAUCHE, jeu))
                x -= VITESSE_DES_JOUEURS;
        }

        jeu->listeDesJoueurs[indiceJoueur].deplacement = 1;
        jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = x;
        jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = y;
    }
    else
        jeu->listeDesJoueurs[indiceJoueur].deplacement = 0;
}

void deplacerJoueurJ2(StructTouchesClavier *clavier, StructJeu *jeu, int indiceJoueur)
{
    int x;
    int y;

    // Si le joueur est en cours de déplacement
    if(clavier->toucheHautJ2 || clavier->toucheBasJ2 || clavier->toucheGaucheJ2 || clavier->toucheDroiteJ2)
    {
        x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x;
        y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y;

        if(clavier->toucheHautJ2)
        {
            printf("Appui Haut J2\n");
            jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
            if(deplacementPossible(x, y, HAUT, jeu))
                y -= VITESSE_DES_JOUEURS;
        }
        if(clavier->toucheBasJ2)
        {
            printf("Appui Bas J2\n");
            jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
            if(deplacementPossible(x, y, BAS, jeu))
                y += VITESSE_DES_JOUEURS;
        }
        if(clavier->toucheDroiteJ2)
        {
            printf("Appui Droite J2\n");
            jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
            if(deplacementPossible(x, y, DROITE, jeu))
                x += VITESSE_DES_JOUEURS;
        }
        if(clavier->toucheGaucheJ2)
        {
            printf("Appui Gauche J2\n");
            jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
            if(deplacementPossible(x, y, GAUCHE, jeu))
                x -= VITESSE_DES_JOUEURS;
        }

        jeu->listeDesJoueurs[indiceJoueur].deplacement = 1;
        jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = x;
        jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = y;
    }
    else
        jeu->listeDesJoueurs[indiceJoueur].deplacement = 0;
}
