#include <stdio.h>
#include <stdlib.h>

#include "../include/audio.h"


void initAudio(StructAudio *audio)
{
    SDL_Init(SDL_INIT_AUDIO);

    if(Mix_OpenAudio( 48000, MIX_DEFAULT_FORMAT, 2,  2048 ) < 0 ) //Ou 2048 // ou 44100
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());

    chargerFichiersAudio(audio);

    Mix_VolumeMusic(60);
}

void chargerFichiersAudio(StructAudio *audio)
{
    //MUSIQUE DU JEU
    audio->musiqueDuJeu = Mix_LoadMUS("assets/audio/spaceTimeTravelMachine-Mandragora.mp3"); //Pour une musique
    if(audio->musiqueDuJeu == NULL )
        printf( "Failed to load musique! SDL_mixer Error: %s\n", Mix_GetError() );

    //SON DES TOUCHES DIRECTIONNELLES
    audio->sonMenuToucheDirectionnelle = Mix_LoadWAV("assets/audio/bruit_touches_directionelles.wav"); //Pour un son
    if(audio->sonMenuToucheDirectionnelle == NULL )
        printf( "Failed to load son menu touche directionnelle SDL_mixer Error: %s\n", Mix_GetError() );

    //SON TOUCHE VALIDER
    audio->sonMenuToucheValider = Mix_LoadWAV("assets/audio/bruit_touche_valider.wav"); //Pour un son
    if(audio->sonMenuToucheValider == NULL )
        printf( "Failed to load son menu touche directionnelle SDL_mixer Error: %s\n", Mix_GetError() );

    //SON DE LA POSE DE LA BOMBE
    audio->sonPoseBombe = Mix_LoadWAV("assets/audio/bruit_mort.wav");
    if(audio->sonPoseBombe == NULL )
        printf( "Failed to load son pose bombe SDL_mixer Error: %s\n", Mix_GetError() );

    //SON EXPLOSION BOMBE
    audio->sonExplosionBombe = Mix_LoadWAV("assets/audio/bruit_explosion_bombe.wav");
    if(audio->sonExplosionBombe == NULL )
        printf( "Failed to load son menu son explosion bombe SDL_mixer Error: %s\n", Mix_GetError() );

    //SON LORS DE LA MORT D'UN PERSONNAGE
    audio->sonMortPersonnage = Mix_LoadWAV("assets/audio/bruit_pose_bombe.wav");
    if(audio->sonMortPersonnage == NULL )
        printf( "Failed to load son menu son mort personnage SDL_mixer Error: %s\n", Mix_GetError() );

    //SON LORS DE LA VICTOIRE
    audio->sonVictoire = Mix_LoadWAV("assets/audio/son_victoire.wav");
    if(audio->sonVictoire == NULL )
        printf( "Failed to load son menu son Victoire SDL_mixer Error: %s\n", Mix_GetError() );

    //SON LORS DE LA DEFAITE
    audio->sonDefaite = Mix_LoadWAV("assets/audio/son_defaite.wav");
    if(audio->sonDefaite == NULL )
        printf( "Failed to load son menu son Defaite SDL_mixer Error: %s\n", Mix_GetError() );
}

void lireUnSon(StructAudio *audio, SonSelectionne sonSelectionne)
{
    if(sonSelectionne == MUSIQUE)
        Mix_PlayMusic(audio->musiqueDuJeu, -1 ); //Pour une musique

    if(sonSelectionne == SON_MENU_TOUCHE_DIRECTIONNELLE)
        Mix_PlayChannel(-1, audio->sonMenuToucheDirectionnelle, 0); //Pour un son

    if(sonSelectionne == SON_MENU_TOUCHE_VALIDER)
        Mix_PlayChannel(-1, audio->sonMenuToucheValider, 0); //Pour un son

    if(sonSelectionne == SON_POSE_BOMBE)
        Mix_PlayChannel(-1, audio->sonPoseBombe, 0); //Pour un son

    if(sonSelectionne == SON_EXPLOSION_BOMBE)
        Mix_PlayChannel(-1, audio->sonExplosionBombe, 0); //Pour un son

    if(sonSelectionne == SON_MORT_PERSONNAGE)
        Mix_PlayChannel(-1, audio->sonMortPersonnage, 0); //Pour un son

    if(sonSelectionne == SON_VICTOIRE)
        Mix_PlayChannel(-1, audio->sonVictoire, 0); //Pour un son

    if(sonSelectionne == SON_DEFAITE)
        Mix_PlayChannel(-1, audio->sonDefaite, 0); //Pour un son
}
