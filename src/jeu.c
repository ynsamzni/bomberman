#include <stdio.h>
#include <stdlib.h>

#include "../include/jeu.h"
#include "../include/IA.h"

void initJeu(StructJeu *jeu, int nbrPlayers)
{

    //Chargement de la map ( à placer dans une focntions externe si possible mais attention, tableau double à passer en paramètre super chiant )
    printf("Chargement de map.dat\n");
    FILE *fic;
    fic = fopen (CHEMIN_D_ACCES_FICHIER_NIVEAU, "r");

    if(fic == NULL)
    {
        printf("Echec ouverture fichier !\n");

        // Générer une nouvelle map de manière aléatoire
        for(int i=0; i<NBR_DE_CASES_HORIZONTALES; i++)
        {
            for(int j=0; j<NBR_DE_CASES_VERTICALES; j++)
                jeu->mapJeu[i][j]=randProbaParmi4Nb(0, 40,
                                                    1, 26,
                                                    2, 34, 3, 0);
        }

    }
    else
    {
        fread(&(jeu->mapJeu), sizeof(jeu->mapJeu), 1, fic);

        fclose(fic);
        printf("Lecture du fichier et chargement de la map réussit\n");

    }


    jeu->nbrDeJoueurs = nbrPlayers; // Enregistre dans la structure le nbr de joueurs

    for(int i = 0; i < jeu->nbrDeJoueurs; i++)  // Mets toutes les coordonées de tous les joueurs à 0
    {
        jeu->listeDesJoueurs[i].humainOuIA = 0; //Par défault, tous les joueurs sont des Humains (certains transformés en IA dans initIA()
        jeu->listeDesJoueurs[i].enVie = 1;
        jeu->listeDesJoueurs[i].coordonnes.x = 0;
        jeu->listeDesJoueurs[i].coordonnes.y = 0;
        jeu->listeDesJoueurs[i].direction = BAS; //Etat initial du perso lors de l'apparition
        jeu->listeDesJoueurs[i].deplacement = 1;

        jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x = 0;
        jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y = 0;
        jeu->listeDesJoueurs[i].bombe.tickDePose = 0;
        jeu->listeDesJoueurs[i].bombe.etatBombe = 0;

    }

    initIA(jeu);

}




void calculerJeu(StructJeu *jeu, StructTouchesClavier *clavier)
{
    for(int i = 0; i < jeu->nbrDeJoueurs; i++)
    {

        if(jeu->listeDesJoueurs[i].humainOuIA == 0) //Action humain
        {
            if(clavier->toucheBombe == 1)
                poserBombe(jeu, i);
            else
                deplacerJoueur(clavier, jeu, i);
        }

        exploserBombe(jeu, i); // Actions joueur + IA
    }
    // Action IA
    deplacerIA(jeu);


}

/**********************************************************************/
/*******************POSE DES BOMBES************************************/
/**********************************************************************/

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

/***************************************************************************************/
/*******************GESTION DE L'EXPLOSION DES BOMBES***********************************/
/***************************************************************************************/

void exploserBombe(StructJeu *jeu, int indiceJoueur)
{
    if( (SDL_GetTicks() - jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose > 1000) &&  (jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe == 1) )
    {
        int X=jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.x;
        int Y=jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.y;

        int longueurExplosion = 10;
        int hautDestructible=1, droiteDestructible=1, basDestructible=1, gaucheDestructible=1;

        for(int cmpt=0; cmpt<longueurExplosion; cmpt++)
        {
            // Déterminer les directions dans lesquelles il ne doit plus avoir d'explosion
            if(Y-cmpt < 0 || jeu->mapJeu[X][Y-cmpt] == 1)
                hautDestructible=0;
            if(X+cmpt >= 20 || jeu->mapJeu[X+cmpt][Y] == 1)
                droiteDestructible=0;
            if(Y+cmpt >= 20 || jeu->mapJeu[X][Y+cmpt] == 1)
                basDestructible=0;
            if(X-cmpt < 0 || jeu->mapJeu[X-cmpt][Y] == 1)
                gaucheDestructible=0;

            // Exploser dans les directions autorisées
            if(hautDestructible == 1)
                jeu->mapJeu[X][Y-cmpt] = 4;
            if(droiteDestructible == 1)
                jeu->mapJeu[X+cmpt][Y] = 4;
            if(basDestructible == 1)
                jeu->mapJeu[X][Y+cmpt] = 4;
            if(gaucheDestructible == 1)
                jeu->mapJeu[X-cmpt][Y] = 4;
        }

        jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe = 2;
    }

    else if((SDL_GetTicks() - jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose > 1500 ) &&  (jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe == 2) )
    {
        for(int i = 0; i < 20; i++)
        {
            for(int j = 0; j<20; j++)
            {
                if(jeu->mapJeu[i][j] == 4)
                    jeu->mapJeu[i][j] = 0;
            }
        }
        jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe = 0;
        jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose = 0;
    }
}

