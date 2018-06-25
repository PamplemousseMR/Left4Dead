#define _XOPEN_SOURCE 199309L

#include <stdio.h>	/* printf, perror */
#include <stdlib.h>	/* exit */
#include <time.h>       /* time */

#include "option.h"
#include "signal.h"
#include "IPC.h"
#include "items.h"
#include "zombie.h"
#include "messages.h"
#include "human.h"

#include "../Commun/map.h"

/* =============== Variables =============== */

static map_t* map = NULL;

/* =============== Fin du programme =============== */

static void clearProg()
{
    clearZombies();
    clearHuman();
    clearItems();
    clearSemaphores();
    clearMemory();
    clearMessage();
    if(map != NULL)deleteMap(map);
    _exit(EXIT_SUCCESS);
}

/* =============== Main =============== */

int main(int argc,char* argv[])
{
    short error;
    /* récupération des options */
    getOption(argc, argv);

    /* si la variable CLEAR est a 1, vider les IPC */
    if(CLEAR)
    {
        deleteIPC();
        return EXIT_SUCCESS;
    }

    /* charger la carte */
    map = loadMap(FILE_NAME,&error);
    if(error!=NO_ERROR)
    {
        fprintf(stderr,"[main] la map ne peut pas etre chargee : error %d\n", error);
        exit(EXIT_FAILURE);
    }

    /* vérifier la carte */
    error = checkMap(map);
    if(error!=NO_ERROR)
    {
        fprintf(stderr,"[main] la map n'est pas correcte : error %d\n", error);
        deleteMap(map);
        exit(EXIT_FAILURE);
    }

    /* mise en place de la fonction lors de l'arrêt du programme */
    if(atexit(clearProg) != 0)
    {
        perror("[main] probleme lors de l'enregistrement du nettoyage ");
        exit(EXIT_FAILURE);
    }

    /* la graine pour l'aleatoire */
    srand(time(NULL));

    /* initialiser le programme */
    initSignal();
    initMessage();
    initMemory(map);
    initSemaphores();

    /* la gestion des zombies et des humains */
    initZombies(map);
    initHuman(map);

    /* les threads de création de nourriture et de munitions */
    initItems(map);

    /* Les réceptions de messages */
    receiveMessages();

    return EXIT_SUCCESS;
}
