#define _XOPEN_SOURCE 199309L

#include <stdio.h>	/* printf, perror */
#include <stdlib.h>	/* exit */

#include "option.h"
#include "signal.h"
#include "IPC.h"
#include "connection.h"
#include "UDPData.h"

/* =============== Fin du programme =============== */

static void clearProg()
{
    clearConnections();
    clearUDPData();
    _exit(EXIT_SUCCESS);
}

/* =============== Main =============== */

int main(int argc,char* argv[])
{
    /* récupération des options */
    getOption(argc, argv);

    /* fonction lors de l'arrêt du programme */
    if(atexit(clearProg) != 0)
    {
        perror("[main] probleme lors de l'enregistrement du netoyage des IPC");
        exit(EXIT_FAILURE);
    }

    /* initialisation des IPC */
    initSignal();
    initMessage();
    initMemory();
    initSemaphores();

    /* crée les fils et les socket TCP */
    initConnections();
    /* la boucle qui attend les requêtes UDP */
    initUDPData();

    return EXIT_SUCCESS;
}
