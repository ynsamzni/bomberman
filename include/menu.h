#ifndef MENU_INCLUDED
#define MENU_INCLUDED


typedef struct{
    int position;
    char entreeTexte[];
}StructMenu;



void initMenu(StructMenu *menu);



#endif // AFFICHAGE_INCLUDED

