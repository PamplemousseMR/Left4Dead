#ifndef __OPTION_H__
#define __OPTION_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>                             /* exit */
#include <stdio.h>                              /* fprintf */
#include <unistd.h>                             /* getopt */
#include <sys/sem.h>                            /* key_t */
#include <string.h>                             /* strcpy */

#include "../Commun/default.h"

/* =============== Variables =============== */

extern key_t MSG_KEY;                           /* la clé de la file de messages */
extern key_t SEM_KEY;                           /* la clé du tableau de sémaphores */
extern key_t MEM_KEY;                           /* la clé du segment de mémoire partagée */
extern int PORT_TCP;                            /* le premier port de connexion */
extern int PORT_UDP;                            /* le premier port de connexion */
extern char PIPE_NAME[MAX_PIPE_NAME_SIZE];      /* le nom du tube */

/* =============== Fonctions =============== */

void getOption(int argc,char* argv[]);          /* enregistre les options des les variables 'extern' */

#endif
