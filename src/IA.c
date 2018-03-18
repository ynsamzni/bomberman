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
        jeu->listeDesJoueurs[1].itineraireSuivi[0].x = -1;
        jeu->listeDesJoueurs[1].itineraireSuivi[0].y = -1;
    }

    if(jeu->nbrDeJoueurs >= 3)                                  //En haut à droite
    {
        jeu->listeDesJoueurs[2].coordonnes.x = WIDTH - 30;
        jeu->listeDesJoueurs[2].coordonnes.y = 0;
        jeu->listeDesJoueurs[2].direction = GAUCHE;
        jeu->listeDesJoueurs[2].itineraireSuivi[0].x = -1;
        jeu->listeDesJoueurs[2].itineraireSuivi[0].y = -1;

        if(jeu->nbrDeJoueurs >= 4)
        {
            jeu->listeDesJoueurs[3].coordonnes.x = 0;    //En bas à gauche
            jeu->listeDesJoueurs[3].coordonnes.y = HEIGHT - 30;
            jeu->listeDesJoueurs[3].direction = HAUT;
            jeu->listeDesJoueurs[3].itineraireSuivi[0].x = -1;
            jeu->listeDesJoueurs[3].itineraireSuivi[0].y = -1;
        }
    }
}

void deplacerIA(StructJeu *jeu)
{
    Coordonnes itineraire[300][300];
    Coordonnes casesDangereuses[148];
    int nbTotalItineraire;

    // Calculer les cases dangereuses
    calculerCasesDangereuses(casesDangereuses, jeu);

    // Si un nouvel itinéraire doit être généré
    if(itineraireDangereux(jeu->listeDesJoueurs[1].itineraireSuivi, casesDangereuses) || longueurItineraire(jeu->listeDesJoueurs[1].itineraireSuivi) == 0)
    {
        // Calculer tous les itinéraires disponibles
        nbTotalItineraire = calculerItineraires(1, itineraire, 0, 0, jeu);

        // Déterminer l'itinéraire à prendre
        comparerItineraires(jeu->listeDesJoueurs[0].coordonnes.x, jeu->listeDesJoueurs[0].coordonnes.y, itineraire, nbTotalItineraire, jeu, casesDangereuses);
    }


    /***** Debug *****/

    printf("\nItineraire :");
    for(int j=0; j< longueurItineraire(jeu->listeDesJoueurs[1].itineraireSuivi); j++)
        printf(" %d/%d", jeu->listeDesJoueurs[1].itineraireSuivi[j].x, jeu->listeDesJoueurs[1].itineraireSuivi[j].y);

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

    int i = 0;
    while(casesDangereuses[i].x != -1 || casesDangereuses[i].y != -1)
    {
        printf("\nBombe %d/%d", casesDangereuses[i].x, casesDangereuses[i].y);
        i++;
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

void comparerItineraires(int x, int y, Coordonnes itineraire[300][300], int nbTotalItineraire, StructJeu *jeu, Coordonnes casesDangereuses[148])
{
    int numeroMeilleurItineraire = -1;
    int longueurMeilleurItineraire = -1;

    // Si le target se trouve sur deux cases
    while(x%30 != 0)
        x -= VITESSE_DES_JOUEURS;
    while(y%30 != 0)
        y -= VITESSE_DES_JOUEURS;

    // Si l'IA se trouve sur une case dangereuse : Trouver l'itinéraire qui mène le plus rapidement à une case non dangereuse
    if(coordonneesDangereuses(jeu->listeDesJoueurs[1].coordonnes.x, jeu->listeDesJoueurs[1].coordonnes.y, casesDangereuses))
    {
        // Analyser l'itinéraire en cours
        if(longueurItineraire(jeu->listeDesJoueurs[1].itineraireSuivi) != 0)
            comparerItineraireEloignementDangerosite(jeu->listeDesJoueurs[1].itineraireSuivi, casesDangereuses, &longueurMeilleurItineraire);

        // Analyser l'ensemble des itinéraires calculés
        for(int i = 0; i < nbTotalItineraire; i++)
        {
            if(comparerItineraireEloignementDangerosite(itineraire[i], casesDangereuses, &longueurMeilleurItineraire))
                numeroMeilleurItineraire = i;
        }
    }

    // Si l'IA n'est pas en danger : Trouver l'itinéraire qui permet de se rapprocher le plus rapidement du target
    else
    {
        // Analyser l'itinéraire en cours
        if(longueurItineraire(jeu->listeDesJoueurs[1].itineraireSuivi) != 0)
            comparerItineraireRapprochementTarget(x, y, jeu->listeDesJoueurs[1].itineraireSuivi, casesDangereuses, &longueurMeilleurItineraire);

        // Analyser l'ensemble des itinéraires calculés
        for(int i = 0; i < nbTotalItineraire; i++)
        {
            if(comparerItineraireRapprochementTarget(x, y, itineraire[i], casesDangereuses, &longueurMeilleurItineraire))
                numeroMeilleurItineraire = i;
        }
    }

    // Si l'IA n'a aucun itinéraire à suivre
/*
    if(numeroMeilleurItineraire == -1)
    {
        printf("\nAUCUN JOUEUR EN VUE");

        // Choisir un itinéraire au hasard
        numeroMeilleurItineraire = rand() % nbTotalItineraire;
        longueurMeilleurItineraire = longueurItineraire(itineraire[numeroMeilleurItineraire]);
    }
*/

    // Copier l'itinéraire dans le profil de l'IA
    if(numeroMeilleurItineraire != -1)
        copierItineraire(itineraire[numeroMeilleurItineraire], jeu->listeDesJoueurs[1].itineraireSuivi, longueurMeilleurItineraire);
    if(longueurMeilleurItineraire != -1)
        modifierCoordonnees(&jeu->listeDesJoueurs[1].itineraireSuivi[longueurMeilleurItineraire], -1, -1);
}

int comparerItineraireEloignementDangerosite(Coordonnes itineraire[300], Coordonnes casesDangereuses[148], int *longueurMeilleurItineraire)
{
    int resultat = 0;
    int nbCasesDangereuses = 0;

    // Déterminer le nombre de cases dangereuses à traverser avant d'atteindre une non dangereuse
    while(coordonneesDangereuses(itineraire[nbCasesDangereuses].x, itineraire[nbCasesDangereuses].y, casesDangereuses)
          && nbCasesDangereuses != longueurItineraire(itineraire))
        nbCasesDangereuses++;

    // Si l'itinéraire comporte au moins une case non dangereuse
    if(nbCasesDangereuses != longueurItineraire(itineraire))
    {
        // Si l'itinéraire est celui qui nécessite un nombre minimum de déplacements sur des cases dangereuses
        if(nbCasesDangereuses < *longueurMeilleurItineraire - 1 || *longueurMeilleurItineraire == -1)
        {
            *longueurMeilleurItineraire = nbCasesDangereuses + 1;
            resultat = 1;
        }
    }

    return resultat;
}

int comparerItineraireRapprochementTarget(int x, int y, Coordonnes itineraire[300], Coordonnes casesDangereuses[148], int *longueurMeilleurItineraire)
{
    int resultat = 0;
    int nbDeplacement = 0, nbDeplacementDansAxe = 0, nbDeplacementHorsAxe = 0;

    // Si l'itinéraire ne comprend pas de cases dangereuses
    if(!itineraireDangereux(itineraire, casesDangereuses))
    {
        // Si un itineraire comprend le target
        if(rechercherItineraire(x, y, itineraire))
        {
            // Calculer le nombre de déplacements nécessaires pour accéder à la position du target
            while(itineraire[nbDeplacement].x != x || itineraire[nbDeplacement].y != y)
                nbDeplacement++;
            nbDeplacement++;

            // Déterminer le nombre de déplacements dans l'axe du target
            if(itineraire[nbDeplacement - 1].x == itineraire[nbDeplacement - 2].x)
            {
                while(itineraire[nbDeplacement - nbDeplacementDansAxe - 1].x == x && nbDeplacementDansAxe != nbDeplacement)
                    nbDeplacementDansAxe++;
            }
            else if(itineraire[nbDeplacement - 1].y == itineraire[nbDeplacement - 2].y)
            {
                while(itineraire[nbDeplacement - nbDeplacementDansAxe - 1].y == y && nbDeplacementDansAxe != nbDeplacement)
                    nbDeplacementDansAxe++;
            }

            // Déterminer le nombre de déplacements hors de l'axe du target
            nbDeplacementHorsAxe = nbDeplacement - nbDeplacementDansAxe;

            // S'il s'agit de l'itineraire le plus rapide
            if(*longueurMeilleurItineraire == -1 || nbDeplacementDansAxe > *longueurMeilleurItineraire)
            {
                *longueurMeilleurItineraire = nbDeplacementHorsAxe + 1;
                resultat = 1;
            }
        }
    }

    return resultat;
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

void copierItineraire(Coordonnes itineraireSrc[300], Coordonnes itineraireDest[300], int longueurACopier)
{
    for(int i=0; i<longueurACopier; i++)
        copierCoordonnees(&itineraireSrc[i], &itineraireDest[i]);

    modifierCoordonnees(&itineraireDest[longueurACopier], -1, -1);
}


/********** Traitement des dangers **********/

void calculerCasesDangereuses(Coordonnes casesDangereuses[148], StructJeu *jeu)
{
    int nbCasesDangereusesTrouvees = 0;
    int nbBombesPosees;
    int longueurExplosion = 10;
    int X, Y;
    int hautDestructible, droiteDestructible, basDestructible, gaucheDestructible;

    // Repérer les bombes posées
     for(int i=0; i<jeu->nbrDeJoueurs; i++)
     {
         // Si une bombe posée est détectée
         if(jeu->listeDesJoueurs[i].bombe.etatBombe >= 1)
         {
            modifierCoordonnees(&casesDangereuses[nbCasesDangereusesTrouvees], jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x, jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y);
            nbCasesDangereusesTrouvees++;
         }
     }
     nbBombesPosees = nbCasesDangereusesTrouvees;

     // Repérer les cases qui vont exploser
     for(int i=0; i<nbBombesPosees; i++)
     {
        X = casesDangereuses[i].x;
        Y = casesDangereuses[i].y;

        hautDestructible = 1;
        droiteDestructible = 1;
        basDestructible = 1;
        gaucheDestructible = 1;

        for(int cmpt=0; cmpt<longueurExplosion; cmpt++)
        {
            // Déterminer les directions dans lesquelles il ne doit plus avoir d'explosion
            if(Y-cmpt < 0 || jeu->mapJeu[X][Y-cmpt] == 1)
                hautDestructible = 0;
            if(X+cmpt >= 20 || jeu->mapJeu[X+cmpt][Y] == 1)
                droiteDestructible = 0;
            if(Y+cmpt >= 20 || jeu->mapJeu[X][Y+cmpt] == 1)
                basDestructible = 0;
            if(X-cmpt < 0 || jeu->mapJeu[X-cmpt][Y] == 1)
                gaucheDestructible = 0;

            // Exploser dans les directions autorisées
            if(hautDestructible == 1)
            {
                modifierCoordonnees(&casesDangereuses[nbCasesDangereusesTrouvees], X, Y-cmpt);
                nbCasesDangereusesTrouvees++;
            }
            if(droiteDestructible == 1)
            {
                modifierCoordonnees(&casesDangereuses[nbCasesDangereusesTrouvees], X+cmpt, Y);
                nbCasesDangereusesTrouvees++;
            }
            if(basDestructible == 1)
            {
                modifierCoordonnees(&casesDangereuses[nbCasesDangereusesTrouvees], X, Y+cmpt);
                nbCasesDangereusesTrouvees++;
            }
            if(gaucheDestructible == 1)
            {
                modifierCoordonnees(&casesDangereuses[nbCasesDangereusesTrouvees], X-cmpt, Y);
                nbCasesDangereusesTrouvees++;
            }
        }
     }

     modifierCoordonnees(&casesDangereuses[nbCasesDangereusesTrouvees], -1, -1);
}

int coordonneesDangereuses(int x, int y, Coordonnes casesDangereuses[148])
{
    int i = 0;
    int resultat = 0;

    while(casesDangereuses[i].x != -1 || casesDangereuses[i].y != -1)
    {
        if(renvoitCaseMatrice(x) == casesDangereuses[i].x && renvoitCaseMatrice(y) == casesDangereuses[i].y)
            resultat = 1;
        i++;
    }

    return resultat;
}

int itineraireDangereux(Coordonnes itineraire[300], Coordonnes casesDangereuses[148])
{
    int resultat = 0;

    for(int i=0; i<longueurItineraire(itineraire); i++)
    {
        if(coordonneesDangereuses(itineraire[i].x, itineraire[i].y, casesDangereuses))
            resultat = 1;
    }

    return resultat;
}


/********** Traitement des coordonnées **********/

void modifierCoordonnees(Coordonnes *coordonneesAModifier, int x, int y)
{
    coordonneesAModifier->x = x;
    coordonneesAModifier->y = y;
}

void copierCoordonnees(Coordonnes *coordonneesSrc, Coordonnes *coordonneesDest)
{
    coordonneesDest->x = coordonneesSrc->x;
    coordonneesDest->y = coordonneesSrc->y;
}
