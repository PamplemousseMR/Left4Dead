#ifndef __UDPDATA_H__
#define __UDPDATA_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>         /* Pour exit, EXIT_FAILURE, EXIT_SUCCESS */
#include <stdio.h>          /* Pour printf, fprintf, perror */
#include <sys/socket.h>     /* Pour socket, bind */
#include <arpa/inet.h>      /* Pour sockaddr_in */
#include <string.h>         /* Pour memset */
#include <unistd.h>         /* Pour close */
#include <errno.h>          /* Pour errno */

#include "option.h"
#include "connection.h"

#include "../commun/messagesStruct.h"

/* =============== Fonctions =============== */

void initUDPData();         /* la boucle principale qui attend des demandes de port en UDP */
void clearUDPData();        /* supprime la socket UDP */

#endif
