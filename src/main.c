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

    int check = 0;


    srand(time(NULL));

    initMenu(&menu); // NUMERO FENETRE => 1
    initAffichage(&affichage, "Bomberman");
    initClavier(&clavier);
    jeu.etat = OFF;


    do
    {

        recupererTouchesClavier(&clavier);

        if(jeu.etat == OFF)
            gestionDuMenu(&menu, &jeu, &clavier, &affichage);

        if(jeu.etat == LANCEMENT)
        {
            initJeu(&jeu);
            jeu.etat = ON;
        }

        if(jeu.etat == ON)
        {
            SDL_Delay(40); //PERMET DE REGLER LA VITESSE DU JEU (Initialement dans clavier.c mais ralentissait le menu)
            calculerJeu(&jeu, &clavier);
            afficherJeu(&affichage, &jeu);
            // Afficher les informations de debug
            system("clear");
            afficherStructureJeu(jeu);
            //***********************************
        }

        if(jeu.etat == EXTINCTION){
            jeu.etat = OFF;
            initMenu(&menu);
            menu.numeroFenetre = 4;

            /** Méthode barbare pour faire en sorte que le profil sélectionné soit actualiser avec le nouveau nbr de victoires/défaites */
            CompteJoueur tabComptes[NBR_MAX_COMPTES];
            chargerComptes(&tabComptes);
            for(int i = 0; i< NBR_MAX_COMPTES; i++){
                if(strcmp(menu.profilSelectionne.nom, tabComptes[i].nom) == 0)
                    menu.profilSelectionne = tabComptes[i];
            }
            /*********************************************************************************************************/
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

