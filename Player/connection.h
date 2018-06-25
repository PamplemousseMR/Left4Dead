#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#define _XOPEN_SOURCE 199309L

#include <stdio.h>                              /* printf */
#include <pthread.h>                            /* thread */
#include <stdlib.h>                             /* EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h>                         /* socket */
#include <arpa/inet.h>                          /* sockaddr_in, IPPROTO_TCP */
#include <stdio.h>                              /* perror */
#include <unistd.h>                             /* close */
#include <string.h>                             /* memset,strncoy */

#include "option.h"

#include "../Commun/IPCAccess.h"
#include "../Commun/messagesStruct.h"

/* =============== Fonctions =============== */

void initConnections();                         /* se connecte au serveur en TCP */
void clearConnections();                        /* supprime la connexion TCP */
void readSock(void*,size_t);
void writeSock(void*,size_t);
PLAYER_TYPE getType();
unsigned short getGlobalId();

#endif
