#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/affichage.h"
#include "../include/clavier.h"


#include "../include/menu.h"

/** Lors du paramétrage de la partie -> fenetre 6
                                     -> declenche le case 6 dans gestion menu -> déclenche jeu->etat = LANCEMENT
                                     -> dans le main on ne va plus dans le menu alors
    */


void initMenu(StructMenu *menu)
{
    menu->numeroFenetre = 0;
    menu->lastNumeroFenetre = menu->numeroFenetre;

    menu->positionCurseurY = 0;
    menu->positionCurseurX = 0;

    menu->paramPartie[0] = 2; // 0 = Ordinateur / 1 = Vide / 2 = Humain
    menu->paramPartie[1] = 0;

    for(int i = 2; i < 4; i++)
        menu->paramPartie[i] = 1;

    for(int i =0; i <8; i++)
        menu->tabNomDuJoueur[i] = 0;

}


void gestionDuMenu(StructMenu *menu, StructJeu *jeu, StructTouchesClavier *clavier, StructAffichage *affichage, StructAudio *audio)
{
    if(menu->numeroFenetre != menu->lastNumeroFenetre){ //réinitialise les curseurs lors du cght de fenetre
        menu->positionCurseurY = 0;
        menu->positionCurseurX = 0;
        if(menu->numeroFenetre == 3) //Cas spécifique du menu principal
            menu->positionCurseurY = 1;
        menu->lastNumeroFenetre = menu->numeroFenetre;
    }

    switch(menu->numeroFenetre)
    {
    case 0:
        // Première page du menu, page d'acceuil
        afficherMenuAccueil(affichage, clavier, jeu, menu);
        break;
    case 1:
        // Menu demandant de sélectionner un profil et/ou d'en créer un
        afficherMenuSelectionProfil(affichage, clavier, jeu, menu, audio);
        break;
    case 2:
        // Menu de création de profil
        afficherMenuCreationProfil(affichage, clavier, jeu, menu, audio);
        break;
    case 3:
        // Menu "principal" (jouer + statistiques)
        afficherMenuPrincipal(affichage, clavier, menu, audio);
        break;
    case 4:
        // Menu affichant les statistiques
        afficherMenuStatistiques(affichage, clavier, menu);
        break;
    case 5:
        // Menu permettant de paramétrer sa partie
        afficherMenuParametragePartie(affichage, clavier, jeu, menu, audio);
        break;
    case 6:
        jeu->etat = LANCEMENT;
        break;
    case 7:
        //Menu pause, un peu particulier car afficher par dessus le jeu
        afficherMenuPause(affichage, clavier, jeu, menu, audio);
        break;
    case -1:
        clavier->toucheQuitter = 1;
        break;
    }
}

/******************** MENU 0 : MENU ACCUEIL **********************/

void afficherMenuAccueil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu)
{
    SDL_Rect rectAffichage;
    rectAffichage.w = 250;
    rectAffichage.h = 250;
    rectAffichage.x = WIDTH / 2 - rectAffichage.w / 2;
    rectAffichage.y = 150;

    int couleurTexteClignotant = (SDL_GetTicks() / 6) % 255; // Passe de 0 à 255 en incrémentant toutes les 6 ms
    SDL_Color color = {couleurTexteClignotant, couleurTexteClignotant, couleurTexteClignotant, 0};

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("* BOMBERMAN *", 50, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 60, affichage->renderer);
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.bombe, NULL, &rectAffichage);
    rectAffichage.w = 60;
    rectAffichage.h = 60;
    rectAffichage.x = 297;
    rectAffichage.y = 132;
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.etincelle, NULL, &rectAffichage);
    afficherTexte("ENTREE pour demarrer", 25, color, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 450, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
        menu->numeroFenetre = 1;
}


/******************** MENU 1 : SELECTION PROFIL **********************/

void afficherMenuSelectionProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu, StructAudio *audio)
{
    int nbTotalProfils;
    CompteJoueur tabComptes[NBR_MAX_COMPTES];

    nbTotalProfils = chargerComptes(tabComptes);

    // Si l'utilisateur se déplace dans le menu
    if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != nbTotalProfils){
        menu->positionCurseurY++;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0){
        menu->positionCurseurY--;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("SELECTION DU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    if(chargerComptes(&tabComptes) != -1) //ON VERIFIE QUE LE FICHIER DES PROFILS NEST PAS VIDE
    {
        for(int i=0; i<nbTotalProfils; i++)
        {
            if(menu->positionCurseurY == i)
                afficherTexte(tabComptes[i].nom, 20, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * i, affichage->renderer);
            else
                afficherTexte(tabComptes[i].nom, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * i, affichage->renderer);
        }

        if(menu->positionCurseurY == nbTotalProfils )
            afficherTexte("Nouveau profil", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * nbTotalProfils, affichage->renderer);
        else
            afficherTexte("Nouveau profil", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * nbTotalProfils, affichage->renderer);
    }
    else
    {
        afficherTexte("APPUYER SUR ENTREE POUR ", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1,250, affichage->renderer);
        afficherTexte("CREER UN NOUVEAU PROFIL", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1,280, affichage->renderer);
    }

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        if(menu->positionCurseurY != nbTotalProfils && nbTotalProfils != -1)
        {

            menu->profilSelectionne = tabComptes[menu->positionCurseurY];
            menu->numeroFenetre = 3;
        }
        else if(menu->positionCurseurY == nbTotalProfils || nbTotalProfils == -1)
            menu->numeroFenetre = 2;
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
        menu->numeroFenetre = 0;
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = -1;
}

/******************** MENU 2 : CREATION PROFIL **********************/

void afficherMenuCreationProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu, StructAudio *audio)
{
    char caractereActuel;
    int nbLettres = 8;

    // Coordonnées initiales des éléments du menu
    SDL_Rect caseLettre;
    caseLettre.w = 55;
    caseLettre.h = 50;
    caseLettre.x = WIDTH / 2 - (caseLettre.w / 2) * nbLettres;
    caseLettre.y = 210;

    SDL_Rect lettre = {caseLettre.x + 5, caseLettre.y - 5, 0, 0};
    SDL_Rect flecheHaute = {caseLettre.x + 5, caseLettre.y - 30, 40, 30};
    SDL_Rect flecheBasse = {caseLettre.x + 5, caseLettre.y + caseLettre.h, 40, 30};

    // Si l'utilisateur se déplace dans le menu
    if(cycleToucheClavierRealise(&clavier->toucheDroite, clavier) && menu->positionCurseurX != 7 && menu->tabNomDuJoueur[menu->positionCurseurX] != 0){
        menu->positionCurseurX++;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    if(cycleToucheClavierRealise(&clavier->toucheGauche, clavier) && menu->positionCurseurX != 0){
        menu->positionCurseurX--;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    //Si l'utilisateur essaye de se déplacer alors que la case est vide
    if(clavier->toucheHaut == 1 && menu->tabNomDuJoueur[menu->positionCurseurX] == 0)
        menu->tabNomDuJoueur[menu->positionCurseurX] = 65;
    if(clavier->toucheBas == 1 && menu->tabNomDuJoueur[menu->positionCurseurX] == 0)
        menu->tabNomDuJoueur[menu->positionCurseurX] = 66;

    //si l'utilisateur se déplace dans une lettre
    if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->tabNomDuJoueur[menu->positionCurseurX] != 'A'){
        menu->tabNomDuJoueur[menu->positionCurseurX]--; //décrémente le caractere ASCII de 1
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->tabNomDuJoueur[menu->positionCurseurX] != 'Z'){
        menu->tabNomDuJoueur[menu->positionCurseurX]++;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    //gestion de la suppression d'une lettre
    if(cycleToucheClavierRealise(&clavier->toucheSupprimer, clavier) == 1 && menu->tabNomDuJoueur[menu->positionCurseurX + 1] == 0 && menu->positionCurseurX != -1){
        menu->tabNomDuJoueur[menu->positionCurseurX] = 0;
        if(menu->positionCurseurX != 0) //empeche de sortir de la zone de saisie en supprimant
            menu->positionCurseurX--;
    }





    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("NOUVEAU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);
    for(int i=0; i<nbLettres; i++)
    {
        // Cases contenant les lettres du nom
        SDL_RenderDrawRect(affichage->renderer, &caseLettre);

        // Nom
        caractereActuel = menu->tabNomDuJoueur[i];
        afficherTexte(&caractereActuel, 45, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, lettre.x, lettre.y, affichage->renderer);

        // Flèches
        if(i == menu->positionCurseurX)
        {
            if(menu->tabNomDuJoueur[menu->positionCurseurX] != 'A')
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheHaute, NULL, &flecheHaute);
            if(menu->tabNomDuJoueur[menu->positionCurseurX] != 'Z')
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheBasse, NULL, &flecheBasse);
        }

        // Passer aux coordonnées de la prochaine case
        caseLettre.x += caseLettre.w;
        lettre.x += caseLettre.w;
        flecheHaute.x += caseLettre.w;
        flecheBasse.x += caseLettre.w;
    }


    afficherTexte("ENTREE pour continuer", 24, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 380, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);


    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        enregistrerNouveauCompte(menu->tabNomDuJoueur);
        menu->numeroFenetre = 1;

    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        menu->numeroFenetre = 1;
    }
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = -1;


}

