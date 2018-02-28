#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/clavier.h"


void initClavier(StructTouchesClavier *clavier)
{
    clavier->toucheHaut = 0; //perfectible avec un clavier = {0, 0, 0, 0, 0, 0} ?
    clavier->toucheBas = 0;
    clavier->toucheDroite = 0;
    clavier->toucheGauche = 0;
    clavier->toucheAction = 0;
    clavier->toucheQuitter = 0;
}

void recupererTouchesClavier(StructTouchesClavier *clavier)
{

    SDL_Event event;
    SDL_Delay(40);

    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {

            case SDLK_UP:
                clavier->toucheHaut = 1;
                break;

            case SDLK_DOWN:
                clavier->toucheBas = 1;
                break;

            case SDLK_RIGHT:
                clavier->toucheDroite = 1;
                break;

            case SDLK_LEFT:
                clavier->toucheGauche = 1;
                break;

            case SDLK_SPACE:
                clavier->toucheAction = 1;
                break;

            case SDLK_p:
                clavier->toucheQuitter = 1;

            }
        }

        if(event.type == SDL_KEYUP)
        {

            switch(event.key.keysym.sym)
            {

            case SDLK_UP:
                clavier->toucheHaut = 0;
                break;

            case SDLK_DOWN:
                clavier->toucheBas = 0;
                break;

            case SDLK_RIGHT:
                clavier->toucheDroite = 0;
                break;

            case SDLK_LEFT:
                clavier->toucheGauche = 0;
                break;


            case SDLK_SPACE:
                clavier->toucheAction = 0;
                break;

            case SDLK_p:
                clavier->toucheQuitter = 0;

            }

        }

        if( event.type == SDL_QUIT )
            clavier->toucheQuitter = 1;
    }

}


int cycleToucheClavierRealise(int *etatToucheClavier, StructTouchesClavier *clavier)
{
    recupererTouchesClavier(clavier);

    // Si l'utilisateur appuie sur la touche
    if(*etatToucheClavier == 1)
    {
        // Attendre que l'utilisateur relâche la touche
        while(*etatToucheClavier == 1)
            recupererTouchesClavier(clavier);
        return 1;
    }
    // Si l'utilisateur n'appuie pas sur la touche
    else
        return 0;
}


void afficherLaStructureDeTouches(StructTouchesClavier clavier){

    printf("Valeur toucheHaut :  %d\n", clavier.toucheHaut);
    printf("Valeur toucheBas :  %d\n", clavier.toucheBas);
    printf("Valeur toucheDroite :  %d\n", clavier.toucheDroite);
    printf("Valeur toucheGauche :  %d\n", clavier.toucheGauche);
    printf("Valeur toucheAction :  %d\n", clavier.toucheAction);
    printf("Valeur toucheQuitter:  %d\n", clavier.toucheQuitter);

    system("clear");

}
