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






int main(int argc, char *argv[])
{
    //Déclaration des 4 structures principales du jeu
    StructJeu jeu;
    StructMenu menu;
    StructAffichage affichage;
    StructTouchesClavier clavier;

    //Initialisation du jeu
    srand(time(NULL));
    initJeu(&jeu, 4);
    initMenu(&menu);
    initAffichage(&affichage, "Bomberman");
    initClavier(&clavier);

    do
    {
        recupererTouchesClavier(&clavier);

        switch(affichage.numeroFenetre)
        {
            case 0:
                clavier.toucheQuitter = 1;
                break;
            case 1:
                calculerJeu(&jeu, &clavier);
                afficherJeu(&affichage, &jeu);
                // Afficher les informations de debug
                system("clear");
                afficherStructureJeu(jeu);
                break;
            case 2:
                afficherMenuPrincipal(&affichage, &clavier, &menu);
                break;
            case 3:
                afficherMenuSelectionProfil(&affichage, &clavier, &jeu, &menu);
                break;
            case 4:
                afficherMenuCreationProfil(&affichage, &clavier, &jeu, &menu);
                break;
        }
    }
    while(clavier.toucheQuitter != 1);

    SDL_DestroyRenderer(affichage.renderer);//Permet de détruire le renderer
    SDL_DestroyWindow(affichage.window); //Permet de détruire la fenêtre crée
    IMG_Quit();
    TTF_Quit();
    SDL_Quit(); //Permet de quitter proprement la SDL

}




