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
                                                    2, 30,
                                                    3, 4);
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
        jeu->listeDesJoueurs[i].enVie = 1;
        jeu->listeDesJoueurs[i].coordonnes.x = 0;
        jeu->listeDesJoueurs[i].coordonnes.y = 0;
        jeu->listeDesJoueurs[i].direction = GAUCHE; //Etat initial du perso lors de l'apparition
        jeu->listeDesJoueurs[i].deplacement = 1;
        jeu->listeDesJoueurs[i].coordonnesSprite.x = 15; //Coordonnée de départ du découpage du sprite
        jeu->listeDesJoueurs[i].coordonnesSprite.y = 13; //Coordonnée de départ du découpage du sprite
    }


    for(int i = 0; i < jeu->nbrDeJoueurs ; i++) // Mets toutes les coordonées de toutes les bombes, ainsi que leur Tick à 0
    {
        jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x = 0;
        jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y = 0;
        jeu->listeDesJoueurs[i].bombe.tickDePose = 0;
        jeu->listeDesJoueurs[i].bombe.etatBombe = 0;

    }

    initIA(jeu);

}




void calculerJeu(StructJeu *jeu, StructTouchesClavier *clavier)
{
    // Action joueur
    if(clavier->toucheBombe == 1)
        poserBombe(jeu);
    else
        deplacerJoueur(jeu, clavier);

    // Action IA
    deplacerIA(jeu);

    // Actions joueur + IA
    exploserBombe(jeu);
    for(int i = 0; i < jeu->nbrDeJoueurs; i++)
    {
        animerDeplacement(jeu, i);
    }

}

/**********************************************************************/
/*******************POSE DES BOMBES************************************/
/**********************************************************************/

void poserBombe(StructJeu *jeu)
{

    if(jeu->listeDesJoueurs[0].bombe.tickDePose == 0)
    {

        jeu->listeDesJoueurs[0].bombe.tickDePose = SDL_GetTicks();
        jeu->mapJeu[renvoitCaseMatrice(jeu->listeDesJoueurs[0].coordonnes.y)][renvoitCaseMatrice(jeu->listeDesJoueurs[0].coordonnes.x)] = 3;
        jeu->listeDesJoueurs[0].bombe.coordonnesBombe.x = renvoitCaseMatrice(jeu->listeDesJoueurs[0].coordonnes.x);
        jeu->listeDesJoueurs[0].bombe.coordonnesBombe.y = renvoitCaseMatrice(jeu->listeDesJoueurs[0].coordonnes.y);

        jeu->listeDesJoueurs[0].bombe.etatBombe = 1;

    }

}

/***************************************************************************************/
/*******************GESTION DE L'EXPLOSION DES BOMBES***********************************/
/***************************************************************************************/

void exploserBombe(StructJeu *jeu)
{
    for(int i = 0; i<jeu->nbrDeJoueurs; i++)
    {
        if( (SDL_GetTicks() - jeu->listeDesJoueurs[i].bombe.tickDePose > 1000) &&  (jeu->listeDesJoueurs[i].bombe.etatBombe == 1) )
        {


            jeu->mapJeu[jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y][jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x] = 4;

            int X,Y;

            int stop = 0;
            int cmpt = 1;
            int longueurExplosion = 10;

            X = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x;
            Y = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y;

            while(cmpt < longueurExplosion && stop != 1)
            {
                X = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x;
                Y = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y + cmpt;
                if(X >= 0 && X <= 20 && Y >= 0 && Y <= 20 && jeu->mapJeu[Y][X] != 1)
                {
                    jeu->mapJeu[Y][X] = 4;
                    cmpt++;
                }
                else
                {
                    stop = 1;
                }
            }


            stop = 0;
            cmpt = 1;
            while(cmpt < longueurExplosion && stop != 1)
            {
                X = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x;
                Y = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y - cmpt;
                if(X >= 0 && X <= 20 && Y >= 0 && Y <= 20 && jeu->mapJeu[Y][X] != 1)
                {
                    jeu->mapJeu[Y][X] = 4;
                    cmpt++;
                }
                else
                {
                    stop = 1;
                }
            }

            stop = 0;
            cmpt = 1;
            while(cmpt < longueurExplosion && stop != 1)
            {
                X = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x + cmpt;
                Y = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y;
                if(X >= 0 && X <= 20 && Y >= 0 && Y <= 20 && jeu->mapJeu[Y][X] != 1)
                {
                    jeu->mapJeu[Y][X] = 4;
                    cmpt++;
                }
                else
                {
                    stop = 1;
                }
            }

            stop = 0;
            cmpt = 1;
            while(cmpt < longueurExplosion && stop != 1)
            {
                X = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x - cmpt;
                Y = jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y;
                if(X >= 0 && X <= 20 && Y >= 0 && Y <= 20 && jeu->mapJeu[Y][X] != 1)
                {
                    jeu->mapJeu[Y][X] = 4;
                    cmpt++;
                }
                else
                {
                    stop = 1;
                }
            }
            jeu->listeDesJoueurs[i].bombe.etatBombe = 2;
        }

        else if((SDL_GetTicks() - jeu->listeDesJoueurs[i].bombe.tickDePose > 1500 ) &&  (jeu->listeDesJoueurs[i].bombe.etatBombe == 2) )
        {
            for(int i = 0; i < 20; i++)
            {
                for(int j = 0; j<20; j++)
                {
                    if(jeu->mapJeu[j][i] == 4)  //inversion de j et i pour faciliter la lecture la matrice map
                    {
                        jeu->mapJeu[j][i] = 0;
                    }
                }
            }
            jeu->listeDesJoueurs[i].bombe.etatBombe = 0;
            jeu->listeDesJoueurs[i].bombe.tickDePose = 0;
        }
    }
}

