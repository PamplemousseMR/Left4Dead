#ifndef __OPTION_H__
#define __OPTION_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>                     /* exit */
#include <stdio.h>                      /* fprintf */
#include <unistd.h>                     /* getopt */
#include <arpa/inet.h>                  /* inet_pton */
#include "../commun/default.h"

/* =============== Variables =============== */

extern int PORT_UDP_SERVER;             /* le port UDP du serveur */
extern int PORT_UDP_PLAYER;             /* le port UDP du programme */
extern struct in_addr ADD_SERVER;       /* l'adresse du seveur */

/* =============== Fonctions =============== */

void getOption(int argc,char* argv[]);  /* enregistre les options des les variables 'extern' */

#endif
