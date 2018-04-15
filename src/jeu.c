#include <stdio.h>
#include <stdlib.h>

#include "../include/jeu.h"
#include "../include/IA.h"
#include "../include/joueurHumain.h"


void initMap(StructJeu *jeu)
{
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
    }
}

void initJoueurs(StructJeu *jeu)
{
    // Parcourir tous les joueurs
    for(int i = 0; i < jeu->nbrDeJoueurs; i++)
    {
        // Initialiser les données de santé
        jeu->listeDesJoueurs[i].enVie = 1;

        // Initialiser les données de position
        switch(i)
        {
        case 0:
            jeu->listeDesJoueurs[i].coordonnes.x = 0;
            jeu->listeDesJoueurs[i].coordonnes.y = 0;
            break;
        case 1:
            jeu->listeDesJoueurs[i].coordonnes.x = WIDTH - 30;
            jeu->listeDesJoueurs[i].coordonnes.y = HEIGHT - 30;
            break;
        case 2:
            jeu->listeDesJoueurs[i].coordonnes.x = WIDTH - 30;
            jeu->listeDesJoueurs[i].coordonnes.y = 0;
            break;
        case 3:
            jeu->listeDesJoueurs[i].coordonnes.x = 0;
            jeu->listeDesJoueurs[i].coordonnes.y = HEIGHT - 30;
            break;
        }

        // Initialiser les données de déplacement
        jeu->listeDesJoueurs[i].direction = BAS;
        jeu->listeDesJoueurs[i].deplacement = 0;
        jeu->listeDesJoueurs[i].itineraireSuivi[0].x = -1;
        jeu->listeDesJoueurs[i].itineraireSuivi[0].y = -1;

        // Initialiser les données de la bombe associée
        jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x = 0;
        jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y = 0;
        jeu->listeDesJoueurs[i].bombe.tickDePose = 0;
        jeu->listeDesJoueurs[i].bombe.etatBombe = 0;
    }
}

void initJeu(StructJeu *jeu)
{
    // Initialiser la map
    initMap(jeu);

    // Initialiser les joueurs
    initJoueurs(jeu);

    // Initialiser l'animation de victoire / défaite qui apparaît en fin de partie
    jeu->animation.victoire = 0;
    jeu->animation.defaite = 0;
}

void calculerJeu(StructJeu *jeu, StructClavier *clavier, StructAudio *audio)
{
    // Déterminer si des joueurs ont des actions en attente d'exécution
    for(int i = 0; i < jeu->nbrDeJoueurs; i++)
    {
        if(jeu->listeDesJoueurs[i].enVie)
        {
            // Si humain
            if(jeu->listeDesJoueurs[i].humainOuIA == 0)
            {
                // Si J1
                if(i == 0)
                {
                    if(clavier->toucheBombeJ1)
                        poserBombe(jeu, i, audio);
                    deplacerJoueurJ1(clavier, jeu, i);
                }
                // Si J2
                else if(i == 1)
                {
                    if(clavier->toucheBombeJ2)
                        poserBombe(jeu, i, audio);
                    deplacerJoueurJ2(clavier, jeu, i);
                }
            }
            // Si IA
            else
            {
                deplacerIA(i, jeu);

                if(!poseBombeDangereuse(i, jeu) // Si une bombe peut être posée avec possibilité de s'en échapper
                   && (ennemiProche(i, LONGUEUR_EXPLOSION_BOMBE, jeu) // Si ennemi proche
                   || (jeu->listeDesJoueurs[i].modeAleatoire && rand() % 15 == 5))) // Si effectue des actions de manière aléatoire
                    poserBombe(jeu, i, audio);
            }
        }

        // Si humain ou IA
        exploserBombe(jeu, i, audio);
    }

    // Déterminer si le jeu doit changer d'état
    actualiserEtatJeu(jeu, audio, clavier);
}


/*********************************************************************************************/
/***************************** GESTION DE L'EXPLOSION DES BOMBES *****************************/
/*********************************************************************************************/

