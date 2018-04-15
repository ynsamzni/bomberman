#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/affichage.h"


void initAffichage(StructAffichage *affichage, char nomFenetre[])
{
    SDL_Init(SDL_INIT_VIDEO); //Initialise le système de gestion de rendu
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    affichage->window = SDL_CreateWindow(nomFenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN); //Création d'une fenêtre
    SDL_SetWindowIcon(affichage->window, IMG_Load("assets/img/icon.png"));
    SDL_RaiseWindow(affichage->window); //Permet de placer la fenêtre au dessus de toutes les autres

    affichage->renderer = SDL_CreateRenderer(affichage->window, -1, SDL_RENDERER_ACCELERATED); //Renderer, permettant de de "dessiner" dans la fenêtre

    chargementTextures(&affichage->structTextures, affichage->renderer);
    chargementCouleurs(&affichage->structCouleur);
}

void chargementTextures(StructTextures *structTextures, SDL_Renderer *renderer)
{
    SDL_Surface *surfaceTmp = NULL;

    surfaceTmp = IMG_Load("assets/img/feuille_sprite.png");
    (*structTextures).feuilleSprites = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/mur_indestructible.bmp"); //On charge une image sans la surface
    (*structTextures).murIndestructible = SDL_CreateTextureFromSurface(renderer, surfaceTmp); //On met la surface dans une texture

    surfaceTmp = IMG_Load("assets/img/mur_destructible.png");
    (*structTextures).murDestructible = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/bombe.png");
    (*structTextures).bombe = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/etincelle.gif");
    (*structTextures).etincelle = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/explosion.png");
    (*structTextures).explosion = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/victoire.png");
    (*structTextures).victoire = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/defaite.png");
    (*structTextures).defaite = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_bas.png");
    (*structTextures).flecheBasse = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_haut.png");
    (*structTextures).flecheHaute = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_gauche.png");
    (*structTextures).flecheGauche = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_droite.png");
    (*structTextures).flecheDroite = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_gauche_blanche.png");
    (*structTextures).flecheGaucheBlanche = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_droite_blanche.png");
    (*structTextures).flecheDroiteBlanche = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    SDL_FreeSurface(surfaceTmp); //On peut donc "détruire" la surface
}

void chargementCouleurs(StructCouleur *structCouleur)
{
    structCouleur->blanc.r = 255;
    structCouleur->blanc.g = 255;
    structCouleur->blanc.b = 255;

    structCouleur->noir.r = 0;
    structCouleur->noir.g = 0;
    structCouleur->noir.b = 0;
}

void afficherTexte(char texte[], int tailleTexte, SDL_Color couleurTexte, char cheminPoliceEcriture[], int positionX, int positionY, SDL_Renderer *renderer)
{
    int textureW, textureH;

    // Charger le fichier comportant la police d'écriture
    TTF_Font *policeEcriture = TTF_OpenFont(cheminPoliceEcriture, tailleTexte);

    // Ecrire le texte dans une surface
    SDL_Surface *surface = TTF_RenderText_Blended(policeEcriture, texte, couleurTexte);

    // Créer une texture à partir de la surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Déterminer les dimensions de la texture
    SDL_QueryTexture(texture, NULL, NULL, &textureW, &textureH);

    // Déterminer les coordonnées de la texture si aucune n'ont pas été indiquées
    if(positionX == -1)
        positionX = (WIDTH / 2) - (textureW / 2);
    if(positionY == -1)
        positionY = (HEIGHT / 2) - (textureH / 2);

    // Créer le rectangle qui contiendra les coordonnées et dimensions de la texture
    SDL_Rect rectTexture = {positionX, positionY, textureW, textureH};

    // Afficher le texte
    SDL_RenderCopy(renderer, texture, NULL, &rectTexture);

    // Libérer de la RAM
    SDL_FreeSurface(surface);
    TTF_CloseFont(policeEcriture);
    SDL_DestroyTexture(texture);
}


