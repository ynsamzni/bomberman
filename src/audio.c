#include <stdio.h>
#include <stdlib.h>
#include "../include/audio.h"


void initAudio(StructAudio *audio)
{
    SDL_Init(SDL_INIT_AUDIO);

    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2,  2048 ) < 0 ) //Ou 2048
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());

    chargerFichiersAudio(audio);

    Mix_VolumeMusic(65);

}

void chargerFichiersAudio(StructAudio *audio)
{
    audio->musiqueDuJeu = Mix_LoadMUS("assets/audio/musique_random_1.mp3"); //Pour une musique
    if(audio->musiqueDuJeu == NULL )
        printf( "Failed to load musique! SDL_mixer Error: %s\n", Mix_GetError() );

    audio->sonMenuToucheDirectionnelle = Mix_LoadWAV("assets/audio/bruit_touche_directionelle_2.wav"); //Pour un son
    if(audio->sonMenuToucheDirectionnelle == NULL )
        printf( "Failed to load son menu touche directionnelle SDL_mixer Error: %s\n", Mix_GetError() );

    audio->sonPoseBombe = Mix_LoadWAV("assets/audio/bruit_pose_bombe.wav");
    if(audio->sonPoseBombe == NULL )
        printf( "Failed to load son menu touche directionnelle SDL_mixer Error: %s\n", Mix_GetError() );

    audio->sonExplosionBombe = Mix_LoadWAV("assets/audio/bruit_explosion.wav");
    if(audio->sonExplosionBombe == NULL )
        printf( "Failed to load son menu touche directionnelle SDL_mixer Error: %s\n", Mix_GetError() );
}

void lireUnSon(StructAudio *audio, SonSelectionne sonSelectionne){

    if(sonSelectionne == MUSIQUE)
        Mix_PlayMusic(audio->musiqueDuJeu, -1 ); //Pour une musique

    if(sonSelectionne == SON_MENU_TOUCHE_DIRECTIONNELLE)
        Mix_PlayChannel(-1, audio->sonMenuToucheDirectionnelle, 0); //Pour un son

    if(sonSelectionne == SON_POSE_BOMBE)
        Mix_PlayChannel(-1, audio->sonPoseBombe, 0); //Pour un son

}


