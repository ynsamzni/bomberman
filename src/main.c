#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "../include/affichage.h"
#include "../include/audio.h"
#include "../include/clavier.h"
#include "../include/jeu.h"
#include "../include/menu.h"


int main(int argc, char *argv[])
{
    // Déclarer les 4 structures principales
    StructJeu jeu;
    StructMenu menu;
    StructAffichage affichage;
    StructClavier clavier;
    StructAudio audio;

    // Initialiser le générateur de nombres pseudo-aléatoires
    srand(time(NULL));

    // Initialiser le jeu
    initMenu(&menu);
    initAffichage(&affichage, "Bomberman");
    initClavier(&clavier);
    initAudio(&audio);
    jeu.etat = OFF;

    // Débuter la lecture de la musique du jeu
    lireAudio(&audio, MUSIQUE);

    do
    {
        // Déterminer les touches utilisées
        recupererEtatTouchesClavier(&clavier);

        switch(jeu.etat)
        {
        case OFF:
            afficherMenu(&menu, &jeu, &clavier, &affichage, &audio);
            break;

        case LANCEMENT:
            initJeu(&jeu);
            jeu.etat = ON;
            break;

        case ON:
            // Régler la vitesse du jeu
            SDL_Delay(40);
            // Gérer les actions des joueurs
            calculerJeu(&jeu, &clavier, &audio);
            // Afficher les actions des joueurs
            afficherJeu(&affichage, &jeu);
            // Debug
            system("clear");
            debugAfficherInformationsPartie(&jeu);
            break;

        case PAUSE:
            menu.numeroFenetre = 7;
            afficherMenu(&menu, &jeu, &clavier, &affichage, &audio);
            break;

        case EXTINCTION:
            // Figer le jeu sur la derniere action en fin de partie
            while(cycleToucheClavierRealise(&clavier.toucheAction, &clavier) != 1 && !clavier.toucheQuitter)
                recupererEtatTouchesClavier(&clavier);
            // Arrêter le jeu
            jeu.etat = OFF;
            // Afficher le menu des statistiques
            menu.numeroFenetre = 4;
            break;
        }
    }
    while(!clavier.toucheQuitter);

    // Détruire le renderer
    SDL_DestroyRenderer(affichage.renderer);

    // Détruire la fenêtre
    SDL_DestroyWindow(affichage.window);

    // Quitter les bibliothèques de rendu
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
