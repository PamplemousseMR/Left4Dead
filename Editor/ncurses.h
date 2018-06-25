#ifndef __NCURSES_H__
#define __NCURSES_H__

#include <stdlib.h>                                     /* NULL */
#include <ncurses.h>                                    /* ncurses */
#include <string.h>                                     /* strlen */

#include "../Commun/map.h"

/* =============== Les tailles des fenêtres =============== */

#define TOOLS_LINE_BOX 3
#define TOOLS_COL_BOX (COLS*2/3)
#define SHOW_LINE_BOX (LINES-TOOLS_LINE_BOX)
#define SHOW_COL_BOX TOOLS_COL_BOX
#define BUTTON_LINE_BOX ((LINES/3)/2)
#define BUTTON_COL_BOX PROPERTIES_COL_BOX
#define LOG_LINE_BOX 3
#define LOG_COL_BOX PROPERTIES_COL_BOX
#define PROPERTIES_COL_BOX (COLS/3)
#define PROPERTIES_LINE_BOX (LINES-2*BUTTON_LINE_BOX-LOG_LINE_BOX)

#define TOOLS_LINE (TOOLS_LINE_BOX-2)                   /* nombre de colonnes de la fenêtre d'outils */
#define TOOLS_COL (TOOLS_COL_BOX-2)                     /* nombre de lignes de la fenêtre d'outils */
#define SHOW_LINE (SHOW_LINE_BOX-2)                     /* nombre de colonnes de la fenêtre d'affichage de la map */
#define SHOW_COL (SHOW_COL_BOX-2)                       /* nombre de lignes de la fenêtre de d'affichage de la map */
#define BUTTON_COL (BUTTON_COL_BOX-2)                   /* nombre de colonnes de la fenêtre des boutons */
#define BUTTON_LINE (BUTTON_LINE_BOX-2)                 /* nombre de lignes de la fenêtre des boutons */
#define LOG_COL (LOG_COL_BOX-2)                         /* nombre de colonnes de la fenêtre de log */
#define LOG_LINE (LOG_LINE_BOX-2)                       /* nombre de lignes de la fenêtre de log */
#define PROPERTIES_COL (PROPERTIES_COL_BOX-2)           /* nombre de colonnes de la fenêtre des propriétés */
#define PROPERTIES_LINE (PROPERTIES_LINE_BOX-2)         /* nombre de lignes de la fenêtre des propriétés */


/* =============== Les valeurs que PROPERTY peut prendre =============== */

#define PLAYER_MAX 0                                    /* nombre max de joueurs */
#define FOOD_SPAWN 1                                    /* valeur de spawn de la nourriture */
#define FOOD_COUNT 2                                    /* nombre de sacs de nouriture */
#define FOOD_ADD 3                                      /* nombre de nourritures par sac */
#define AMO_SPAWN 4                                     /* valeur du spawn des sacs de munitions */
#define AMO_COUNT 5                                     /* nombre de sacs de munitions */
#define AMO_ADD 6                                       /* valeur de chaque sac de munitions */
#define ZOMBIE_SPAWN 7                                  /* valeur de spawn des zombies */
#define ZOMBIE_MAX 8                                    /* nombre max de zombies */
#define MAP_HEIGHT 9                                    /* hauteur de la map */
#define MAP_WIDTH 10                                    /* largeur de la map */

/* =============== Les valeurs que TOOL peut prendre =============== */

#define ERASE_TOOL 0                                    /* outil effacer */
#define CEMETERY_TOOL 1                                 /* outil cimetière */
#define OBSTACLE_TOOL 2                                 /* outil obstacle */
#define HOME_TOOL 3                                     /* outil repère */
#define HIDEOUT_TOOL 4                                  /* outil cachette */

/* =============== Les valeurs que BUTTON peut prendre =============== */

#define SAVE 0                                          /* clic sur sauvegarder */
#define QUIT 1                                          /* clic sur quiter */

/* =============== Les fenêtres et les valeurs des boutons séléctionnés =============== */

extern WINDOW* TOOLS;                                   /* le fenêtre d'outils */
extern WINDOW* SHOW;                                    /* la fenêtre de la map */
extern WINDOW* PROPERTIES;                              /* la fenêtre des proprietés */
extern WINDOW* LOG;                                     /* les logs */
extern short TOOL;                                      /* l'outil selectionné */
extern short PROPERTIE;                                 /* la propriété séléctionée */
extern short BUTTON;                                    /* sauvegarde ou fin */

/* =============== Fonctions =============== */

void initNcurses();                                     /* initialise ncurses */
void endNcurses();                                      /* éteint ncurses */
void initWindows();                                     /* initalise les fenêtres */
void clearWindows();                                    /* éteint les fenêtres */
void drawTools(short,short);                            /* dessine les outils */
void drawProperties(unsigned short ,unsigned short      /* dessine les propriétés */
                    ,unsigned short ,unsigned short
                    ,unsigned short ,unsigned short
                    ,unsigned short ,unsigned short
                    ,unsigned short ,unsigned short
                    , unsigned short);
char getSelectedProperty(int, MEVENT*);                 /* la propriété selectionée */
char getSelectedTool(int,MEVENT*);                      /* l'outil selectionné */
char getSelectedTile(int,unsigned short*,               /* la case selectionnée dans la fenêtre */
                     unsigned short*,MEVENT*);
char getSelectedButton(int, MEVENT*);                   /* le bouton selectionné */

#endif
