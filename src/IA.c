#include <stdio.h>
#include <stdlib.h>

#include "../include/IA.h"

void initIA(StructJeu *jeu)  //Gère l'apparition des IA
{


    if(jeu->nbrDeJoueurs >= 2)                                      //En bas à droite
    {
        jeu->listeDesJoueurs[1].humainOuIA = 1;
        jeu->listeDesJoueurs[1].coordonnes.x = WIDTH - 30;
        jeu->listeDesJoueurs[1].coordonnes.y = HEIGHT - 30;
        jeu->listeDesJoueurs[1].direction = HAUT;

        if(jeu->nbrDeJoueurs >= 3)                                  //En haut à droite
        {
            jeu->listeDesJoueurs[2].humainOuIA = 1;
            jeu->listeDesJoueurs[2].coordonnes.x = WIDTH - 30;
            jeu->listeDesJoueurs[2].coordonnes.y = 0;
            jeu->listeDesJoueurs[2].direction = GAUCHE;

            if(jeu->nbrDeJoueurs >= 4)
            {
                jeu->listeDesJoueurs[3].humainOuIA = 1;
                jeu->listeDesJoueurs[3].coordonnes.x = 0;    //En bas à gauche
                jeu->listeDesJoueurs[3].coordonnes.y = HEIGHT - 30;
                jeu->listeDesJoueurs[3].direction = HAUT;

            }
        }
    }


}


