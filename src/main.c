#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/constantes.h"
#include "../include/affichage.h"
#include "../include/jeu.h"
#include "../include/clavier.h"
#include "../include/menu.h"

/****************Structure du jeu**********************

StructJeu
=> map
=> coordonnées du joueur1
=> coordonnées du joueur2, joueur3, joueur4
=> coordonnées des bombes (tableau ?)

StructAffichage
=> la fenetre
=> le renderer
=> les textures

StructAudio
=> les différents sons utilisés

StructTouchesClaviers
=> touches pressées par le joueur


InitAffichage(StructAffichage);    => initialise l'affichage et charge les textures
InitClavier(StructTouchesClavier) => initialise les touches
InitAudio(StructAudio); => initialise l'audio en chargeant les sons dans la structure
InitJeu(StructJeu);          => initialise les variables du jeu, notamment en fonction du nombre de joueur, etc.. et charge la map


do{

AfficherJeu(StructAffichage, StructJeu);  => Dessinne le jeu
JouerAudio(StructJeu, StructAudio); => Assure la gestion de l'audio
RecupererTouchesClavier(StructTouchesClavier);    => Récupère les positions
CalculerJeu(StructTouchesClavier, StructJeu );  => Calcul les positions des joueurs, etc..


}while



***************************************************/

void fonctionTestMenu(StructJeu *jeu);




int main(int argc, char *argv[])
{
    //Déclaration des 4 structures principales du jeu
    StructJeu jeu;
    StructMenu menu;
    StructAffichage affichage;
    StructTouchesClavier clavier;

    int demarrerJeu = 0;
    int check = 0;


    srand(time(NULL));

    initMenu(&menu); // NUMERO FENETRE => 1
    initAffichage(&affichage, "Bomberman");
    initClavier(&clavier);



    do
    {

        recupererTouchesClavier(&clavier);

        if(demarrerJeu == 0)
        {
            switch(menu.numeroFenetre)
            {
            case 0:
                // Première page du menu, page d'acceuil
                break;
            case 1:
                 // Menu demandant de sélectionner un profil et/ou d'en créer un
                afficherMenuSelectionProfil(&affichage, &clavier, &jeu, &menu);
                break;
            case 2:
                // Menu de création de profil
                afficherMenuCreationProfil(&affichage, &clavier, &jeu, &menu);
                break;
            case 3:
                // Menu "principal" (jouer + statistiques)
                afficherMenuPrincipal(&affichage, &clavier, &menu);
                break;
            case 4:
                // Menu affichant les statistiques

                break;
            case 5:
                // Menu permettant de paramétrer sa partie
                afficherMenuParametragePartie(&affichage, &clavier, &jeu, &menu);
                break;
            case 6:
                //Permet de lancer le jeu
                demarrerJeu = 1;
                break;
            case -1:
                clavier.toucheQuitter = 1;
            break;
            }
        }

        if(demarrerJeu == 1)
        {
            if(check == 0)
            {
                initJeu(&jeu);
                check = 1;
            }

            calculerJeu(&jeu, &clavier);
            afficherJeu(&affichage, &jeu);
            // Afficher les informations de debug
            system("clear");
            afficherStructureJeu(jeu);
            if(clavier.toucheArriere == 1){
                check = 0;
                demarrerJeu = 0;
                menu.numeroFenetre = 5;
            }
        }


    }
    while(clavier.toucheQuitter != 1);

    SDL_DestroyRenderer(affichage.renderer);//Permet de détruire le renderer
    SDL_DestroyWindow(affichage.window); //Permet de détruire la fenêtre crée
    IMG_Quit();
    TTF_Quit();
    SDL_Quit(); //Permet de quitter proprement la SDL

}



void fonctionTestMenu(StructJeu *jeu)  //Fonction permettant de tester les différents paramètres de jeu pouvant être sélectionnés par le joueur dans le menu
{
    //!!! Cette fonction décide aussi quels joueurs sont humains et quels joueurs sont des IA

    int nbrJoueursHumains;
    int nbrJoueursIA;
    printf("*********MENU BOMBERMAN*********\n\n");
    printf("nbr de joueurs humains : ");
    scanf("%d", &nbrJoueursHumains);
    printf("nbr de joueurs IA : ");
    scanf("%d", &nbrJoueursIA);
    printf("\n");

    for(int i = 0; i < nbrJoueursHumains; i++)
    {
        printf("***Saisie J%d***\n", i+1);
        jeu->listeDesJoueurs[i].humainOuIA = 0;
        printf("Nom : ");
        __fpurge(stdin);
        fgets(jeu->listeDesJoueurs[i].compte.nom, 25, stdin);
        printf("nbr de victoire : ");
        scanf("%d", &jeu->listeDesJoueurs[i].compte.nbrVictoires);
        printf("nbr défaites : ");
        scanf("%d", &jeu->listeDesJoueurs[i].compte.nbrDefaites);
        printf("\n");
    }
    for(int i = nbrJoueursHumains; i < nbrJoueursHumains + nbrJoueursIA; i++)
        jeu->listeDesJoueurs[i].humainOuIA = 1;

    jeu->nbrDeJoueurs = nbrJoueursHumains + nbrJoueursIA;




}

