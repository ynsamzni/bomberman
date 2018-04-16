#ifndef MENU_H
#define MENU_H

#include "affichage.h"
#include "jeu.h"
#include "audio.h"


typedef struct {
    int numeroFenetre;
    int dernierNumeroFenetre;
    int positionCurseurY;
    int positionCurseurX;
    CompteJoueur profilSelectionne;
    int paramPartie[4];
    char tabNomDuJoueur[8];
} StructMenu;

void initMenu(StructMenu *menu);
void afficherMenu(StructMenu *menu, StructJeu *jeu, StructClavier *clavier, StructAffichage *affichage, StructAudio *audio);
void afficherMenuAccueil(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu, StructAudio *audio);
void afficherMenuSelectionProfil(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu, StructAudio *audio);
void afficherMenuCreationProfil(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu, StructAudio *audio);
void afficherMenuPrincipal(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu, StructAudio *audio);
void afficherMenuStatistiques(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu);
void afficherMenuParametragePartie(StructAffichage *affichage, StructClavier *clavier, StructJeu *jeu, StructMenu *menu, StructAudio *audio);
void afficherMenuPause(StructAffichage *affichage, StructClavier *clavier, StructJeu *jeu, StructMenu *menu, StructAudio *audio);
void enregistrerNouveauCompte(char nomCompte[]);
int chargerComptes(CompteJoueur *tabComptes);
void appliquerParametragePartie(StructJeu *jeu, StructMenu *menu);


#endif // MENU_H