void poserBombe(StructJeu *jeu, int indiceJoueur, StructAudio *audio)
{
    int caseBombeX, caseBombeY;

    // Si le joueur n'a aucune bombe de posée à son actif
    if(jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe == 0)
    {
        // Déterminer la case sur laquelle va être posée la bombe
        caseBombeX = coordonneeMatricielle(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x);
        caseBombeY = coordonneeMatricielle(jeu->listeDesJoueurs[indiceJoueur].coordonnes.y);

        if(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x%30 != 0 || jeu->listeDesJoueurs[indiceJoueur].coordonnes.y%30 != 0)
        {
            if(jeu->listeDesJoueurs[indiceJoueur].direction == DROITE)
                caseBombeX++;
            else if(jeu->listeDesJoueurs[indiceJoueur].direction == BAS)
                caseBombeY++;
        }

        // Poser la bombe
        jeu->mapJeu[caseBombeX][caseBombeY] = 3;
        lireAudio(audio, SON_POSE_BOMBE);

        // Démarrer le timer d'explosion de la bombe
        jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose = SDL_GetTicks();

        // Lier la bombe au joueur qui l'a posé
        jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.x = caseBombeX;
        jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.y = caseBombeY;
        jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe = 1;
    }
}

void exploserBombe(StructJeu *jeu, int indiceJoueur, StructAudio *audio)
{
    int X = jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.x;
    int Y = jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.y;
    int hautExplosion, droiteExplosion, basExplosion, gaucheExplosion;

    // Si une bombe doit exploser
    if(jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe == 1 && SDL_GetTicks() - jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose > 1000)
    {
        // Supposer que toutes les directions sont destructibles
        hautExplosion=1;
        droiteExplosion=1;
        basExplosion=1;
        gaucheExplosion=1;

        // Parcourir toutes les cases qui sont susceptibles d'exploser
        for(int cmpt=0; cmpt<=LONGUEUR_EXPLOSION_BOMBE; cmpt++)
        {
            // Déterminer les directions dans lesquelles il ne doit plus avoir d'explosion
            if(Y-cmpt < 0 || jeu->mapJeu[X][Y-cmpt] == 1)
                hautExplosion=0;
            if(X+cmpt >= 20 || jeu->mapJeu[X+cmpt][Y] == 1)
                droiteExplosion=0;
            if(Y+cmpt >= 20 || jeu->mapJeu[X][Y+cmpt] == 1)
                basExplosion=0;
            if(X-cmpt < 0 || jeu->mapJeu[X-cmpt][Y] == 1)
                gaucheExplosion=0;

            // Exploser dans les directions autorisées
            if(hautExplosion == 1)
                jeu->mapJeu[X][Y-cmpt] = 4;
            if(droiteExplosion == 1)
                jeu->mapJeu[X+cmpt][Y] = 4;
            if(basExplosion == 1)
                jeu->mapJeu[X][Y+cmpt] = 4;
            if(gaucheExplosion == 1)
                jeu->mapJeu[X-cmpt][Y] = 4;
        }
        jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe = 2;
        lireAudio(audio, SON_EXPLOSION_BOMBE);

        // Tuer les joueurs qui se trouvent dans l'explosion de la bombe
        for(int indiceJoueur = 0; indiceJoueur < jeu->nbrDeJoueurs; indiceJoueur++)
        {
            if(contenuCoordonnees(jeu, jeu->listeDesJoueurs[indiceJoueur].coordonnes.x, jeu->listeDesJoueurs[indiceJoueur].coordonnes.y)  == 4 && jeu->listeDesJoueurs[indiceJoueur].enVie != 0)
            {
                jeu->listeDesJoueurs[indiceJoueur].enVie = 0;
                lireAudio(audio, SON_MORT_PERSONNAGE);
            }
        }
    }

    // Si l'explosion d'une bombe doit prendre fin
    else if(jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe == 2 && SDL_GetTicks() - jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose > 1500)
    {
        // Supposer que toutes les directions sont en cours d'explosion
        hautExplosion=1;
        droiteExplosion=1;
        basExplosion=1;
        gaucheExplosion=1;

        // Parcourir toutes les cases qui sont susceptibles d'être en cours d'explosion
        for(int cmpt=0; cmpt<=LONGUEUR_EXPLOSION_BOMBE; cmpt++)
        {
            // Déterminer les directions dans lesquelles il n'y a pas d'explosion
            if(Y-cmpt < 0 || jeu->mapJeu[X][Y-cmpt] == 1)
                hautExplosion=0;
            if(X+cmpt >= 20 || jeu->mapJeu[X+cmpt][Y] == 1)
                droiteExplosion=0;
            if(Y+cmpt >= 20 || jeu->mapJeu[X][Y+cmpt] == 1)
                basExplosion=0;
            if(X-cmpt < 0 || jeu->mapJeu[X-cmpt][Y] == 1)
                gaucheExplosion=0;

            // Mettre fin aux explosions
            if(hautExplosion == 1)
                jeu->mapJeu[X][Y-cmpt] = 0;
            if(droiteExplosion == 1)
                jeu->mapJeu[X+cmpt][Y] = 0;
            if(basExplosion == 1)
                jeu->mapJeu[X][Y+cmpt] = 0;
            if(gaucheExplosion == 1)
                jeu->mapJeu[X-cmpt][Y] = 0;
        }
        jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe = 0;
        jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose = 0;
    }
}


