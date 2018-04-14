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
#include "../include/audio.h"



int main(int argc, char *argv[])
{
    //Déclaration des 4 structures principales du jeu
    StructJeu jeu;
    StructMenu menu;
    StructAffichage affichage;
    StructTouchesClavier clavier;
    StructAudio audio;


    srand(time(NULL));

    initMenu(&menu);
    initAffichage(&affichage, "Bomberman");
    initClavier(&clavier);
    initAudio(&audio);
    jeu.etat = OFF;

    lireUnSon(&audio, MUSIQUE);

    do
    {

        recupererTouchesClavier(&clavier);

        if(jeu.etat == OFF || jeu.etat == PAUSE)
            gestionDuMenu(&menu, &jeu, &clavier, &affichage, &audio);

        if(jeu.etat == LANCEMENT)
        {
            initJeu(&jeu);
            jeu.etat = ON;
        }

        if(jeu.etat == ON)
        {
            SDL_Delay(40); //PERMET DE REGLER LA VITESSE DU JEU (Initialement dans clavier.c mais ralentissait le menu)
            calculerJeu(&jeu, &clavier, &audio);
            afficherJeu(&affichage, &jeu);
            // Afficher les informations de debug
            system("clear");
            afficherStructureJeu(jeu);
            //***********************************
        }

          if(jeu.etat == PAUSE)
        {
            menu.numeroFenetre = 7;
        }


        if(jeu.etat == EXTINCTION){
            jeu.etat = OFF;
            initMenu(&menu);
            menu.numeroFenetre = 4;

            /** Méthode barbare pour faire en sorte que le profil sélectionné soit actualiser avec le nouveau nbr de victoires/défaites */
            CompteJoueur tabComptes[NBR_MAX_COMPTES];
            chargerComptes(tabComptes);
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

    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

}




