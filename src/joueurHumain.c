#include <stdio.h>
#include <stdlib.h>

#include "../include/joueurHumain.h"


void initJoueursHumains(StructJeu *jeu){
    int a = 0;
    for(int i = 0; i < jeu->nbrDeJoueurs; i++){
        if(jeu->listeDesJoueurs[i].humainOuIA == 0 && a == 0)
            a = 1;
        else if(jeu->listeDesJoueurs[i].humainOuIA == 0){
            jeu->listeDesJoueurs[i].direction = DROITE;
            jeu->listeDesJoueurs[i].coordonnes.x = WIDTH - 30;
            jeu->listeDesJoueurs[i].coordonnes.y = HEIGHT - 30;
        }
    }

}

void poserBombe(StructJeu *jeu, int indiceJoueur)
{
    // Si le joueur a une bombe d'active
    if(jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose == 0)
    {
        int coordonneesJoueurX=renvoitCaseMatrice(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x);
        int coordonneesJoueurY=renvoitCaseMatrice(jeu->listeDesJoueurs[indiceJoueur].coordonnes.y);

        jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose = SDL_GetTicks();

        // Poser la bombe à la position du joueur
        jeu->mapJeu[coordonneesJoueurX][coordonneesJoueurY] = 3;

        // Enregistrer les coordonnées de la bombe posée
        jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.x = coordonneesJoueurX;
        jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.y = coordonneesJoueurY;

        // Marquer la bombe comme posée
        jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe = 1;

    }

}


void deplacerJoueurJ1(StructTouchesClavier *clavier, StructJeu *jeu, int indiceJoueur) //Gère le déplacement du J1
{
    int x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x;
    int y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y;

    if(clavier->toucheHautJ1 == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
        printf("Appui Haut\n");
        if(contenuCaseMatrice(jeu, x, y - 1) == 0 && contenuCaseMatrice(jeu, x + 29, y - 1) == 0)
            y = y - VITESSE_DES_JOUEURS;
    }

    if(clavier->toucheBasJ1 == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
        printf("Appui Bas\n");
        if(contenuCaseMatrice(jeu, x, y + 31) == 0 && contenuCaseMatrice(jeu, x + 29, y + 31)== 0 )
            y = y + VITESSE_DES_JOUEURS;
    }

    if(clavier->toucheDroiteJ1 == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
        printf("Appui Droite\n");
        if(contenuCaseMatrice(jeu, x + 31, y) == 0 && contenuCaseMatrice(jeu, x + 31, y + 29) == 0 )
            x = x + VITESSE_DES_JOUEURS;
    }

    if(clavier->toucheGaucheJ1 == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
        printf("Appui Gauche\n");
        if(contenuCaseMatrice(jeu, x - 1, y) == 0 && contenuCaseMatrice(jeu, x - 1, y + 29) == 0 )
            x = x - VITESSE_DES_JOUEURS;
    }

    if(clavier->toucheHautJ1 == 0 && clavier->toucheBasJ1 == 0 && clavier->toucheGaucheJ1 == 0 && clavier->toucheDroiteJ1 == 0)
        jeu->listeDesJoueurs[indiceJoueur].deplacement = 0;
    else
        jeu->listeDesJoueurs[indiceJoueur].deplacement = 1;

    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x =  x ;
    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = y;
}



void deplacerJoueurJ2(StructTouchesClavier *clavier, StructJeu *jeu, int indiceJoueur) //Gère le déplacement du J1
{
    int x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x;
    int y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y;

    if(clavier->toucheHautJ2 == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
        printf("Appui Haut\n");
        if(contenuCaseMatrice(jeu, x, y - 1) == 0 && contenuCaseMatrice(jeu, x + 29, y - 1) == 0)
            y = y - VITESSE_DES_JOUEURS;
    }

    if(clavier->toucheBasJ2 == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
        printf("Appui Bas\n");
        if(contenuCaseMatrice(jeu, x, y + 31) == 0 && contenuCaseMatrice(jeu, x + 29, y + 31)== 0 )
            y = y + VITESSE_DES_JOUEURS;
    }

    if(clavier->toucheDroiteJ2 == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
        printf("Appui Droite\n");
        if(contenuCaseMatrice(jeu, x + 31, y) == 0 && contenuCaseMatrice(jeu, x + 31, y + 29) == 0 )
            x = x + VITESSE_DES_JOUEURS;
    }

    if(clavier->toucheGaucheJ2 == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
        printf("Appui Gauche\n");
        if(contenuCaseMatrice(jeu, x - 1, y) == 0 && contenuCaseMatrice(jeu, x - 1, y + 29) == 0 )
            x = x - VITESSE_DES_JOUEURS;
    }

    if(clavier->toucheHautJ2 == 0 && clavier->toucheBasJ2 == 0 && clavier->toucheGaucheJ2 == 0 && clavier->toucheDroiteJ2 == 0)
        jeu->listeDesJoueurs[indiceJoueur].deplacement = 0;
    else
        jeu->listeDesJoueurs[indiceJoueur].deplacement = 1;

    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x =  x ;
    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = y;
}