/*********************************************************************************/
/*******************DEPLACEMENTS ET COLLISIONS************************************/
/*********************************************************************************/

void deplacerJoueur(StructJeu *jeu, StructTouchesClavier *clavier) //Gère le déplacement des joueurs et les collisions
{


    int x = jeu->listeDesJoueurs[0].coordonnes.x;
    int y = jeu->listeDesJoueurs[0].coordonnes.y;


    if(clavier->toucheHaut == 1)
    {
        jeu->listeDesJoueurs[0].direction = HAUT;
        printf("Appui Haut\n");
        if(contenuCaseMatrice(jeu, x, y - 1) == 0 && contenuCaseMatrice(jeu, x + 29, y - 1) == 0)
        {
            y = y - VITESSE_DES_JOUEURS;

        }
    }

    if(clavier->toucheBas == 1)
    {
        jeu->listeDesJoueurs[0].direction = BAS;
        printf("Appui Bas\n");
        if(contenuCaseMatrice(jeu, x, y + 31) == 0 && contenuCaseMatrice(jeu, x + 29, y + 31)== 0 )
        {
            y = y + VITESSE_DES_JOUEURS;
        }
    }

    if(clavier->toucheDroite == 1)
    {
        jeu->listeDesJoueurs[0].direction = DROITE;
        printf("Appui Droite\n");
        if(contenuCaseMatrice(jeu, x + 31, y) == 0 && contenuCaseMatrice(jeu, x + 31, y + 29) == 0 )
        {
            x = x + VITESSE_DES_JOUEURS;
        }
    }

    if(clavier->toucheGauche == 1)
    {
        jeu->listeDesJoueurs[0].direction = GAUCHE;
        printf("Appui Gauche\n");
        if(contenuCaseMatrice(jeu, x - 1, y) == 0 && contenuCaseMatrice(jeu, x - 1, y + 29) == 0 )
        {
            x = x - VITESSE_DES_JOUEURS;
        }
    }

    if(clavier->toucheHaut == 0 && clavier->toucheBas == 0 && clavier->toucheGauche == 0 && clavier->toucheDroite == 0)
    {
        jeu->listeDesJoueurs[0].deplacement = 0;
    }
    else
    {
        jeu->listeDesJoueurs[0].deplacement = 1;
    }

    jeu->listeDesJoueurs[0].coordonnes.x =  x ;
    jeu->listeDesJoueurs[0].coordonnes.y = y;

}



/*********************************************************************************/
/**********************CALCUL DECOUPAGE DU SPRITE*********************************/
/*********************************************************************************/


void animerDeplacement(StructJeu *jeu, int Indicejoueur)
{

    int check = 0;

    if(jeu->listeDesJoueurs[Indicejoueur].deplacement == 1)
    {

        if(jeu->listeDesJoueurs[Indicejoueur].direction == HAUT)
        {
            check = 1;
            jeu->listeDesJoueurs[Indicejoueur].coordonnesSprite.y = 13;
        }

        if(jeu->listeDesJoueurs[Indicejoueur].direction == BAS)
        {
            check = 1;
            jeu->listeDesJoueurs[Indicejoueur].coordonnesSprite.y = 13 + 2*64;
        }

        if(jeu->listeDesJoueurs[Indicejoueur].direction == DROITE)
        {
            check = 1;
            jeu->listeDesJoueurs[Indicejoueur].coordonnesSprite.y = 13 + 3*64;
        }

        if(jeu->listeDesJoueurs[Indicejoueur].direction == GAUCHE)
        {
            check = 1;
            jeu->listeDesJoueurs[Indicejoueur].coordonnesSprite.y = 13 + 64;
        }
        if(check == 1)
        {
            jeu->listeDesJoueurs[Indicejoueur].coordonnesSprite.x = jeu->listeDesJoueurs[Indicejoueur].coordonnesSprite.x + 64;
            if(jeu->listeDesJoueurs[Indicejoueur].coordonnesSprite.x > 512)
            {
                jeu->listeDesJoueurs[Indicejoueur].coordonnesSprite.x = 15;
            }
        }
    }

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
    if( 0 > x || x > WIDTH || 0 > y || y > HEIGHT)
    {
        return 1;
    }
    else
    {
        return jeu->mapJeu[renvoitCaseMatrice(y)][renvoitCaseMatrice(x)];
    }
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
