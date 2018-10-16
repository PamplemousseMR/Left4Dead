#ifndef __GETKEY_H__
#define __GETKEY_H__

#include <ncurses.h>            /* getch */

#include "update.h"
#include "move.h"

/* =============== Fonctions ============== */

void initGetkey();              /* la boucle qui attend les saisies clavier/souris */
short getSelectedId();

#endif
