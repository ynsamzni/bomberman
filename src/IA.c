#include <stdio.h>
#include <stdlib.h>

#include "../include/IA.h"


void deplacerIA(int indiceJoueur, StructJeu *jeu)
{
    Coordonnes itineraire[300][300];
    Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES];
    int nbTotalItineraire;

    // Si l'IA ne se trouve pas au milieu d'un déplacement
    if(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x%30 == 0 && jeu->listeDesJoueurs[indiceJoueur].coordonnes.y%30 == 0)
    {
        // Calculer les cases dangereuses
        calculerCasesDangereuses(casesDangereuses, jeu);

        // Si un nouvel itinéraire doit être généré
        if(itineraireDangereux(jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi, casesDangereuses) || longueurItineraire(jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi) == 0)
        {
            // Calculer tous les itinéraires disponibles
            nbTotalItineraire = calculerItineraires(indiceJoueur, itineraire, 0, 0, jeu);

            // Déterminer l'itinéraire à prendre
            comparerItineraires(indiceJoueur, itineraire, nbTotalItineraire, jeu, casesDangereuses);
        }
    }
    // Réaliser un déplacement
    suivreItineraire(indiceJoueur, jeu);


    /***** Debug *****/

    //printf("\nPOSITION : %d/%d", jeu->listeDesJoueurs[1].coordonnes.x, jeu->listeDesJoueurs[1].coordonnes.y);
/*
    printf("\nItineraire :");
    for(int j=0; j< longueurItineraire(jeu->listeDesJoueurs[1].itineraireSuivi); j++)
        printf(" %d/%d", jeu->listeDesJoueurs[1].itineraireSuivi[j].x, jeu->listeDesJoueurs[1].itineraireSuivi[j].y);
*/
/*
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
*/
/*
    int i = 0;
    while(casesDangereuses[i].x != -1 || casesDangereuses[i].y != -1)
    {
        printf("\nBombe %d/%d", casesDangereuses[i].x, casesDangereuses[i].y);
        i++;
    }
*/
    //printf("\n---------\n");
}


/**************************************************************************************/
/***************************** TRAITEMENT DES ITINERAIRES *****************************/
/**************************************************************************************/

