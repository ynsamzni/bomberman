#include <stdio.h>
#include <stdlib.h>

#include "../include/jeu.h"
#include "../include/IA.h"
#include "../include/joueurHumain.h"




void initMap(StructJeu *jeu)
{
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
}

void initTousLesJoueurs(StructJeu *jeu) //Initialisation par défault des joueurs qu'ils soient IA ou Humain
{
    for(int i = 0; i < jeu->nbrDeJoueurs; i++)
    {
        jeu->listeDesJoueurs[i].enVie = 1;
        jeu->listeDesJoueurs[i].coordonnes.x = 0;
        jeu->listeDesJoueurs[i].coordonnes.y = 0;
        jeu->listeDesJoueurs[i].direction = BAS;
        jeu->listeDesJoueurs[i].deplacement = 0;

        jeu->listeDesJoueurs[i].bombe.coordonnesBombe.x = 0;
        jeu->listeDesJoueurs[i].bombe.coordonnesBombe.y = 0;
        jeu->listeDesJoueurs[i].bombe.tickDePose = 0;
        jeu->listeDesJoueurs[i].bombe.etatBombe = 0;
    }
}

void initJeu(StructJeu *jeu)
{

    initMap(jeu);
    initTousLesJoueurs(jeu);
    initJoueursHumains(jeu);
    initIA(jeu);

    jeu->animations.victoire = 0;
    jeu->animations.defaite = 0;

}



void calculerJeu(StructJeu *jeu, StructTouchesClavier *clavier, StructAudio *audio)
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

    // Autres
    tuerJoueur(jeu, audio);
    checkVictoire(jeu, audio);

    if(clavier->toucheArriere == 1)
        jeu->etat = EXTINCTION;
}


/***************************************************************************************/
/*******************GESTION DE L'EXPLOSION DES BOMBES***********************************/
/***************************************************************************************/