/******************************************************************************/
/*****************************AFFICHAGE DU JEU*********************************/
/******************************************************************************/

void afficherJeu(StructAffichage *affichage, StructJeu *jeu)
{
    SDL_Rect caseMap = {0, 0, 30, 30}; // Case utilisée pour remplir la map
    SDL_Rect spriteDecoupe;
    SDL_Rect spriteInitial = {15, 13, 35, 50};  // Coordonnées du sprite initial dans la feuille de sprites
    int nbTotalFrames = 9; // Nombre de sprites sur une même ligne de la feuille de sprites
    int dureeParFrame = 100;
    int frame;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Afficher les cases
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j<20; j++)
        {
            caseMap.x = i*30;
            caseMap.y = j*30;

            if(jeu->mapJeu[i][j] == 1)
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.murIndestructible, NULL, &caseMap);
            else if(jeu->mapJeu[i][j] == 2)
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.murDestructible, NULL, &caseMap);
            else if(jeu->mapJeu[i][j] == 3)
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.bombe, NULL, &caseMap);
            else if(jeu->mapJeu[i][j] == 4)
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.explosion, NULL, &caseMap);
        }
    }

    // Afficher les joueurs
    for(int i = 0; i < jeu->nbrDeJoueurs; i++)
    {
        if(jeu->listeDesJoueurs[i].enVie == 1)
        {
            spriteDecoupe=spriteInitial;

            // Déterminer les coordonnées X du nouveau sprite
            if(jeu->listeDesJoueurs[i].deplacement == 1)
                frame = (SDL_GetTicks() / dureeParFrame) % nbTotalFrames; // Passer à la prochaine frame toutes les 'dureeParFrame' ms
            else
                frame = 0;
            spriteDecoupe.x += frame * 64;

            // Déterminer les coordonnées Y du nouveau sprite
            if(jeu->listeDesJoueurs[i].direction == HAUT)
                spriteDecoupe.y += 0;
            if(jeu->listeDesJoueurs[i].direction == GAUCHE)
                spriteDecoupe.y += 64;
            if(jeu->listeDesJoueurs[i].direction == BAS)
                spriteDecoupe.y += 2*64;
            if(jeu->listeDesJoueurs[i].direction == DROITE)
                spriteDecoupe.y += 3*64;

            // Déterminer la couleur du sprite
            switch(i)
            {
            case 0:
                SDL_SetTextureColorMod(affichage->structTextures.feuilleSprites, 255, 255, 255);
                break;
            case 1:
                SDL_SetTextureColorMod(affichage->structTextures.feuilleSprites, 0,100,255); //BLEU
                break;
            case 2:
                SDL_SetTextureColorMod(affichage->structTextures.feuilleSprites, 0, 255, 100); //VERT
                break;
            case 3:
                SDL_SetTextureColorMod(affichage->structTextures.feuilleSprites, 255, 70, 0); //ROUGE
                break;
            }

            // Déterminer la case sur laquelle se trouve le joueur
            caseMap.x = jeu->listeDesJoueurs[i].coordonnes.x;
            caseMap.y = jeu->listeDesJoueurs[i].coordonnes.y;

            // Afficher le sprite mis à jour
            SDL_RenderCopy(affichage->renderer, affichage->structTextures.feuilleSprites, &spriteDecoupe, &caseMap);
        }
    }


    //Gestion des animations
    if(jeu->animations.victoire == 1)
        animationVictoire(affichage);
    if(jeu->animations.defaite == 1)
        animationDefaite(affichage);


    // Actualiser l'affichage
    SDL_RenderPresent(affichage->renderer);
}

void animationVictoire(StructAffichage *affichage)
{
    afficherTexte("VICTOIRE !", 80, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 180, affichage->renderer);
}

void animationDefaite(StructAffichage *affichage)
{
    afficherTexte("DEFAITE !", 80, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 180, affichage->renderer);
}