int calculerItineraires(int indiceJoueur, Coordonnes itineraire[300][300], int nbItineraire, int nbDeplacement, StructJeu *jeu)
{
    int nbDeplacementIterationActuelle = 0;
    int x, y, xInitial, yInitial;
    Direction direction = HAUT;
    Coordonnes nvllePosition;
    int cmptItinerairesDepartIdentique;

    int nbMaxItinerairesDepartIdentique = 20;
    int nbMaxDeplacements = 20;

    // Déterminer la position initiale
    xInitial = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x;
    yInitial = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y;

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

    // Stop si cela fait dépasser le nombre max de déplacements autorisés dans un même itinéraire
    if(nbDeplacement > nbMaxDeplacements)
        return nbItineraire;

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
           && (nbDeplacement == 0 || !rechercherDeplacementItineraire(nvllePosition.x, nvllePosition.y, itineraire[nbItineraire])) && (nvllePosition.x != xInitial || nvllePosition.y != yInitial)) // Si on n'est pas déjà passé par cette case
        {
           // Passer à un nouvel itineraire si un déplacement a déjà été effectué dans cette itération
            if(nbDeplacementIterationActuelle != 0)
            {
                // Vérifier si la création d'un nouvel itinéraire ne fait pas dépasser le nombre max d'itinéraires autorisés avec un même point de départ
                cmptItinerairesDepartIdentique = 0;
                for(int i=0; i<nbItineraire; i++)
                {
                    if(itineraire[i][0].x == itineraire[nbItineraire][0].x && itineraire[i][0].y == itineraire[nbItineraire][0].y)
                        cmptItinerairesDepartIdentique++;
                }

                // Si la création d'un nouvel itinéraire est autorisé
                if(cmptItinerairesDepartIdentique < nbMaxItinerairesDepartIdentique)
                {
                    nbItineraire++;
                    for(int i=0; i<=nbDeplacement; i++)
                    {
                        itineraire[nbItineraire][i].x = itineraire[nbItineraire-1][i].x;
                        itineraire[nbItineraire][i].y = itineraire[nbItineraire-1][i].y;
                    }
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

void suivreItineraire(int indiceJoueur, StructJeu *jeu)
{
    // Si le joueur doit terminer un déplacement
    if(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x%30 != 0 || jeu->listeDesJoueurs[indiceJoueur].coordonnes.y%30 != 0)
    {
        // Poursuivre le déplacement du joueur
        switch(jeu->listeDesJoueurs[indiceJoueur].direction)
        {
            case HAUT:
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y -= VITESSE_DES_JOUEURS;
                break;
            case DROITE:
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x += VITESSE_DES_JOUEURS;
                break;
            case BAS:
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.y += VITESSE_DES_JOUEURS;
                break;
            case GAUCHE:
                jeu->listeDesJoueurs[indiceJoueur].coordonnes.x -= VITESSE_DES_JOUEURS;
                break;
        }

        // Si on atteint une position de l'itinéraire
        if(jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi[0].x == jeu->listeDesJoueurs[indiceJoueur].coordonnes.x
           && jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi[0].y == jeu->listeDesJoueurs[indiceJoueur].coordonnes.y)
        {
            // Ne plus marquer le joueur comme étant en déplacement s'il a atteint la dernière position de l'itinéraire
            if(longueurItineraire(jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi) == 1)
                jeu->listeDesJoueurs[indiceJoueur].deplacement = 0;

            // Supprimer la position de l'itinéraire
            supprimerDeplacementItineraire(0, jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi);
        }
    }
    // Si le joueur doit commencer un nouveau déplacement
    else if(longueurItineraire(jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi) != 0)
    {
        // Marquer le joueur comme étant en déplacement
        jeu->listeDesJoueurs[indiceJoueur].deplacement = 1;

        // Débuter le prochain déplacement de l'itinéraire
        if(jeu->listeDesJoueurs[indiceJoueur].coordonnes.y + 30 == jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi[0].y)
        {
            jeu->listeDesJoueurs[indiceJoueur].direction = BAS;
            jeu->listeDesJoueurs[indiceJoueur].coordonnes.y += VITESSE_DES_JOUEURS;
        }
        else if(jeu->listeDesJoueurs[indiceJoueur].coordonnes.y - 30 == jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi[0].y)
        {
            jeu->listeDesJoueurs[indiceJoueur].direction = HAUT;
            jeu->listeDesJoueurs[indiceJoueur].coordonnes.y -= VITESSE_DES_JOUEURS;
        }
        else if(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x + 30 == jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi[0].x)
        {
            jeu->listeDesJoueurs[indiceJoueur].direction = DROITE;
            jeu->listeDesJoueurs[indiceJoueur].coordonnes.x += VITESSE_DES_JOUEURS;
        }
        else if(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x - 30 == jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi[0].x)
        {
            jeu->listeDesJoueurs[indiceJoueur].direction = GAUCHE;
            jeu->listeDesJoueurs[indiceJoueur].coordonnes.x -= VITESSE_DES_JOUEURS;
        }
    }
}

void comparerItineraires(int indiceJoueur, Coordonnes itineraire[300][300], int nbTotalItineraire, StructJeu *jeu, Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES])
{
    int numeroMeilleurItineraire = -1;
    int longueurMeilleurItineraire = -1;

    // Coordonnées de l'ennemi
    int x = jeu->listeDesJoueurs[0].coordonnes.x;
    int y = jeu->listeDesJoueurs[0].coordonnes.y;

    // Si l'IA se trouve sur une case dangereuse : Trouver l'itinéraire qui mène le plus rapidement à une case non dangereuse
    if(coordonneesDangereuses(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x, jeu->listeDesJoueurs[indiceJoueur].coordonnes.y, casesDangereuses))
    {
        // Analyser l'itinéraire en cours
        comparerItineraireEloignementDangerosite(jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi, casesDangereuses, &longueurMeilleurItineraire);

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
        comparerItineraireRapprochementTarget(x, y, jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi, casesDangereuses, &longueurMeilleurItineraire);

        // Analyser l'ensemble des itinéraires calculés
        for(int i = 0; i < nbTotalItineraire; i++)
        {
            if(comparerItineraireRapprochementTarget(x, y, itineraire[i], casesDangereuses, &longueurMeilleurItineraire))
                numeroMeilleurItineraire = i;
        }
    }

    // Si l'IA n'a aucun itinéraire à suivre : Sélectionner aléatoirement un itinéraire non dangereux
    if(numeroMeilleurItineraire == -1 && longueurMeilleurItineraire == -1 && nbTotalItineraire != 0)
    {
        // Déterminer si au moins un itinéraire non dangereux est disponible
        int itineraireNonDangereuxDisponible = 0, i = 0;
        while(i < nbTotalItineraire && !itineraireNonDangereuxDisponible)
        {
            if(!itineraireDangereux(itineraire[i], casesDangereuses))
                itineraireNonDangereuxDisponible = 1;
            i++;
        }

        // Si au moins un itinéraire non dangereux est disponible
        if(itineraireNonDangereuxDisponible)
        {
            // Choisir un itinéraire non dangereux au hasard
            do
            {
                numeroMeilleurItineraire = rand() % nbTotalItineraire;
            }
            while(itineraireDangereux(itineraire[numeroMeilleurItineraire], casesDangereuses));
            longueurMeilleurItineraire = longueurItineraire(itineraire[numeroMeilleurItineraire]);
        }

        // Marquer l'IA comme étant en mode aléatoire
        jeu->listeDesJoueurs[indiceJoueur].modeAleatoire = 1;
    }
    else
        // Ne plus marquer l'IA comme étant en mode aléatoire
        jeu->listeDesJoueurs[indiceJoueur].modeAleatoire = 0;

    // Copier le meilleur itinéraire trouvé dans le profil de l'IA
    if(numeroMeilleurItineraire != -1)
        copierItineraire(itineraire[numeroMeilleurItineraire], jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi);
    if(longueurMeilleurItineraire != -1)
        modifierCoordonnees(&jeu->listeDesJoueurs[indiceJoueur].itineraireSuivi[longueurMeilleurItineraire], -1, -1);
}

int comparerItineraireEloignementDangerosite(Coordonnes itineraire[300], Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES], int *longueurMeilleurItineraire)
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

int comparerItineraireRapprochementTarget(int x, int y, Coordonnes itineraire[300], Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES], int *longueurMeilleurItineraire)
{
    int resultat = 0;
    int nbDeplacement = 0, nbDeplacementDansAxe = 0, nbDeplacementHorsAxe = 0;
    int longueurItineraire;

    // Si l'itinéraire ne comprend pas de cases dangereuses
    if(!itineraireDangereux(itineraire, casesDangereuses))
    {
        // Si un itineraire comprend le target
        if(rechercherDeplacementItineraire(x, y, itineraire))
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

            // Déterminer la longueur de l'itinéraire permettant de se rapprocher assez pour poser une bombe
            if(nbDeplacementDansAxe < LONGUEUR_EXPLOSION_BOMBE)
                longueurItineraire = nbDeplacementHorsAxe + 1;
            else
                longueurItineraire = nbDeplacementHorsAxe + 1 + (nbDeplacementDansAxe - LONGUEUR_EXPLOSION_BOMBE);

            // S'il s'agit de l'itineraire le plus rapide
            if(*longueurMeilleurItineraire == -1 || longueurItineraire < *longueurMeilleurItineraire)
            {
                *longueurMeilleurItineraire = longueurItineraire;
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

void copierItineraire(Coordonnes itineraireSrc[300], Coordonnes itineraireDest[300])
{
    for(int i=0; i<longueurItineraire(itineraireSrc); i++)
        copierCoordonnees(&itineraireSrc[i], &itineraireDest[i]);
}

int rechercherDeplacementItineraire(int x, int y, Coordonnes itineraire[300])
{
    int resultat = 0;
    int i = 0;

    while(i < longueurItineraire(itineraire) && resultat == 0)
    {
        if(itineraire[i].x == x && itineraire[i].y == y)
            resultat = 1;
        i++;
    }

    return resultat;
}

void supprimerDeplacementItineraire(int deplacementASupprimer, Coordonnes itineraire[300])
{
    for(int i=0; i<longueurItineraire(itineraire); i++)
    {
        if(i >= deplacementASupprimer)
            copierCoordonnees(&itineraire[i + 1], &itineraire[i]);
    }
    modifierCoordonnees(&itineraire[longueurItineraire(itineraire)], -1, -1);
}


/**********************************************************************************/
/***************************** TRAITEMENT DES DANGERS *****************************/
/**********************************************************************************/

void calculerCasesDangereuses(Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES], StructJeu *jeu)
{
    int nbCasesDangereusesTrouvees = 0;
    int nbBombesPosees;
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

        for(int cmpt=1; cmpt<=LONGUEUR_EXPLOSION_BOMBE; cmpt++)
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

int poseBombeDangereuse(int indiceJoueur, StructJeu *jeu)
{
    int resultat = 1;
    int x, y;
    int xInitial, yInitial;
    Direction direction = HAUT;

    // Déterminer les coordonnées du joueur
    xInitial = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x;
    yInitial = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y;

    while(xInitial%30 != 0)
    {
        if(jeu->listeDesJoueurs[indiceJoueur].direction == DROITE)
            xInitial += VITESSE_DES_JOUEURS;
        else
            xInitial -= VITESSE_DES_JOUEURS;
    }
    while(yInitial%30 != 0)
    {
        if(jeu->listeDesJoueurs[indiceJoueur].direction == HAUT)
            yInitial -= VITESSE_DES_JOUEURS;
        else
            yInitial += VITESSE_DES_JOUEURS;
    }

    // Tester les 4 directions
    for(int i=0; i<4; i++)
    {
        x = xInitial;
        y = yInitial;

        // Tester l'ensemble des déplacements possibles dans la direction en cours
        while(deplacementPossible(x, y, direction + i, jeu))
        {
            switch(direction + i)
            {
                case HAUT:
                    // Simuler le déplacement
                    y -= 30;

                    // Déterminer s'il est possible d'échapper au rayon d'action de la bombe
                    if(deplacementPossible(x, y, DROITE, jeu) || deplacementPossible(x, y, GAUCHE, jeu))
                        resultat = 0;
                    break;
                case DROITE:
                    // Simuler le déplacement
                    x += 30;

                    // Déterminer s'il est possible d'échapper au rayon d'action de la bombe
                    if(deplacementPossible(x, y, HAUT, jeu) || deplacementPossible(x, y, BAS, jeu))
                        resultat = 0;
                    break;
                case BAS:
                    // Simuler le déplacement
                    y += 30;

                    // Déterminer s'il est possible d'échapper au rayon d'action de la bombe
                    if(deplacementPossible(x, y, DROITE, jeu) || deplacementPossible(x, y, GAUCHE, jeu))
                        resultat = 0;
                    break;
                case GAUCHE:
                    // Simuler le déplacement
                    x -= 30;

                    // Déterminer s'il est possible d'échapper au rayon d'action de la bombe
                    if(deplacementPossible(x, y, HAUT, jeu) || deplacementPossible(x, y, BAS, jeu))
                        resultat = 0;
                    break;
            }
        }
    }

    return resultat;
}

int coordonneesDangereuses(int x, int y, Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES])
{
    int i = 0;
    int resultat = 0;

    while(casesDangereuses[i].x != -1 || casesDangereuses[i].y != -1)
    {
        if(coordonneeMatricielle(x) == casesDangereuses[i].x && coordonneeMatricielle(y) == casesDangereuses[i].y)
            resultat = 1;
        i++;
    }

    return resultat;
}

int itineraireDangereux(Coordonnes itineraire[300], Coordonnes casesDangereuses[NB_CASES_AFFECTEES_EXPLOSIONS_BOMBES])
{
    int resultat = 0;

    for(int i=0; i<longueurItineraire(itineraire); i++)
    {
        if(coordonneesDangereuses(itineraire[i].x, itineraire[i].y, casesDangereuses))
            resultat = 1;
    }

    return resultat;
}

int ennemiProche(int indiceJoueur, int distanceMax, StructJeu *jeu)
{
    int resultat = 0;

    int xJoueur = jeu->listeDesJoueurs[indiceJoueur].coordonnes.x;
    int yJoueur = jeu->listeDesJoueurs[indiceJoueur].coordonnes.y;

    int xEnnemi;
    int yEnnemi;

    int xSimulee;
    int ySimulee;

    int distance;
    Direction direction;

    int indiceEnnemi = 0;
    while(indiceEnnemi < jeu->nbrDeJoueurs && resultat == 0)
    {
        // S'il s'agit de l'indice d'un ennemi
        if(indiceEnnemi != indiceJoueur)
        {
            xEnnemi = jeu->listeDesJoueurs[indiceEnnemi].coordonnes.x;
            yEnnemi = jeu->listeDesJoueurs[indiceEnnemi].coordonnes.y;

            // Si un ennemi se trouve sur le même axe
            if(xEnnemi == xJoueur || yEnnemi == yJoueur)
            {
                // Déterminer la distance qui sépare le joueur de l'ennemi
                if(xEnnemi == xJoueur)
                {
                    if(yEnnemi > yJoueur)
                    {
                        distance = coordonneeMatricielle(yEnnemi) - coordonneeMatricielle(yJoueur);
                        direction = BAS;
                    }
                    else
                    {
                        distance = coordonneeMatricielle(yJoueur) - coordonneeMatricielle(yEnnemi);
                        direction = HAUT;
                    }
                }
                else if(yEnnemi == yJoueur)
                {
                    if(xEnnemi > xJoueur)
                    {
                        distance = coordonneeMatricielle(xEnnemi) - coordonneeMatricielle(xJoueur);
                        direction = DROITE;
                    }
                    else
                    {
                        distance = coordonneeMatricielle(xJoueur) - coordonneeMatricielle(xEnnemi);
                        direction = GAUCHE;
                    }
                }

                // Si l'ennemi est proche
                if(distance <= distanceMax)
                {
                    // Déterminer si aucun obstacle ne se trouve entre le joueur et l'ennemi
                    xSimulee = xJoueur;
                    ySimulee = yJoueur;

                    while(contenuCoordonnees(jeu, xSimulee, ySimulee) == 0 && resultat == 0)
                    {
                        if(xSimulee == xEnnemi && ySimulee == yEnnemi)
                            resultat = 1;

                        switch(direction)
                        {
                            case HAUT:
                                ySimulee -= VITESSE_DES_JOUEURS;
                                break;
                            case BAS:
                                ySimulee += VITESSE_DES_JOUEURS;
                                break;
                            case DROITE:
                                xSimulee += VITESSE_DES_JOUEURS;
                                break;
                            case GAUCHE:
                                xSimulee -= VITESSE_DES_JOUEURS;
                                break;
                        }
                    }
                }
            }
        }
        indiceEnnemi++;
    }

    return resultat;
}


/**************************************************************************************/
/***************************** TRAITEMENT DES COORDONNEES *****************************/
/**************************************************************************************/

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
