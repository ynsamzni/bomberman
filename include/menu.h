#ifndef MENU_INCLUDED
#define MENU_INCLUDED


#include "affichage.h"
#include "jeu.h"

/******GUIDE DU MENU*********

Menu 0 : Page d'accueil, ou il est juste écrit bomberman, une photo sympa, une petite musique et "appuyer sur entrée pour continuer"
Menu 1 : Menu demandant de sélectionner un profil et/ou d'en créer un
Menu 2 : Menu de création de profil
Menu 3 : Menu disant Bienvenue <nom du profil> et proposant soit de consulter les statistiques, soit de lancer une partie
Menu 4 : Menu affichant les statisqtiques
Menu 5 : Menu permettant de parmatrétrer sa partie
Menu 6 : Le jeu

****************************/



typedef struct
{
    int numeroFenetre;
    int lastNumeroFenetre; //enregistre la fenêtre précédente et permet ainsi de savoir que l'on à changé pour pouvoir réinitialiser les curseurs
    int positionCurseurY;
    int positionCurseurX;
    CompteJoueur profilSelectionne;
    int variableCouleur;
    int tickAttente;
    int paramPartie[4];
    char tabNomDuJoueur[8];

} StructMenu;





void initMenu(StructMenu *menu);
void afficherTexte(char texte[], int tailleTexte, SDL_Color couleurTexte, char cheminPoliceEcriture[], int positionX, int positionY, SDL_Renderer *renderer);

void afficherMenuAccueil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu);
void afficherMenuSelectionProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu);
void afficherMenuCreationProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu);
void afficherMenuPrincipal(StructAffichage *affichage, StructTouchesClavier *clavier, StructMenu *menu);
void afficherMenuStatistiques(StructAffichage *affichage, StructTouchesClavier *clavier, StructMenu *menu);
void afficherMenuParametragePartie(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu);

void enregistrerNouveauCompte(char nomCompte[]);
int chargerComptes(CompteJoueur *tabComptes);
void initLeJeuUneDeuxiemeFois(StructJeu *jeu, StructMenu *menu);

void gestionDuMenu(StructMenu *menu, StructJeu *jeu, StructTouchesClavier *clavier, StructAffichage *affichage);


#endif // AFFICHAGE_INCLUDED

