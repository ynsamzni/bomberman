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

    // Numéro de la fenêtre qui doit être affichée
    affichage->numeroFenetre = 2;
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
    /**
    Dans la SDL, il existe une structure spécialement faite pour stocker un rectangle qui s'appelle SDL_Rect.
    Celle-ci comporte quatre champs, tous des int, que vous connaissez déjà :
    x et y : la position du rectangle par rapport au repère de votre fenêtre
    w : la largeur du rectangle
    h : la hauteur du rectangle
    **/


    /**
    Nous avons vu comment afficher une texture dans son ensemble. Cependant, il peut arriver que l'on ait besoin d'afficher
    seulement un rectangle particulier de la texture. Pour cela, il nous faut voir le prototype de la fonction SDL_RenderCopy. Le voici :

    int SDL_RenderCopy(SDL_Renderer*   renderer  ,  SDL_Texture*    texture  ,  const SDL_Rect* srcrect  ,  const SDL_Rect* dstrect);

    La fonction renvoie 0 si tout s'est bien passé et une valeur négative sinon. On peut donc procéder à l'éternelle
    vérification du bon fonctionnement de la fonction mais je vous laisse le soin de la faire (cf chapitre précédent si vous n'y arrivez pas).

    Ce qui nous intéresse, ce sont les arguments srcrect et dstrect qui correspondent respectivement à la source et à la destination de
    la texture.
    - La source est le rectangle au sein même de la texture que l'on veut afficher. Attention, la position du rectangle correspond à un repère
    relatif à l'angle en haut à gauche de la texture et non du renderer. Si l'on envoie NULL, ce rectangle recouvrira la texture entière.
    - La destination est un rectangle relatif au renderer dans lequel on va afficher la source. En fait, c'est comme si l'on considérait
    que la source était une nouvelle texture de la taille du rectangle source et qu'on l'affichait à la position indiquée par la destination.
    - Il n'est pas nécessaire de s'inquiéter de la taille de la destination car l'image mère est redimensionnée pour corresponpondre au carré
    acceuillant l'image.
    **/

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



//Permet d'afficher le fillon de la victoire
void animationVictoire(StructAffichage *affichage)
{
    SDL_Rect rectAffichage = {0, 0, 600, 600}; // Case utilisée pour remplir la map
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.victoire, NULL, &rectAffichage);
    SDL_RenderPresent(affichage->renderer);
}


//Permet d'afficher le loser de la défaite
void animationDefaite(StructAffichage *affichage)
{
    SDL_Rect rectAffichage = {0, 0, 600, 600}; // Case utilisée pour remplir la map
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.defaite, NULL, &rectAffichage);
    SDL_RenderPresent(affichage->renderer);
}


/******************************************************************************/
/****************************AFFICHAGE DES MENUS*******************************/
/******************************************************************************/


void afficherMenuPrincipal(StructAffichage *affichage, StructTouchesClavier *clavier, StructMenu *menu)
{
    // Si l'utilisateur se déplace dans le menu
    if(clavier->toucheBas && menu->position != 2)
        menu->position++;
    if(clavier->toucheHaut && menu->position != 0)
        menu->position--;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("BOMBERMAN", 50, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 140, 50, affichage->renderer);

    if(menu->position == 0)
        afficherTexte("DEBUG : Charger le jeu", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 140, 150, affichage->renderer);
    else
        afficherTexte("DEBUG : Charger le jeu", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 140, 150, affichage->renderer);

    if(menu->position == 1)
        afficherTexte("Jouer", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 250, 200, affichage->renderer);
    else
        afficherTexte("Jouer", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 250, 200, affichage->renderer);

    if(menu->position == 2)
        afficherTexte("Quitter", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 240, 250, affichage->renderer);
    else
        afficherTexte("Quitter", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 240, 250, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        switch(menu->position)
        {
            case 0:
                affichage->numeroFenetre = 1;
                break;
            case 1:
                affichage->numeroFenetre = 3;
                break;
            case 2:
                affichage->numeroFenetre = 0;
                break;
        }
        initMenu(menu);
    }
    else if(clavier->toucheQuitter)
        affichage->numeroFenetre = 0;
}


void afficherMenuSelectionProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu)
{
    // Variables temporaires
    int nbTotalProfils=1;
    char nomProfil[5]="Yanis";

    // Si l'utilisateur se déplace dans le menu
    if(clavier->toucheBas && menu->position != nbTotalProfils)
        menu->position++;
    if(clavier->toucheHaut && menu->position != 0)
        menu->position--;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("SELECTION DU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 65, 70, affichage->renderer);

    for(int i=0; i<nbTotalProfils; i++)
    {
        if(menu->position == i)
            afficherTexte(nomProfil, 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 260, 170 + 50 * i, affichage->renderer);
        else
            afficherTexte(nomProfil, 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 260, 170 + 50 * i, affichage->renderer);
    }

    if(menu->position == nbTotalProfils)
        afficherTexte("Nouveau profil", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 190, 170 + 50 * nbTotalProfils, affichage->renderer);
    else
        afficherTexte("Nouveau profil", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 190, 170 + 50 * nbTotalProfils, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        switch(menu->position)
        {
            case 0:
                affichage->numeroFenetre = 1;
                break;
            case 1:
                affichage->numeroFenetre = 4;
                break;
        }
        initMenu(menu);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        affichage->numeroFenetre = 2;
        initMenu(menu);
    }
    else if(clavier->toucheQuitter)
        affichage->numeroFenetre = 0;
}


void afficherMenuCreationProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu)
{
    char caractereTmp;

    // Coordonnées initiales des éléments du menu
    SDL_Rect caseLettre = {130, 210, 65, 60};
    SDL_Rect lettre = {caseLettre.x + 5, caseLettre.y - 5, 0, 0};
    SDL_Rect flecheHaute = {caseLettre.x + 5, caseLettre.y - 30, 40, 30};
    SDL_Rect flecheBasse = {caseLettre.x + 5, caseLettre.y + caseLettre.h, 40, 30};

    // Si l'utilisateur se déplace dans le menu
    if(clavier->toucheDroite && menu->position != 4)
            menu->position++;
    if(clavier->toucheGauche && menu->position != 0)
            menu->position--;
    if(clavier->toucheHaut && menu->entreeTexte[menu->position] != 'A')
            menu->entreeTexte[menu->position]--;
    if(clavier->toucheBas && menu->entreeTexte[menu->position] != 'Z')
            menu->entreeTexte[menu->position]++;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("NOUVEAU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 110, 70, affichage->renderer);

    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);
    for(int i=0; i<5; i++)
    {
        // Cases contenant les lettres du nom
        SDL_RenderDrawRect(affichage->renderer, &caseLettre);

        // Nom
        caractereTmp=menu->entreeTexte[i];
        afficherTexte(&caractereTmp, 55, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, lettre.x, lettre.y, affichage->renderer);

        // Flèches
        if(i == menu->position)
        {
            if(menu->entreeTexte[menu->position] != 'A')
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheHaute, NULL, &flecheHaute);
            if(menu->entreeTexte[menu->position] != 'Z')
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheBasse, NULL, &flecheBasse);
        }

        // Passer aux coordonnées de la prochaine case
        caseLettre.x += caseLettre.w;
        lettre.x += caseLettre.w;
        flecheHaute.x += caseLettre.w;
        flecheBasse.x += caseLettre.w;
    }

    afficherTexte("ENTREE pour continuer", 24, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 150, 380, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        strcpy(jeu->listeDesJoueurs[0].nom, menu->entreeTexte);
        affichage->numeroFenetre = 1;
        initMenu(menu);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        affichage->numeroFenetre = 3;
        initMenu(menu);
    }
    else if(clavier->toucheQuitter)
        affichage->numeroFenetre = 0;

}