/***** MENU 3 : MENU PRINCIPAL ************************************/

void afficherMenuPrincipal(StructAffichage *affichage, StructTouchesClavier *clavier, StructMenu *menu, StructAudio *audio)
{
    char chaineBienvenue[40] = "";

    // Si l'utilisateur se déplace dans le menu
    if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != 2){
        menu->positionCurseurY++;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0){
        menu->positionCurseurY--;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer

    strcat(chaineBienvenue,"Bienvenue ");
    strcat(chaineBienvenue, menu->profilSelectionne.nom);
    afficherTexte(chaineBienvenue, 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 50, affichage->renderer);

    if(menu->positionCurseurY == 0)
        afficherTexte("STATISTIQUES", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 150, affichage->renderer);
    else
        afficherTexte("STATISTIQUES", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 150, affichage->renderer);

    if(menu->positionCurseurY == 1)
        afficherTexte("LANCER UNE PARTIE", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 200, affichage->renderer);
    else
        afficherTexte("LANCER UNE PARTIE", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 200, affichage->renderer);

    if(menu->positionCurseurY == 2)
        afficherTexte("Quitter", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 250, affichage->renderer);
    else
        afficherTexte("Quitter", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 250, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        switch(menu->positionCurseurY)
        {
        case 0:
            menu->numeroFenetre = 4; //Statistiques
            break;
        case 1:
            menu->numeroFenetre = 5; //Lancer une partie
            break;
        case 2:
            menu->numeroFenetre = -1; //Quitter
            break;
        }
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        menu->numeroFenetre = 1;
    }
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = 0;


}

/******************** MENU 4 : MENU STATISTIQUES **********************/
void afficherMenuStatistiques(StructAffichage *affichage, StructTouchesClavier *clavier, StructMenu *menu)
{
    CompteJoueur tabComptes[NBR_MAX_COMPTES];
    int nbrDeComptes;
    int tailleMaxListeAAfficher;
    char chaine[15];
    int nbrMaxVictoire;
    CompteJoueur c;
    int i = 0;
    int indiceJoueurAvecMaxVictoire;

    nbrDeComptes = chargerComptes(tabComptes);

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);


    afficherTexte("Statistiques", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 50, affichage->renderer);

    if(nbrDeComptes < 10)
        tailleMaxListeAAfficher = nbrDeComptes;
    else
        tailleMaxListeAAfficher = 10;


    for(int cmpt = 0; cmpt < tailleMaxListeAAfficher; cmpt++)
    {
        nbrMaxVictoire = 0;
        indiceJoueurAvecMaxVictoire = 0;
        for(int j = cmpt; j<nbrDeComptes ; j++)
        {
            if(tabComptes[j].nbrVictoires >= nbrMaxVictoire)
            {
                nbrMaxVictoire = tabComptes[j].nbrVictoires;
                indiceJoueurAvecMaxVictoire = j;
            }
        }
        c = tabComptes[cmpt];
        tabComptes[cmpt] = tabComptes[indiceJoueurAvecMaxVictoire];
        tabComptes[indiceJoueurAvecMaxVictoire] = c;
    }

    while(i < tailleMaxListeAAfficher)
    {

        sprintf(chaine, "Victoires : %d", tabComptes[i].nbrVictoires);
        afficherTexte(tabComptes[i].nom, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 80, (i+1)*50 + 80, affichage->renderer);

        afficherTexte(chaine, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 250, (i+1)*50 + 80, affichage->renderer);
        sprintf(chaine, "Defaites : %d", tabComptes[i].nbrDefaites);

        afficherTexte(chaine, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 400, (i+1)*50 + 80, affichage->renderer);
        i++;
    }

    afficherTexte("Appuyer sur Echap pour returner au menu", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, i*50 + 250, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);


    if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        menu->numeroFenetre = 3;
    }
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = 0;

}


