#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/clavier.h"


void initClavier(StructTouchesClavier *clavier)
{
    //Touches J1
    clavier->toucheHautJ1 = 0; //perfectible avec un clavier = {0, 0, 0, 0, 0, 0} ?
    clavier->toucheBasJ1 = 0;
    clavier->toucheDroiteJ1 = 0;
    clavier->toucheGaucheJ1 = 0;
    clavier->toucheBombeJ1 = 0;

    //Touches J2
    clavier->toucheHautJ2 = 0;
    clavier->toucheBasJ2 = 0;
    clavier->toucheDroiteJ2 = 0;
    clavier->toucheGaucheJ2 = 0;
    clavier->toucheBombeJ2 = 0;

    //Touches menu
    clavier->toucheHaut = 0; //perfectible avec un clavier = {0, 0, 0, 0, 0, 0} ?
    clavier->toucheBas = 0;
    clavier->toucheDroite = 0;
    clavier->toucheGauche = 0;
    clavier->toucheAction = 0;
    clavier->toucheArriere = 0;
    clavier->toucheQuitter = 0;
}

void recupererTouchesClavier(StructTouchesClavier *clavier)
{

    SDL_Event event;
    //Suppression car ralentit le menu SDL_Delay(40);

    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {

            case SDLK_UP:
                clavier->toucheHautJ1 = 1;
                clavier->toucheHaut = 1;
                break;

            case SDLK_DOWN:
                clavier->toucheBasJ1 = 1;
                clavier->toucheBas = 1;
                break;

            case SDLK_RIGHT:
                clavier->toucheDroiteJ1 = 1;
                clavier->toucheDroite = 1;
                break;

            case SDLK_LEFT:
                clavier->toucheGaucheJ1 = 1;
                clavier->toucheGauche = 1;
                break;

            case SDLK_SPACE:
                clavier->toucheBombeJ1 = 1;
                break;

            case SDLK_z:
                clavier->toucheHautJ2 = 1;
                break;

            case SDLK_s:
                clavier->toucheBasJ2 = 1;
                break;

            case SDLK_d:
                clavier->toucheDroiteJ2= 1;
                break;

            case SDLK_q:
                clavier->toucheGaucheJ2 = 1;
                break;

            case SDLK_v:
                clavier->toucheBombeJ2 = 1;
                break;

            case SDLK_RETURN:
                clavier->toucheAction = 1;
                break;

            case SDLK_ESCAPE:
                clavier->toucheArriere = 1;
                break;

            case SDLK_p:
                clavier->toucheQuitter = 1;
                break;

            }
        }

        if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_UP:
                clavier->toucheHautJ1 = 0;
                clavier->toucheHaut = 0;
                break;

            case SDLK_DOWN:
                clavier->toucheBasJ1 = 0;
                clavier->toucheBas = 0;
                break;

            case SDLK_RIGHT:
                clavier->toucheDroiteJ1 = 0;
                clavier->toucheDroite = 0;
                break;

            case SDLK_LEFT:
                clavier->toucheGaucheJ1 = 0;
                clavier->toucheGauche = 0;
                break;

            case SDLK_SPACE:
                clavier->toucheBombeJ1 = 0;
                break;

            case SDLK_z:
                clavier->toucheHautJ2 = 0;
                break;

            case SDLK_s:
                clavier->toucheBasJ2 = 0;
                break;

            case SDLK_d:
                clavier->toucheDroiteJ2 = 0;
                break;

            case SDLK_q:
                clavier->toucheGaucheJ2 = 0;
                break;

            case SDLK_v:
                clavier->toucheBombeJ2 = 0;
                break;

            case SDLK_RETURN:
                clavier->toucheAction = 0;
                break;

            case SDLK_ESCAPE:
                clavier->toucheArriere = 0;
                break;

            case SDLK_p:
                clavier->toucheQuitter = 0;
                break;

            }

            if( event.type == SDL_QUIT )
                clavier->toucheQuitter = 1;
        }

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


void afficherLaStructureDeTouches(StructTouchesClavier clavier)
{


    printf("Valeur toucheHautJ1 :  %d\n", clavier.toucheHautJ1);
    printf("Valeur toucheBasJ1 :  %d\n", clavier.toucheBasJ1);
    printf("Valeur toucheDroiteJ1 :  %d\n", clavier.toucheDroiteJ1);
    printf("Valeur toucheGaucheJ1 :  %d\n", clavier.toucheGaucheJ1);
    printf("Valeur toucheBombeJ1 :  %d\n", clavier.toucheBombeJ1);

    printf("Valeur toucheHautJ2 :  %d\n", clavier.toucheHautJ2);
    printf("Valeur toucheBasJ2 :  %d\n", clavier.toucheBasJ2);
    printf("Valeur toucheDroiteJ2 :  %d\n", clavier.toucheDroiteJ2);
    printf("Valeur toucheGaucheJ2 :  %d\n", clavier.toucheGaucheJ2);
    printf("Valeur toucheBombeJ2 :  %d\n", clavier.toucheBombeJ2);


    printf("Valeur toucheHaut :  %d\n", clavier.toucheHaut);
    printf("Valeur toucheBas :  %d\n", clavier.toucheBas);
    printf("Valeur toucheDroite :  %d\n", clavier.toucheDroite);
    printf("Valeur toucheGauche :  %d\n", clavier.toucheGauche);
    printf("Valeur toucheAction :  %d\n", clavier.toucheAction);
    printf("Valeur toucheQuitter:  %d\n", clavier.toucheQuitter);

    system("clear");

}
