#include <stdio.h>
#include <stdlib.h>

#include "../include/audio.h"


void initAudio(StructAudio *audio)
{
    // Initialiser les bibliothèques audio
    SDL_Init(SDL_INIT_AUDIO);

    if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2,  2048) < 0)
        printf("SDL_mixer n'a pas pu être initialisé ! Erreur SDL_mixer : %s\n", Mix_GetError());

    // Charger les fichiers audio
    chargerAudio(audio);

    // Définir le volume des musiques
    Mix_VolumeMusic(60);
}

void chargerAudio(StructAudio *audio)
{
    // Musique du jeu
    audio->musiqueDuJeu = Mix_LoadMUS("assets/audio/spaceTimeTravelMachine-Mandragora.mp3");
    if(audio->musiqueDuJeu == NULL)
        printf("L'audio 'musiqueDuJeu' n'a pas pu être chargé ! Erreur SDL_mixer : %s\n", Mix_GetError());

    // Son des touches directionnelles
    audio->sonMenuToucheDirectionnelle = Mix_LoadWAV("assets/audio/bruit_touches_directionelles.wav");
    if(audio->sonMenuToucheDirectionnelle == NULL)
        printf("L'audio 'sonMenuToucheDirectionnelle' n'a pas pu être chargé ! Erreur SDL_mixer : %s\n", Mix_GetError());

    // Son de la touche valider
    audio->sonMenuToucheValider = Mix_LoadWAV("assets/audio/bruit_touche_valider.wav");
    if(audio->sonMenuToucheValider == NULL)
        printf("L'audio 'sonMenuToucheValider' n'a pas pu être chargé ! Erreur SDL_mixer : %s\n", Mix_GetError());

    // Son d'une pose de bombe
    audio->sonPoseBombe = Mix_LoadWAV("assets/audio/bruit_mort.wav");
    if(audio->sonPoseBombe == NULL)
        printf("L'audio 'sonPoseBombe' n'a pas pu être chargé ! Erreur SDL_mixer : %s\n", Mix_GetError());

    // Son d'une explosion de bombe
    audio->sonExplosionBombe = Mix_LoadWAV("assets/audio/bruit_explosion_bombe.wav");
    if(audio->sonExplosionBombe == NULL)
        printf("L'audio 'sonExplosionBombe' n'a pas pu être chargé ! Erreur SDL_mixer : %s\n", Mix_GetError());

    // Son de la mort d'un personnage
    audio->sonMortPersonnage = Mix_LoadWAV("assets/audio/bruit_pose_bombe.wav");
    if(audio->sonMortPersonnage == NULL)
        printf("L'audio 'sonMortPersonnage' n'a pas pu être chargé ! Erreur SDL_mixer : %s\n", Mix_GetError());

    // Son d'une victoire
    audio->sonVictoire = Mix_LoadWAV("assets/audio/son_victoire.wav");
    if(audio->sonVictoire == NULL)
        printf("L'audio 'sonVictoire' n'a pas pu être chargé ! Erreur SDL_mixer : %s\n", Mix_GetError());

    // Son d'une défaite
    audio->sonDefaite = Mix_LoadWAV("assets/audio/son_defaite.wav");
    if(audio->sonDefaite == NULL)
        printf("L'audio 'sonDefaite' n'a pas pu être chargé ! Erreur SDL_mixer : %s\n", Mix_GetError());
}

void lireAudio(StructAudio *audio, SonSelectionne sonSelectionne)
{
    // Musique du jeu
    if(sonSelectionne == MUSIQUE)
        Mix_PlayMusic(audio->musiqueDuJeu, -1);

    // Son des touches directionnelles
    if(sonSelectionne == SON_MENU_TOUCHE_DIRECTIONNELLE)
        Mix_PlayChannel(-1, audio->sonMenuToucheDirectionnelle, 0);

    // Son de la touche valider
    if(sonSelectionne == SON_MENU_TOUCHE_VALIDER)
        Mix_PlayChannel(-1, audio->sonMenuToucheValider, 0);

    // Son d'une pose de bombe
    if(sonSelectionne == SON_POSE_BOMBE)
        Mix_PlayChannel(-1, audio->sonPoseBombe, 0);

    // Son d'une explosion de bombe
    if(sonSelectionne == SON_EXPLOSION_BOMBE)
        Mix_PlayChannel(-1, audio->sonExplosionBombe, 0);

    // Son de la mort d'un personnage
    if(sonSelectionne == SON_MORT_PERSONNAGE)
        Mix_PlayChannel(-1, audio->sonMortPersonnage, 0);

    // Son d'une victoire
    if(sonSelectionne == SON_VICTOIRE)
        Mix_PlayChannel(-1, audio->sonVictoire, 0);

    // Son d'une défaite
    if(sonSelectionne == SON_DEFAITE)
        Mix_PlayChannel(-1, audio->sonDefaite, 0);
}