void deplacerIA(StructJeu *jeu)  //Permet de déplacer une IA
{
    int x, y;
    int faceAuMur;

    /* POINT INITIALE :
            - vérification case à droite x + 31
            - vérification case à gauche x - 1
        DEUXIEME POINT
            - vérification case à droite x + 31 / y + 29
            - vérification case à gauche x - 1 / y + 29
    */

    for(int i = 1; i < jeu->nbrDeJoueurs; i++)
    {
        if(jeu->listeDesJoueurs[i].humainOuIA == 1 && jeu->listeDesJoueurs[i].enVie == 1)
        {
            x = jeu->listeDesJoueurs[i].coordonnes.x;
            y = jeu->listeDesJoueurs[i].coordonnes.y;
            faceAuMur = 0;




            /*******************HAUT************************************/
            if(jeu->listeDesJoueurs[i].direction == HAUT)
            {
                if(y%30 != 0 ) //Si le perso n'est pas juste une case
                {
                    jeu->listeDesJoueurs[i].coordonnes.y = y - VITESSE_DES_JOUEURS;
                }
                else //Sinon, il est nécessairement juste sur une case
                {

                    if(contenuCaseMatrice(jeu, x, y - 1) != 0)
                    {
                        faceAuMur = 1;
                    }
                    //1 - SI UNIQUEMENT LA CASE DE DROITE EST VIDE
                    if(contenuCaseMatrice(jeu, x - 1, y)  != 0 && contenuCaseMatrice(jeu, x + 31, y) == 0)
                    {
                        randomMove(jeu, i, HAUT, 1, faceAuMur);
                    }
                    //2 - SI UNIQUEMENT LA CASE DE GAUCHE EST VIDE
                    else if(contenuCaseMatrice(jeu, x -1, y) == 0 && contenuCaseMatrice(jeu, x + 31, y) != 0)
                    {
                        randomMove(jeu, i, HAUT, 2, faceAuMur);
                    }
                    //3 - SI LES CASES DE DROITE ET GAUCHE SONT VIDES
                    else if(contenuCaseMatrice(jeu, x - 1, y) == 0 && contenuCaseMatrice(jeu, x + 31, y) == 0)
                    {
                        randomMove(jeu, i, HAUT, 3, faceAuMur);
                    }
                    //SI LA CASE DE DROITE ET DE GAUCHE SONT PLEINES ET faceAuMur = 0
                    else if(contenuCaseMatrice(jeu, x - 1, y) != 0 && contenuCaseMatrice(jeu, x + 31, y) != 0 && faceAuMur == 0)
                    {
                        jeu->listeDesJoueurs[i].coordonnes.y = y - VITESSE_DES_JOUEURS;
                    }
                    //SI LA CASE DE DROITE ET DE GAUCHE SONT PLEINES ET faceAuMur = 0
                    else if(contenuCaseMatrice(jeu, x - 1, y) != 0 && contenuCaseMatrice(jeu, x + 31, y) != 0 && faceAuMur == 1)
                    {
                        jeu->listeDesJoueurs[i].direction = BAS;
                        jeu->listeDesJoueurs[i].coordonnes.y = y + VITESSE_DES_JOUEURS;
                    }

                }
            }

            /****************************BAS************************************/
            else if(jeu->listeDesJoueurs[i].direction == BAS)
            {
                if(y%30 != 0) //Si le perso n'est pas juste sur une case
                {
                    jeu->listeDesJoueurs[i].coordonnes.y = y + VITESSE_DES_JOUEURS;
                }
                else //Sinon, il est nécessairement juste sur une case
                {

                    if(contenuCaseMatrice(jeu, x, y + 31) != 0)
                    {
                        faceAuMur = 1;
                    }
                    //1 - SI UNIQUEMENT LA CASE DE DROITE EST VIDE
                    if(contenuCaseMatrice(jeu, x - 1, y)  != 0 && contenuCaseMatrice(jeu, x + 31, y) == 0)
                    {
                        randomMove(jeu, i, BAS, 1, faceAuMur);
                    }
                    //2 - SI UNIQUEMENT LA CASE DE GAUCHE EST VIDE
                    else if(contenuCaseMatrice(jeu, x -1, y) == 0 && contenuCaseMatrice(jeu, x + 31, y) != 0)
                    {
                        randomMove(jeu, i, BAS, 2, faceAuMur);
                    }
                    //3 - SI LES CASES DE DROITE ET GAUCHE SONT VIDES
                    else if(contenuCaseMatrice(jeu, x - 1, y) == 0 && contenuCaseMatrice(jeu, x + 31, y) == 0)
                    {
                        randomMove(jeu, i, BAS, 3, faceAuMur);
                    }
                    //SI LA CASE DE DROITE ET DE GAUCHE SONT PLEINES ET faceAuMur = 0
                    else if(contenuCaseMatrice(jeu, x - 1, y) != 0 && contenuCaseMatrice(jeu, x + 31, y) != 0 && faceAuMur == 0)
                    {
                        jeu->listeDesJoueurs[i].coordonnes.y = y + VITESSE_DES_JOUEURS;
                    }
                    //SI LA CASE DE DROITE ET DE GAUCHE SONT PLEINES ET faceAuMur = 0
                    else if(contenuCaseMatrice(jeu, x - 1, y) != 0 && contenuCaseMatrice(jeu, x + 31, y) != 0 && faceAuMur == 1)
                    {
                        jeu->listeDesJoueurs[i].direction = HAUT;
                        jeu->listeDesJoueurs[i].coordonnes.y = y - VITESSE_DES_JOUEURS;
                    }

                }

            }

            /*******************************DROITE***************************/
            else if(jeu->listeDesJoueurs[i].direction == DROITE)
            {
                if(x%30 != 0) //Si le perso n'est pas juste sur une case
                {
                    jeu->listeDesJoueurs[i].coordonnes.x = x + VITESSE_DES_JOUEURS;
                }
                else //Sinon, il est nécessairement juste sur une case
                {

                    if(contenuCaseMatrice(jeu, x + 31, y) != 0)
                    {
                        faceAuMur = 1;
                    }
                    //1 - SI UNIQUEMENT LA CASE DU HAUT EST VIDE
                    if(contenuCaseMatrice(jeu, x, y - 1)  == 0 && contenuCaseMatrice(jeu, x, y + 31) != 0)
                    {
                        randomMove(jeu, i, DROITE, 1, faceAuMur);
                    }
                    //2 - SI UNIQUEMENT LA CASE DU BAS EST VIDE
                    else if(contenuCaseMatrice(jeu, x, y - 1)  != 0 && contenuCaseMatrice(jeu, x, y + 31) == 0)
                    {
                        randomMove(jeu, i, DROITE, 2, faceAuMur);
                    }
                    //3 - SI LES CASES DU HAUT ET DU BAS SONT VIDES
                    else if(contenuCaseMatrice(jeu, x, y - 1)  == 0 && contenuCaseMatrice(jeu, x, y + 31) == 0)
                    {
                        randomMove(jeu, i, DROITE, 3, faceAuMur);
                    }
                    //SI LES CASES DU HAUT ET DU BAS SONT PLEINES ET faceAuMur = 0
                    else if(contenuCaseMatrice(jeu, x, y - 1)  != 0 && contenuCaseMatrice(jeu, x, y + 31) != 0 && faceAuMur == 0)
                    {
                        jeu->listeDesJoueurs[i].coordonnes.x = x + VITESSE_DES_JOUEURS;
                    }
                    //SI LES CASES DU HAUT ET DU BAS SONT PLEINES ET faceAuMur = 1
                    else if(contenuCaseMatrice(jeu, x, y - 1)  != 0 && contenuCaseMatrice(jeu, x, y + 31) != 0 && faceAuMur == 1)
                    {
                        jeu->listeDesJoueurs[i].direction = GAUCHE;
                        jeu->listeDesJoueurs[i].coordonnes.x = x - VITESSE_DES_JOUEURS;
                    }

                }

            }

            /*******************************GAUCHE***************************/
            else if(jeu->listeDesJoueurs[i].direction == GAUCHE)
            {
                if( x%30 != 0) //Si le perso n'est pas juste sur une case
                {
                    jeu->listeDesJoueurs[i].coordonnes.x = x - VITESSE_DES_JOUEURS;
                }
                else //Sinon, il est nécessairement juste sur une case
                {

                    if(contenuCaseMatrice(jeu, x - 1, y) != 0)
                    {
                        faceAuMur = 1;
                    }
                    //1 - SI UNIQUEMENT LA CASE DU HAUT EST VIDE
                    if(contenuCaseMatrice(jeu, x, y - 1)  == 0 && contenuCaseMatrice(jeu, x, y + 31) != 0)
                    {
                        randomMove(jeu, i, GAUCHE, 1, faceAuMur);
                    }
                    //2 - SI UNIQUEMENT LA CASE DU BAS EST VIDE
                    else if(contenuCaseMatrice(jeu, x, y - 1)  != 0 && contenuCaseMatrice(jeu, x, y + 31) == 0)
                    {
                        randomMove(jeu, i, GAUCHE, 2, faceAuMur);
                    }
                    //3 - SI LES CASES DU HAUT ET DU BAS SONT VIDES
                    else if(contenuCaseMatrice(jeu, x, y - 1)  == 0 && contenuCaseMatrice(jeu, x, y + 31) == 0)
                    {
                        randomMove(jeu, i, GAUCHE, 3, faceAuMur);
                    }
                    //SI LES CASES DU HAUT ET DU BAS SONT PLEINES ET faceAuMur = 0
                    else if(contenuCaseMatrice(jeu, x, y - 1)  != 0 && contenuCaseMatrice(jeu, x, y + 31) != 0 && faceAuMur == 0)
                    {
                        jeu->listeDesJoueurs[i].coordonnes.x = x - VITESSE_DES_JOUEURS;
                    }
                    //SI LES CASES DU HAUT ET DU BAS SONT PLEINES ET faceAuMur = 1
                    else if(contenuCaseMatrice(jeu, x, y - 1)  != 0 && contenuCaseMatrice(jeu, x, y + 31) != 0 && faceAuMur == 1)
                    {
                        jeu->listeDesJoueurs[i].direction = DROITE;
                        jeu->listeDesJoueurs[i].coordonnes.x = x + VITESSE_DES_JOUEURS;
                    }

                }

            }


        }

        printf("faceAuMur = %d \n", faceAuMur);

    }
}

