#include <stdio.h>
#include <stdlib.h>

#include "../include/IA.h"

void initIA(StructJeu *jeu)  //Gère l'apparition des IA
{


    if(jeu->nbrDeJoueurs >= 2 && jeu->listeDesJoueurs[1].humainOuIA == 1)                                      //En bas à droite
    {
        jeu->listeDesJoueurs[1].coordonnes.x = WIDTH - 30;
        jeu->listeDesJoueurs[1].coordonnes.y = HEIGHT - 30;
        jeu->listeDesJoueurs[1].direction = HAUT;
    }

    if(jeu->nbrDeJoueurs >= 3)                                  //En haut à droite
    {
        jeu->listeDesJoueurs[2].coordonnes.x = WIDTH - 30;
        jeu->listeDesJoueurs[2].coordonnes.y = 0;
        jeu->listeDesJoueurs[2].direction = GAUCHE;

        if(jeu->nbrDeJoueurs >= 4)
        {
            jeu->listeDesJoueurs[3].coordonnes.x = 0;    //En bas à gauche
            jeu->listeDesJoueurs[3].coordonnes.y = HEIGHT - 30;
            jeu->listeDesJoueurs[3].direction = HAUT;

        }
    }



}

void deplacerIA(StructJeu *jeu)
{
    Coordonnes itineraire[300][300];
    int nbTotalItineraire;

    // Calculer tous les itinéraires disponibles
    nbTotalItineraire = calculerItineraires(1, itineraire, 0, 0, jeu);


    /***** Debug *****/
    int j;
    for(int i = 0; i < nbTotalItineraire; i++)
    {
        j = 0;
        printf("\nItineraire %d :", i);
        while(itineraire[i][j].x != -1 || itineraire[i][j].y != -1)
        {
            printf(" %d/%d", itineraire[i][j].x, itineraire[i][j].y);
            j++;
        }
    }
    printf("\n---------\n");
}


/********** Traitement des itinéraires **********/

int calculerItineraires(int indiceJoueur, Coordonnes itineraire[300][300], int nbItineraire, int nbDeplacement, StructJeu *jeu)
{
    int nbDeplacementIterationActuelle = 0;
    int x, y, xInitial, yInitial;
    Direction direction = HAUT;
    Coordonnes nvllePosition;
    int nbMaxItineraires = 50;

    // Déterminer la position initiale
    xInitial = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x;
    yInitial = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y;

    while(xInitial%30 != 0)
        xInitial -= VITESSE_DES_JOUEURS;
    while(yInitial%30 != 0)
        yInitial -= VITESSE_DES_JOUEURS;

    // Déterminer la dernière position calculée de l'itinéraire
    if(nbDeplacement == 0)
    {
        x = xInitial;
        y = yInitial;
    }
    else
    {
        x = itineraire[nbItineraire][nbDeplacement - 1].x;
        y = itineraire[nbItineraire][nbDeplacement - 1].y;
    }

    // Tester les déplacements : HAUT -> DROITE -> BAS -> GAUCHE
    for(int i=0; i<4; i++)
    {
        // Déterminer la position à étudier
        switch(direction + i)
        {
            case HAUT:
                nvllePosition.x = x;
                nvllePosition.y = y - 30;
                break;
            case DROITE:
                nvllePosition.x = x + 30;
                nvllePosition.y = y;
                break;
            case BAS:
                nvllePosition.x = x;
                nvllePosition.y = y + 30;
                break;
            case GAUCHE:
                nvllePosition.x = x - 30;
                nvllePosition.y = y;
                break;
        }

        // Déterminer si la position étudiée doit faire partie de l'itinéraire
        if(deplacementPossible(x, y, direction + i, jeu)
           && (nbDeplacement == 0 || !rechercherItineraire(nvllePosition.x, nvllePosition.y, itineraire[nbItineraire])) && (nvllePosition.x != xInitial || nvllePosition.y != yInitial)) // Si on n'est pas déjà passé par cette case
        {
           // Passer à un nouvel itineraire si un déplacement a déjà été effectué dans cette itération
            if(nbDeplacementIterationActuelle != 0)
            {
                // Stop si cela fait dépasser le nombre max d'itinéraires autorisés
                if(nbItineraire > nbMaxItineraires - 2)
                    return nbItineraire;

                nbItineraire++;
                for(int i=0; i<=nbDeplacement; i++)
                {
                    itineraire[nbItineraire][i].x = itineraire[nbItineraire-1][i].x;
                    itineraire[nbItineraire][i].y = itineraire[nbItineraire-1][i].y;
                }
            }

            // Actualiser le nombre de déplacements possibles dans cette itération
            nbDeplacementIterationActuelle++;

            // Ajouter le déplacement dans l'itinéraire actuel
            modifierCoordonnees(&itineraire[nbItineraire][nbDeplacement], nvllePosition.x, nvllePosition.y);
            modifierCoordonnees(&itineraire[nbItineraire][nbDeplacement+1], -1, -1);

            // Calculer le prochain déplacement de cet itineraire
            nbItineraire = calculerItineraires(indiceJoueur, itineraire, nbItineraire, nbDeplacement+1, jeu);
        }
    }

    if(nbDeplacement == 0 && nbDeplacementIterationActuelle != 0)
        return nbItineraire + 1;
    else
        return nbItineraire;
}

int longueurItineraire(Coordonnes itineraire[300])
{
    int longueur = 0;

    while(itineraire[longueur].x != -1 || itineraire[longueur].y != -1)
        longueur++;

    return longueur;
}

int rechercherItineraire(int x, int y, Coordonnes itineraire[300])
{
    int resultat = 0;

    for(int i=0; i<longueurItineraire(itineraire); i++)
    {
        if(itineraire[i].x == x && itineraire[i].y == y)
            resultat = i + 1;
    }

    return resultat; // Renvoyer la position
}


/********** Traitement des coordonnées **********/

void modifierCoordonnees(Coordonnes *coordonneesAModifier, int x, int y)
{
    coordonneesAModifier->x = x;
    coordonneesAModifier->y = y;
}