/*********************************************************************************/
/*******************DEPLACEMENTS ET COLLISIONS************************************/
/*********************************************************************************/

void deplacerJoueur(StructTouchesClavier *clavier, StructJeu *jeu, int indiceJoueur) //Gère le déplacement des joueurs et les collisions
{


    int x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x;
    int y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y;


    if(clavier->toucheHaut == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
        printf("Appui Haut\n");
        if(contenuCaseMatrice(jeu, x, y - 1) == 0 && contenuCaseMatrice(jeu, x + 29, y - 1) == 0)
        {
            y = y - VITESSE_DES_JOUEURS;

        }
    }

    if(clavier->toucheBas == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
        printf("Appui Bas\n");
        if(contenuCaseMatrice(jeu, x, y + 31) == 0 && contenuCaseMatrice(jeu, x + 29, y + 31)== 0 )
        {
            y = y + VITESSE_DES_JOUEURS;
        }
    }

    if(clavier->toucheDroite == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
        printf("Appui Droite\n");
        if(contenuCaseMatrice(jeu, x + 31, y) == 0 && contenuCaseMatrice(jeu, x + 31, y + 29) == 0 )
        {
            x = x + VITESSE_DES_JOUEURS;
        }
    }

    if(clavier->toucheGauche == 1)
    {
        jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
        printf("Appui Gauche\n");
        if(contenuCaseMatrice(jeu, x - 1, y) == 0 && contenuCaseMatrice(jeu, x - 1, y + 29) == 0 )
        {
            x = x - VITESSE_DES_JOUEURS;
        }
    }

    if(clavier->toucheHaut == 0 && clavier->toucheBas == 0 && clavier->toucheGauche == 0 && clavier->toucheDroite == 0)
    {
        jeu->listeDesJoueurs[indiceJoueur].deplacement = 0;
    }
    else
    {
        jeu->listeDesJoueurs[indiceJoueur].deplacement = 1;
    }

    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x =  x ;
    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = y;

}


/*********************************************************************************/
/**********************AFFICHER LA STRUCTURE**************************************/
/*********************************************************************************/


void afficherStructureJeu(StructJeu jeu)
{


    printf("Nombre de joueurs = %d \n", jeu.nbrDeJoueurs);
    for(int i = 0; i < jeu.nbrDeJoueurs; i++)
    {


        printf("   Joueur %d \n", i+1);
        printf("   Type : %d \n", jeu.listeDesJoueurs[i].humainOuIA);
        printf("   Coordonnée X : %d \n", jeu.listeDesJoueurs[i].coordonnes.x);
        printf("   Coordonnée Y : %d \n", jeu.listeDesJoueurs[i].coordonnes.y);
        if(jeu.listeDesJoueurs[i].direction == HAUT)
        {
            printf("   Direction : HAUT \n");
        }
        if(jeu.listeDesJoueurs[i].direction == BAS)
        {
            printf("   Direction : BAS \n");
        }
        if(jeu.listeDesJoueurs[i].direction == DROITE)
        {
            printf("   Direction : DROITE \n");
        }
        if(jeu.listeDesJoueurs[i].direction == GAUCHE)
        {
            printf("   Direction : GAUCHE \n");
        }
        printf("   Déplacement : %d \n",jeu.listeDesJoueurs[i].deplacement);
        printf("      Bombe \n");
        printf("      Coordonnée Bombe X : %d \n", jeu.listeDesJoueurs[i].bombe.coordonnesBombe.x);
        printf("      Coordonnée Bombe Y : %d \n", jeu.listeDesJoueurs[i].bombe.coordonnesBombe.y);
        printf("      Nbr Ticks : %d \n", jeu.listeDesJoueurs[i].bombe.tickDePose);
        printf("\n****************************************\n");
    }
}

/*********************************************************************************/
/*****************FONCTIONS POUR SIMPLIFIER LE CODE*******************************/
/*********************************************************************************/


int contenuCaseMatrice(StructJeu *jeu, int x, int y)
{
    if( x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
        return 1;
    else
        return jeu->mapJeu[renvoitCaseMatrice(x)][renvoitCaseMatrice(y)];
}


int renvoitCaseMatrice(int coordonne)
{
    return (coordonne/30);
}

int randProbaParmi4Nb(int val1, int probaVal1, int val2, int probaVal2, int val3, int probaVal3, int val4, int probaVal4)
{
    // Générer un nombre aléatoire compris entre 0 et 100
    int val = rand() % 101;

    // Si la probabilité de val1 est atteinte
    if (val <= probaVal1)
        return val1;
    // Si la probabilité de val2 est atteinte
    else if (val <= probaVal1 + probaVal2)
        return val2;
    // Si la probabilité de val3 est atteinte
    else if (val <= probaVal1 + probaVal2 + probaVal3)
        return val3;
    // Si la probabilité de val4 est atteinte
    else
        return val4;

}