/******************** MENU 5 : MENU DE PARAMETRAGE DE LA PARTIE **********************/

void afficherMenuParametragePartie(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu, StructAudio *audio)
{
    SDL_Rect contourSelection;
    contourSelection.w = 300;
    contourSelection.h = 50;
    contourSelection.x = WIDTH / 2 - contourSelection.w / 2;
    contourSelection.y = 130;

    SDL_Rect flecheGauche;
    flecheGauche.w = 40;
    flecheGauche.h = 40;
    flecheGauche.x = contourSelection.x - flecheGauche.w - 10;
    flecheGauche.y = contourSelection.y + 5;

    SDL_Rect flecheDroite;
    flecheDroite.w = 40;
    flecheDroite.h = 40;
    flecheDroite.x = contourSelection.x + contourSelection.w + 10;
    flecheDroite.y = contourSelection.y + 5;

    int dejaUnJoueurHumainSelectionne = 0;


    //Permet de gérer le déplacement vertical dans le menu
    if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0){
        menu->positionCurseurY--;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != 3){
        menu->positionCurseurY++;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    //Permet de regarder si un joueur humain est déjà séléctionné ou non
    if(menu->paramPartie[1] == 2 || menu->paramPartie[2] == 2 || menu->paramPartie[3] == 2)
        dejaUnJoueurHumainSelectionne = 1;

    //Permet de gérer le déplacement horizontal dans le menu
    if(cycleToucheClavierRealise(&clavier->toucheGauche, clavier) ){
        if( (menu->paramPartie[menu->positionCurseurY] == 0 ) && dejaUnJoueurHumainSelectionne == 1) //Si on arrive en bout de séléction ( 0 ) et que un joueur humain est déja selectionné alors on va directement en 1 (vide)
            menu->paramPartie[menu->positionCurseurY] = 1;
        else
            menu->paramPartie[menu->positionCurseurY]--;

        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    if(cycleToucheClavierRealise(&clavier->toucheDroite, clavier) ){
        if( (menu->paramPartie[menu->positionCurseurY] == 1 ) && dejaUnJoueurHumainSelectionne == 1) //Si on arrive en bout de séléction ( 1 ) et que un joueur humain est déja selectionné alors on va directement en 0 (ordinateur)
            menu->paramPartie[menu->positionCurseurY] = 0;
        else
            menu->paramPartie[menu->positionCurseurY]++;

        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    //Remet le joueur au début des choix une fois arrivée au bout
    if(menu->paramPartie[menu->positionCurseurY] > 2)
        menu->paramPartie[menu->positionCurseurY] = 0;
      if(menu->paramPartie[menu->positionCurseurY] < 0)
        menu->paramPartie[menu->positionCurseurY] = 2;


    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    afficherTexte("PARAMETRAGE PARTIE", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 30, affichage->renderer);



    // Dessin des cases contenant les joueurs et remplissage PAR DEFAULT de celle-ci
    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);
    for(int i = 0; i < 4; i++)
    {
        SDL_RenderDrawRect(affichage->renderer, &contourSelection);

        if(i == 0)
            afficherTexte(menu->profilSelectionne.nom, 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, contourSelection.y + 5, affichage->renderer);
        else
        {
            if(menu->paramPartie[i] == 0)
                afficherTexte("Ordinateur", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, contourSelection.y + 5, affichage->renderer);
            else if(menu->paramPartie[i] == 1)
                afficherTexte("Vide", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, contourSelection.y + 5, affichage->renderer);
            else if(menu->paramPartie[i] == 2)
                afficherTexte("Humain", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, contourSelection.y + 5, affichage->renderer);
        }

        contourSelection.y += 80;
    }


    flecheGauche.y = flecheGauche.y + 80*menu->positionCurseurY;
    flecheDroite.y = flecheDroite.y + 80*menu->positionCurseurY;

    // Dessin flèches
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheGauche, NULL, &flecheGauche);
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheDroite, NULL, &flecheDroite);

    afficherTexte("ENTREE pour lancer la partie", 20, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 490, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);


    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        initLeJeuUneDeuxiemeFois(jeu, menu);
        menu->numeroFenetre = 6;
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        menu->numeroFenetre = 3;
    }
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = -1;


}