void poserBombe(StructJeu *jeu, int indiceJoueur, StructAudio *audio)
{
    int caseBombeX, caseBombeY;

    // Si le joueur n'a aucune bombe de posée à son actif
    if(jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe == 0)
    {
        // Déterminer la case sur laquelle va être posée la bombe
        caseBombeX = renvoitCaseMatrice(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x);
        caseBombeY = renvoitCaseMatrice(jeu->listeDesJoueurs[indiceJoueur].coordonnes.y);


        if(jeu->listeDesJoueurs[indiceJoueur].coordonnes.x%30 != 0 || jeu->listeDesJoueurs[indiceJoueur].coordonnes.y%30 != 0)
        {
            switch(jeu->listeDesJoueurs[indiceJoueur].direction)
            {
                case DROITE:
                    caseBombeX++;
                    break;
                case BAS:
                    caseBombeY++;
                    break;
            }
        }

        // Poser la bombe
        jeu->mapJeu[caseBombeX][caseBombeY] = 3;
        lireUnSon(audio, SON_POSE_BOMBE);

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
    if( (SDL_GetTicks() - jeu->listeDesJoueurs[indiceJoueur].bombe.tickDePose > 1000) &&  (jeu->listeDesJoueurs[indiceJoueur].bombe.etatBombe == 1) )
    {
        int X=jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.x;
        int Y=jeu->listeDesJoueurs[indiceJoueur].bombe.coordonnesBombe.y;

        int hautDestructible=1, droiteDestructible=1, basDestructible=1, gaucheDestructible=1;

        lireUnSon(audio, SON_EXPLOSION_BOMBE);

        for(int cmpt=0; cmpt<=LONGUEUR_EXPLOSION_BOMBE; cmpt++)
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
/**********************AFFICHER LA STRUCTURE**************************************/
/*********************************************************************************/


void afficherStructureJeu(StructJeu jeu)
{


    printf("Nombre de joueurs = %d \n", jeu.nbrDeJoueurs);
    for(int i = 0; i < jeu.nbrDeJoueurs; i++)
    {


        printf("   Joueur %d \n", i+1);
        printf("   Type : %d \n", jeu.listeDesJoueurs[i].humainOuIA);
        printf("   En vie : %d \n", jeu.listeDesJoueurs[i].enVie);

        if(jeu.listeDesJoueurs[i].humainOuIA == 0)
        {
            printf("      Nom : %s ", jeu.listeDesJoueurs[i].compte.nom);
            printf("      Victoires : %d \n", jeu.listeDesJoueurs[i].compte.nbrVictoires);
            printf("      Défaites : %d \n", jeu.listeDesJoueurs[i].compte.nbrDefaites);

        }
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



void tuerJoueur(StructJeu *jeu, StructAudio *audio)
{
    for(int indiceJoueur = 0; indiceJoueur < jeu->nbrDeJoueurs; indiceJoueur++)
    {
        if(contenuCoordonnees(jeu, jeu->listeDesJoueurs[indiceJoueur].coordonnes.x, jeu->listeDesJoueurs[indiceJoueur].coordonnes.y)  == 4 && jeu->listeDesJoueurs[indiceJoueur].enVie != 0 )
        {
            jeu->listeDesJoueurs[indiceJoueur].enVie = 0;
            lireUnSon(audio, SON_MORT_PERSONNAGE);
        }
    }

}

void checkVictoire(StructJeu *jeu, StructAudio *audio)
{
    int nbrIA = 0; //pour une raison qui m'échappe, la syntaxe int nbrIA, nbrHumains = 0; me renvoyait n'importe quoi comme valeur pour les variables
    int nbrHumains = 0;

    for(int i = 0; i < jeu->nbrDeJoueurs ; i ++)
    {
        if(jeu->listeDesJoueurs[i].humainOuIA == 0 && jeu->listeDesJoueurs[i].enVie == 1)
            nbrHumains++;
        if(jeu->listeDesJoueurs[i].humainOuIA == 1 && jeu->listeDesJoueurs[i].enVie == 1)
            nbrIA++;
    }

    if(nbrHumains == 0)
    {
        jeu->animations.defaite = 1;
        lireUnSon(audio, SON_DEFAITE);
        for(int i = 0; i < jeu->nbrDeJoueurs; i++)
        {
            if(jeu->listeDesJoueurs[i].humainOuIA == 0 && strcmp(jeu->listeDesJoueurs[i].compte.nom, "Test") != 0) //2eme condition pour empecher de créer un joueur dans le fichier des comptes. Améliorations à venir
                joueurNbrVictoireOuDefaitePlusUn(jeu, i, 0);
        }
        jeu->etat = EXTINCTION;
    }
    else if(nbrHumains == 1 && nbrIA == 0)
    {
        jeu->animations.victoire = 1;
        lireUnSon(audio, SON_VICTOIRE);
        for(int i = 0; i < jeu->nbrDeJoueurs; i++)
        {
            if(jeu->listeDesJoueurs[i].humainOuIA == 0 && jeu->listeDesJoueurs[i].enVie == 1 && strcmp(jeu->listeDesJoueurs[i].compte.nom, "Test") != 0)
                joueurNbrVictoireOuDefaitePlusUn(jeu, i, 1);
            if(jeu->listeDesJoueurs[i].humainOuIA == 0 && jeu->listeDesJoueurs[i].enVie == 0 && strcmp(jeu->listeDesJoueurs[i].compte.nom, "Test") != 0)
                joueurNbrVictoireOuDefaitePlusUn(jeu, i, 0);
        }
        jeu->etat = EXTINCTION;
    }


}

void joueurNbrVictoireOuDefaitePlusUn(StructJeu *jeu, int indiceJoueur, int victoireOuDefaite)
{
    FILE* fic;
    CompteJoueur c;
    int a = 0;
    int i = 0;


    fic = fopen(CHEMIN_D_ACCES_FICHIER_COMPTES_JOUEURS, "rb");

    if(fic == NULL)
        printf("[LECTURE] Erreur d'accès au fichier ! \n");
    else
    {
        while(fread(&c, sizeof(CompteJoueur),1,fic) && !feof(fic))
        {
            if(strcmp(jeu->listeDesJoueurs[indiceJoueur].compte.nom, c.nom) == 0)
                a = 1;
            if(a != 1)
                i++;
        }
    }
    fclose(fic);

    c = jeu->listeDesJoueurs[indiceJoueur].compte;
    if(victoireOuDefaite == 1)
        c.nbrVictoires++;
    if(victoireOuDefaite == 0)
        c.nbrDefaites++;

    fic = fopen(CHEMIN_D_ACCES_FICHIER_COMPTES_JOUEURS, "r+");

    if(fic == NULL)
        printf("[ECRITURE] Erreur d'accès au fichier ! \n");
    else
    {
        printf("Retour de fseek : %d \n", fseek(fic, sizeof(CompteJoueur)*i, SEEK_SET));

        fwrite(&c, sizeof(c), 1, fic);
    }

    fclose(fic);

}


