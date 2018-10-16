#ifndef __NCURSES_H__
#define __NCURSES_H__

#include <stdlib.h>                                         /* NULL */
#include <ncurses.h>                                        /* ncurses */
#include <string.h>                                         /* strlen */

#include "getkey.h"

#include "../commun/messagesStruct.h"
#include "../commun/map.h"

/* =============== Les tailles des fenêtres =============== */

#define SHOW_LINE_BOX LINES
#define SHOW_COL_BOX (COLS*3/5)
#define PLAYER_LINE_BOX (LINES-3)
#define PLAYER_COL_BOX (COLS-SHOW_COL_BOX)
#define BUTTON_LINE_BOX (LINES-SHOW_LINE_BOX)
#define BUTTON_COL_BOX (PLAYER_COL_BOX)

#define SHOW_LINE (SHOW_LINE_BOX-2)                         /* nombre de lignes de la fenêtre d'affichage de la map */
#define SHOW_COL (SHOW_COL_BOX-2)                           /* nombre de colones de la fenêtre de d'affichage de la map */
#define PLAYER_LINE (PLAYER_LINE_BOX-2)
#define PLAYER_COL (PLAYER_COL_BOX-2)

/* =============== Les fenêtres =============== */

extern WINDOW* SHOW;                                        /* la fenêtre de la map */
extern WINDOW* PLAYER;

/* =============== Fonctions =============== */

void initNcurses();                                         /* initialise ncurses */
void endNcurses();                                          /* éteint ncurses */
void initWindows();                                         /* initalise les fenêtres */
void clearWindows();                                        /* éteint les fenêtres */
void drawZombies(zombie_t*,unsigned short);                 /* dessine les zombie dans la fenetre d'informations */
void drawHumans(human_t*,unsigned short,unsigned short);    /* dessine les humains dans la fenetre d'informations */
short getClickId(MEVENT*);
char getClickEat(MEVENT*);
char getClickGet(MEVENT*);
char getClickSet(MEVENT*);

#endif
