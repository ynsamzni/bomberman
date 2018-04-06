#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL_mixer.h>

typedef enum{
    MUSIQUE, SON_MENU_TOUCHE_DIRECTIONNELLE, SON_MENU_TOUCHE_BLOQUEE, SON_MENU_TOUCHE_VALIDER, SON_POSE_BOMBE, SON_EXPLOSION_BOMBE
}SonSelectionne;


typedef struct{
    Mix_Music  *musiqueDuJeu;
    Mix_Chunk *sonMenuToucheDirectionnelle;
    Mix_Chunk *sonPoseBombe;
    Mix_Chunk *sonExplosionBombe;
}StructAudio;

void initAudio(StructAudio *audio);
void chargerFichiersAudio(StructAudio *audio);
void lireUnSon(StructAudio *audio, SonSelectionne sonSelectionne);



#endif // AUDIO_H_INCLUDED