/*********************************************************************************************/
/***************************** FONCTIONS POUR SIMPLIFIER LE CODE *****************************/
/*********************************************************************************************/

int deplacementPossible(int x, int y, Direction direction, StructJeu *jeu)
{
    if((direction == HAUT && contenuCoordonnees(jeu, x, y - 1) == 0 && contenuCoordonnees(jeu, x + 29, y - 1) == 0)
            || (direction == DROITE && contenuCoordonnees(jeu, x + 31, y) == 0 && contenuCoordonnees(jeu, x + 31, y + 29) == 0)
            || (direction == BAS && contenuCoordonnees(jeu, x, y + 31) == 0 && contenuCoordonnees(jeu, x + 29, y + 31)== 0)
            || (direction == GAUCHE && contenuCoordonnees(jeu, x - 1, y) == 0 && contenuCoordonnees(jeu, x - 1, y + 29) == 0))
        return 1;
    else
        return 0;
}

int contenuCoordonnees(StructJeu *jeu, int x, int y)
{
    if(x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
        return 1;
    else
        return jeu->mapJeu[coordonneeMatricielle(x)][coordonneeMatricielle(y)];
}

int coordonneeMatricielle(int coordonne)
{
    return (coordonne/30);
}

void actualiserEtatJeu(StructJeu *jeu, StructAudio *audio, StructClavier *clavier)
{
    int nbrIA = 0;
    int nbrHumains = 0;

    // Déterminer si le jeu doit être mis en EXTINCTION
    for(int i = 0; i < jeu->nbrDeJoueurs ; i ++)
    {
        if(jeu->listeDesJoueurs[i].humainOuIA == 0 && jeu->listeDesJoueurs[i].enVie)
            nbrHumains++;
        else if(jeu->listeDesJoueurs[i].humainOuIA == 1 && jeu->listeDesJoueurs[i].enVie)
            nbrIA++;
    }
    if(nbrHumains == 0)
    {
        jeu->animation.defaite = 1;
        lireAudio(audio, SON_DEFAITE);
        for(int i = 0; i < jeu->nbrDeJoueurs; i++)
        {
            if(jeu->listeDesJoueurs[i].humainOuIA == 0 && strcmp(jeu->listeDesJoueurs[i].compte.nom, "Test") != 0) //2eme condition pour empecher de créer un joueur dans le fichier des comptes. Améliorations à venir
                actualiserStatistiquesJoueur(jeu, i, 0);
        }
        jeu->etat = EXTINCTION;
    }
    else if(nbrHumains == 1 && nbrIA == 0)
    {
        jeu->animation.victoire = 1;
        lireAudio(audio, SON_VICTOIRE);
        for(int i = 0; i < jeu->nbrDeJoueurs; i++)
        {
            if(jeu->listeDesJoueurs[i].humainOuIA == 0 && jeu->listeDesJoueurs[i].enVie == 1 && strcmp(jeu->listeDesJoueurs[i].compte.nom, "Test") != 0)
                actualiserStatistiquesJoueur(jeu, i, 1);
            if(jeu->listeDesJoueurs[i].humainOuIA == 0 && jeu->listeDesJoueurs[i].enVie == 0 && strcmp(jeu->listeDesJoueurs[i].compte.nom, "Test") != 0)
                actualiserStatistiquesJoueur(jeu, i, 0);
        }
        jeu->etat = EXTINCTION;
    }

    // Déterminer si le jeu doit être mis en PAUSE
    if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
        jeu->etat = PAUSE;
}

void actualiserStatistiquesJoueur(StructJeu *jeu, int indiceJoueur, int victoireOuDefaite)
{
    CompteJoueur contenuFichier; // Pour stocker temporairement les comptes lus dans le fichier
    FILE* fic;

    fic = fopen(CHEMIN_D_ACCES_FICHIER_COMPTES_JOUEURS, "rb+");

    if(fic == NULL)
        printf("[LECTURE] Erreur d'accès au fichier ! \n");
    else
    {
        // Lire le fichier
        while(fread(&contenuFichier, sizeof(CompteJoueur), 1, fic) && !feof(fic))
        {
            // S'il s'agit du compte à modifier
            if(strcmp(jeu->listeDesJoueurs[indiceJoueur].compte.nom, contenuFichier.nom) == 0)
            {
                // Positionner le pointeur de fichier au début du contenu à écraser
                fseek(fic, -sizeof(CompteJoueur), SEEK_CUR);

                // Effectuer les modifications dans le fichier
                if(victoireOuDefaite == 1)
                    contenuFichier.nbrVictoires++;
                else if(victoireOuDefaite == 0)
                    contenuFichier.nbrDefaites++;
                fwrite(&contenuFichier, sizeof(CompteJoueur), 1, fic);

                // Positionner le pointeur de fichier à la fin du fichier (le contenu étant modifié, il est inutile de continuer)
                fseek(fic, 0, SEEK_END);
            }
        }
        fclose(fic);
    }
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


/*****************************************************************/
/***************************** DEBUG *****************************/
/*****************************************************************/

void debugAfficherInformationsPartie(StructJeu *jeu)
{
    printf("Nombre de joueurs = %d \n", jeu->nbrDeJoueurs);
    for(int i = 0; i < jeu->nbrDeJoueurs; i++)
    {


        printf("   Joueur %d \n", i+1);
        printf("   Type : %d \n", jeu->listeDesJoueurs[i].humainOuIA);
        printf("   En vie : %d \n", jeu->listeDesJoueurs[i].enVie);

        if(jeu->listeDesJoueurs[i].humainOuIA == 0)
        {
            printf("      Nom : %s ", jeu->listeDesJoueurs[i].compte.nom);
            printf("      Victoires : %d \n", jeu->listeDesJoueurs[i].compte.nbrVictoires);
            printf("      Défaites : %d \n", jeu->listeDesJoueurs[i].compte.nbrDefaites);

        }
        printf("   Coordonnée X : %d \n", jeu->listeDesJoueurs[i].coordonnes.x);
        printf("   Coordonnée Y : %d \n", jeu->listeDesJoueurs[i].coordonnes.y);
        if(jeu->listeDesJoueurs[i].direction == HAUT)
        {
            printf("   Direction : HAUT \n");
        }
        if(jeu->listeDesJoueurs[i].direction == BAS)
        {
            printf("   Direction : BAS \n");
        }
        if(jeu->listeDesJoueurs[i].direction == DROITE)
        {

            printf("   Direction : DROITE \n");
        }
        if(jeu->listeDesJoueurs[i].direction == GAUCHE)
        {
            printf("   Direction : GAUCHE \n");
        }
        printf("   Déplacement : %d \n",jeu->listeDesJoueurs[i].deplacement);
        printf("      Bombe \n");
        printf("      Coordonnée Bombe X : %d \n", jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x);
        printf("      Coordonnée Bombe Y : %d \n", jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y);
        printf("      Nbr Ticks : %d \n", jeu->listeDesJoueurs[i].bombe.tickDePose);
        printf("\n****************************************\n");
    }
}