/**********************************************************************/
/**********GESTION DU CHOIX DE DIRECTION ALEATOIRE*********************/
/**********************************************************************/

void randomMove(StructJeu *jeu, int indiceJoueur, Direction direction, int situation, int faceAuMur)
{
    debugRandomMove(indiceJoueur, direction, situation, faceAuMur);


    int  randomizer;
    /*********************************************HAUT*********************************************************************/

    if(direction == HAUT)
    {
        if(situation == 1) //CASE DROITE VIDE
        {
            randomizer = rand()%3+1;  /* nombre aléatoire compris entre 1 et 3 */
            if(randomizer == 1 || randomizer == 2)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
                }

                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 3)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
            }

        }
        if(situation == 2) //CASE GAUCHE VIDE
        {
            randomizer = rand()%3+1;
            if(randomizer == 1 || randomizer == 2)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
                }

                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 3)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
            }

        }
        if(situation == 3) //CASE DROITE ET GAUCHE VIDE
        {
            randomizer = rand()%6+1;
            if(randomizer == 1 || randomizer == 2 || randomizer == 3 || randomizer == 4)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
                }

                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 5)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
            }
            if(randomizer == 6)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
            }

        }
    }

    /*********************************************BAS*********************************************************************/


    else if(direction == BAS)
    {
        if(situation == 1) //CASE DROITE VIDE
        {
            randomizer = rand()%3+1;  /* nombre aléatoire compris entre 1 et 3 */
            if(randomizer == 1 || randomizer == 2)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
                }

                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 3)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
            }

        }
        if(situation == 2) //CASE GAUCHE VIDE
        {
            randomizer = rand()%3+1;
            if(randomizer == 1 || randomizer == 2)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
                }

                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 3)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
            }

        }
        if(situation == 3) //CASE DROITE ET GAUCHE VIDE
        {
            randomizer = rand()%6+1;
            if(randomizer == 1 || randomizer == 2 || randomizer == 3 || randomizer == 4)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
                }

                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 5)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
            }
            if(randomizer == 6)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
            }

        }
    }


    /***********************************DROITE*************************************************************/


    else if(direction == DROITE)
    {
        if(situation == 1) //CASE HAUT VIDE
        {
            randomizer = rand()%3+1;  /* nombre aléatoire compris entre 1 et 3 */
            if(randomizer == 1 || randomizer == 2)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
                }
                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 3)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
            }

        }
        if(situation == 2) //CASE BAS VIDE
        {
            randomizer = rand()%3+1;
            if(randomizer == 1 || randomizer == 2)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
                }
                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 3)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
            }

        }
        if(situation == 3) //CASE HAUT ET BAS VIDE
        {
            randomizer = rand()%6+1;
            if(randomizer == 1 || randomizer == 2 || randomizer == 3 || randomizer == 4)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
                }
                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 5)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
            }
            if(randomizer == 6)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
            }

        }
    }

    /**************************************GAUCHE**************************************************************/


    else if(direction == GAUCHE)
    {
        if(situation == 1) //CASE HAUT VIDE
        {
            randomizer = rand()%3+1;  /* nombre aléatoire compris entre 1 et 3 */
            if(randomizer == 1 || randomizer == 2)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
                }
                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 3)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
            }

        }
        if(situation == 2) //CASE BAS VIDE
        {
            randomizer = rand()%3+1;
            if(randomizer == 1 || randomizer == 2)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
                }
                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 3)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
            }

        }
        if(situation == 3) //CASE HAUT ET BAS VIDE
        {
            randomizer = rand()%6+1;
            if(randomizer == 1 || randomizer == 2 || randomizer == 3 || randomizer == 4)
            {
                if(faceAuMur == 0)
                {
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - VITESSE_DES_JOUEURS;
                }
                if(faceAuMur == 1)
                {
                    jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
                    jeu->listeDesJoueurs[indiceJoueur].coordonnes.x = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + VITESSE_DES_JOUEURS;
                }
            }
            if(randomizer == 5)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - VITESSE_DES_JOUEURS;
            }
            if(randomizer == 6)
            {
                jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + VITESSE_DES_JOUEURS;
            }

        }
    }




}



void debugRandomMove(int indiceJoueur, Direction direction, int situation, int faceAuMur)
{

    if(direction == HAUT)
    {
        printf("L'IA %d va dans la direction HAUT et est dans la situation %d  et faceAuMur = %d \n", indiceJoueur, situation, faceAuMur);
    }
    if(direction == BAS)
    {
        printf("L'IA %d va dans la direction BAS et est dans la situation %d et faceAuMur = %d \n", indiceJoueur, situation, faceAuMur);
    }
    if(direction == DROITE)
    {
        printf("L'IA %d va dans la direction DROITE et est dans la situation %d et faceAuMur = %d \n", indiceJoueur, situation, faceAuMur);
    }
    if(direction == GAUCHE)
    {
        printf("L'IA %d va dans la direction GAUCHE et est dans la situation %d et faceAuMur = %d \n", indiceJoueur, situation, faceAuMur);
    }


}




