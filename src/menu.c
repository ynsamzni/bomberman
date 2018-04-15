#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/menu.h"
#include "../include/affichage.h"
#include "../include/clavier.h"


void initMenu(StructMenu *menu)
{
    // Initialiser le système de numérotation de fenêtres
    menu->numeroFenetre = 0;
    menu->dernierNumeroFenetre = menu->numeroFenetre;
}

void gestionDuMenu(StructMenu *menu, StructJeu *jeu, StructTouchesClavier *clavier, StructAffichage *affichage, StructAudio *audio)
{
    // S'il y'a changement de fenêtre
    if(menu->numeroFenetre != menu->dernierNumeroFenetre)
    {
        // Réinitialiser les curseurs
        menu->positionCurseurY = 0;
        menu->positionCurseurX = 0;

        // Cas spécifiques
        if(menu->numeroFenetre == 3)
            // Placer le curseur sur le bouton jouer
            menu->positionCurseurY = 1;
        else if(menu->numeroFenetre == 2)
        {
            // Afficher un champ de texte vide
            menu->tabNomDuJoueur[0] = 'A';
            menu->tabNomDuJoueur[1] = '\0';
        }
        else if(menu->numeroFenetre == 5)
        {
            // Afficher les paramètres par défaut d'une partie (0 = Ordinateur / 1 = Vide / 2 = Humain)
            menu->paramPartie[0] = 2;
            menu->paramPartie[1] = 0;
            menu->paramPartie[2] = 1;
            menu->paramPartie[3] = 1;
        }

        // Actualiser la numérotation de la dernière fenêtre visitée
        menu->dernierNumeroFenetre = menu->numeroFenetre;
    }

    // Afficher le menu
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
    afficherTexte("* BOMBERMAN *", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.bombe, NULL, &rectAffichage);
    rectAffichage.w = 60;
    rectAffichage.h = 60;
    rectAffichage.x = 297;
    rectAffichage.y = 132;
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.etincelle, NULL, &rectAffichage);
    afficherTexte("ENTREE pour demarrer", 20, color, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 450, affichage->renderer);

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
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0){
        menu->positionCurseurY--;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("SELECTION DU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    if(chargerComptes(tabComptes) != -1) //ON VERIFIE QUE LE FICHIER DES PROFILS NEST PAS VIDE
    {
        for(int i=0; i<nbTotalProfils; i++)
        {
            if(menu->positionCurseurY == i)
                afficherTexte(tabComptes[i].nom, 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * i, affichage->renderer);
            else
                afficherTexte(tabComptes[i].nom, 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * i, affichage->renderer);
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
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
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
    int nbMaxLettres = 8;

    SDL_Rect caseLettre;
    caseLettre.w = 55;
    caseLettre.h = 50;
    caseLettre.x = WIDTH / 2 - (caseLettre.w / 2) * nbMaxLettres;
    caseLettre.y = 170;

    SDL_Rect lettre = {caseLettre.x + 5, caseLettre.y - 5, 0, 0};
    SDL_Rect flecheHaute = {caseLettre.x + 5, caseLettre.y - 30, 40, 30};
    SDL_Rect flecheBasse = {caseLettre.x + 5, caseLettre.y + caseLettre.h, 40, 30};

    // Si l'utilisateur appuie sur une touche : Effectuer l'action associée
    if(cycleToucheClavierRealise(&clavier->toucheDroite, clavier) && menu->positionCurseurX != nbMaxLettres - 1)
    {
        // Si la dernière lettre est sélectionnée : Ajouter une nouvelle lettre
        if(menu->tabNomDuJoueur[menu->positionCurseurX + 1] == '\0')
        {
            menu->tabNomDuJoueur[menu->positionCurseurX + 1] = 'A';
            menu->tabNomDuJoueur[menu->positionCurseurX + 2] = '\0';
        }

        // Effectuer le déplacement
        menu->positionCurseurX++;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheGauche, clavier) && menu->positionCurseurX != 0)
    {
        menu->positionCurseurX--;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->tabNomDuJoueur[menu->positionCurseurX] != 'A')
    {
        menu->tabNomDuJoueur[menu->positionCurseurX]--; // Décrémente le caractère ASCII de 1
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->tabNomDuJoueur[menu->positionCurseurX] != 'Z')
    {
        menu->tabNomDuJoueur[menu->positionCurseurX]++; // Incrémente le caractère ASCII de 1
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheSupprimer, clavier) &&  menu->positionCurseurX != 0)
    {
        // Si la dernière lettre est sélectionnée : La supprimer
        if(menu->tabNomDuJoueur[menu->positionCurseurX + 1] == '\0')
        {
            menu->tabNomDuJoueur[menu->positionCurseurX] = '\0';
            menu->positionCurseurX--;
            lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
        }
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Définir la couleur utilisée pour les opérations de dessin
    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);

    // Copier les éléments du menu dans le renderer
    afficherTexte("NOUVEAU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    for(int i=0; i<nbMaxLettres; i++)
    {
        // Cases contenant les lettres du nom
        SDL_RenderDrawRect(affichage->renderer, &caseLettre);

        // Nom
        if(i < strlen(menu->tabNomDuJoueur))
        {
            caractereActuel = menu->tabNomDuJoueur[i];
            afficherTexte(&caractereActuel, 45, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, lettre.x, lettre.y, affichage->renderer);
        }

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

    afficherTexte("ENTREE pour continuer", 20, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 310, affichage->renderer);

    if(menu->tabNomDuJoueur[menu->positionCurseurX + 1] == '\0')
    {
        // Si le curseur ne se trouve ni sur le premier si sur le dernier emplacement texte
        if(0 < menu->positionCurseurX && menu->positionCurseurX < nbMaxLettres - 1)
            afficherTexte("DROITE pour ajouter une lettre | RETOUR ARRIERE pour supprimer une lettre", 14, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 345, affichage->renderer);
        // Si le curseur ne se trouve pas sur le dernier emplacement texte
        else if(menu->positionCurseurX < nbMaxLettres - 1)
            afficherTexte("DROITE pour ajouter une lettre", 14, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 345, affichage->renderer);
        // Si le curseur ne se trouve pas sur le premier emplacement texte
        else if(0 < menu->positionCurseurX)
            afficherTexte("RETOUR ARRIERE pour supprimer une lettre", 14, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 345, affichage->renderer);
    }

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
        enregistrerNouveauCompte(menu->tabNomDuJoueur);
        menu->numeroFenetre = 1;
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
        menu->numeroFenetre = 1;
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
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0){
        menu->positionCurseurY--;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer

    strcat(chaineBienvenue,"BIENVENUE ");
    strcat(chaineBienvenue, menu->profilSelectionne.nom);
    afficherTexte(chaineBienvenue, 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    if(menu->positionCurseurY == 0)
        afficherTexte("Statistiques", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170, affichage->renderer);
    else
        afficherTexte("Statistiques", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170, affichage->renderer);

    if(menu->positionCurseurY == 1)
        afficherTexte("Lancer une partie", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 220, affichage->renderer);
    else
        afficherTexte("Lancer une partie", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 220, affichage->renderer);

    if(menu->positionCurseurY == 2)
        afficherTexte("Quitter", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 270, affichage->renderer);
    else
        afficherTexte("Quitter", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 270, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
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


    afficherTexte("STATISTIQUES", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

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
        afficherTexte(tabComptes[i].nom, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 80, (i+1)*50 + 120, affichage->renderer);

        afficherTexte(chaine, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 250, (i+1)*50 + 120, affichage->renderer);
        sprintf(chaine, "Defaites : %d", tabComptes[i].nbrDefaites);

        afficherTexte(chaine, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 400, (i+1)*50 + 120, affichage->renderer);
        i++;
    }

    afficherTexte("ECHAP pour retourner au menu", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, i*50 + 270, affichage->renderer);

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
    contourSelection.y = 170;

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
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != 3){
        menu->positionCurseurY++;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
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

        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    if(cycleToucheClavierRealise(&clavier->toucheDroite, clavier) ){
        if( (menu->paramPartie[menu->positionCurseurY] == 1 ) && dejaUnJoueurHumainSelectionne == 1) //Si on arrive en bout de séléction ( 1 ) et que un joueur humain est déja selectionné alors on va directement en 0 (ordinateur)
            menu->paramPartie[menu->positionCurseurY] = 0;
        else
            menu->paramPartie[menu->positionCurseurY]++;

        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    //Remet le joueur au début des choix une fois arrivée au bout
    if(menu->paramPartie[menu->positionCurseurY] > 2)
        menu->paramPartie[menu->positionCurseurY] = 0;
      if(menu->paramPartie[menu->positionCurseurY] < 0)
        menu->paramPartie[menu->positionCurseurY] = 2;


    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    afficherTexte("PARAMETRAGE PARTIE", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);



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

    afficherTexte("ENTREE pour lancer la partie", 20, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 510, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);


    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
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
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
     if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != 1){
        menu->positionCurseurY++;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }


    afficherTexte("* PAUSE *", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 150, affichage->renderer);

    if(menu->positionCurseurY == 0)
        afficherTexte("Redemarrer la partie", 25, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 275, affichage->renderer);
    else
        afficherTexte("Redemarrer la partie", 25, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 275, affichage->renderer);

    if(menu->positionCurseurY == 1)
        afficherTexte("Quitter la partie", 25, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 325, affichage->renderer);
    else
        afficherTexte("Quitter la partie", 25, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 325, affichage->renderer);




    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);


    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
        switch(menu->positionCurseurY)
        {
        case 0:
            jeu->etat = LANCEMENT; //Le case 6 dans le menu correspond à jeu->LANCEMENT
            break;
        case 1:
            jeu->etat = OFF;
            menu->numeroFenetre = 3;
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
        printf("[ECRITURE] Impossible d'accéder à la liste des joueurs\n");
    else
        fwrite(&c, sizeof(c), 1, fic);

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
        return -1;
    }
    else
    {

        while(fread(&compte, sizeof(compte), 1, fic) && !feof(fic))
        {
            tabComptes[nbrDeComptes] = compte;
            nbrDeComptes++;
        }
        fclose(fic);
        return nbrDeComptes;
    }
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
