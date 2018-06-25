#define _XOPEN_SOURCE 199309L

#include <stdio.h>	/* printf, perror */
#include <stdlib.h>	/* exit */

#include "option.h"
#include "signal.h"
#include "connection.h"
#include "ncurses.h"
#include "update.h"
#include "getkey.h"
#include "move.h"

/* =============== Fin du programme =============== */

static void clearProg()
{
    clearMove();
    clearUpdate();
    clearWindows();
    endNcurses();
    clearConnections();
    _exit(EXIT_SUCCESS);
}

/* =============== Main =============== */

int main(int argc,char* argv[])
{
    /* récupération des options */
    getOption(argc, argv);

    if(atexit(clearProg) != 0)
    {
        perror("[main] probleme lors de l'enregistrement du nettoyage ");
        exit(EXIT_FAILURE);
    }

    /* initialiser le programme */
    initSignal();
    initConnections();

    /* initialise ncurses */
    initNcurses();
    initColor();
    initWindows();

    /* lance le programme */
    initUpdate();
    initMove();
    initGetkey();

    return EXIT_SUCCESS;
}