/******************** MENU 7 : MENU PAUSE **********************/

void afficherMenuPause(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu, StructAudio *audio)
{
    SDL_Rect rectAffichageFond;
    rectAffichageFond.w = 400;
    rectAffichageFond.h = 400;
    rectAffichageFond.x = WIDTH / 2 - rectAffichageFond.w / 2;
    rectAffichageFond.y = 100;

    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderFillRect(affichage->renderer, &rectAffichageFond);

    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(affichage->renderer, &rectAffichageFond);

    if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0){
        menu->positionCurseurY--;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
     if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != 1){
        menu->positionCurseurY++;
        lireUnSon(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }


    afficherTexte("* PAUSE *", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 150, affichage->renderer);

    if(menu->positionCurseurY == 0)
        afficherTexte("Redemarrer la partie", 25, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 300, affichage->renderer);
    else
        afficherTexte("Redemarrer la partie", 25, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 300, affichage->renderer);

    if(menu->positionCurseurY == 1)
        afficherTexte("Quitter la partie", 25, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 350, affichage->renderer);
    else
        afficherTexte("Quitter la partie", 25, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 350, affichage->renderer);




    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);


    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        switch(menu->positionCurseurY)
        {
        case 0:
            jeu->etat = LANCEMENT; //Le case 6 dans le menu correspond à jeu->LANCEMENT
            break;
        case 1:
            jeu->etat = EXTINCTION;
            break;
        }
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        jeu->etat = ON;
    }


}







/*************************************************************/
/*************FONCTIONS POUR SIMPLIFIER LE CODE **************/
/*************************************************************/


void enregistrerNouveauCompte(char nomCompte[])
{

    FILE *fic;
    CompteJoueur c;

    strcpy(c.nom, nomCompte);
    c.nbrVictoires = 0;
    c.nbrDefaites = 0;

    fic = fopen (CHEMIN_D_ACCES_FICHIER_COMPTES_JOUEURS, "a");
    if(fic == NULL)
    {
        printf("[ECRITURE] Impossible d'accéder à la liste des joueurs\n");
    }
    else
    {
        fwrite(&c, sizeof(c), 1, fic);
        printf("Ecriture réussit !");
    }
    fclose(fic);
}



int chargerComptes(CompteJoueur *tabComptes)
{

    CompteJoueur compte;
    int nbrDeComptes = 0;

    FILE *fic;
    fic = fopen (CHEMIN_D_ACCES_FICHIER_COMPTES_JOUEURS, "r");
    if(fic == NULL)
    {
        printf("Impossible d'accéder à la liste des joueurs\n");
        // fclose(fic); crée une erreur lorsque le fichier n'existe pas sinon
        return -1;
    }
    else
    {

        while(fread(&compte, sizeof(compte), 1, fic) && !feof(fic))
        {
            tabComptes[nbrDeComptes] = compte;
            nbrDeComptes++;
        }

        printf("Lecture réussit ! Il y a  %d profils enregistrés !\n", nbrDeComptes);
        fclose(fic);
        return nbrDeComptes;
    }

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

void initLeJeuUneDeuxiemeFois(StructJeu *jeu, StructMenu *menu)
{
    int nbrJoueursHumains = 0;
    int nbrJoueursIA = 0;

    CompteJoueur cDefault = {"Test", 0, 0};

    for(int i = 0; i < 4; i++)
    {
        if(menu->paramPartie[i] == 0)
            nbrJoueursIA++;
        if (menu->paramPartie[i] == 2)
            nbrJoueursHumains++;
    }

    for(int i = 0; i <= nbrJoueursHumains; i++)
    {
        if(i == 0)
            jeu->listeDesJoueurs[i].compte = menu->profilSelectionne;
        else
            jeu->listeDesJoueurs[i].compte = cDefault;

        jeu->listeDesJoueurs[i].humainOuIA = 0;
    }

    for(int i = nbrJoueursHumains; i < nbrJoueursHumains + nbrJoueursIA; i++)
        jeu->listeDesJoueurs[i].humainOuIA = 1;

    jeu->nbrDeJoueurs = nbrJoueursHumains + nbrJoueursIA;

}



