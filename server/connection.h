#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#define _XOPEN_SOURCE 199309L

#include <stdio.h>                              /* printf */
#include <stdlib.h>                             /* EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h>                         /* socket */
#include <arpa/inet.h>                          /* sockaddr_in, IPPROTO_TCP */
#include <stdio.h>                              /* perror */
#include <string.h>                             /* memset */
#include <unistd.h>                             /* Pour fork */
#include <fcntl.h>                              /* open */
#include <sys/wait.h>                           /* Pour wait */
#include <sys/types.h>                          /* Pour pid_t */
#include <signal.h>                             /* sigXXX */
#include <sys/stat.h>                           /* Pour mkfifo */
#include <pthread.h>                            /* threads */
#include <time.h>                               /* nanosleep */
#include <sys/msg.h>                            /* msgsnd, msgrcv */
#include <time.h>                               /* nanosleep */

#include "option.h"
#include "IPC.h"

#include "../commun/IPCAccess.h"
#include "../commun/messagesStruct.h"
#include "../commun/default.h"

/* =============== Fonctions =============== */

void initConnections();                         /* initialise les fils qui attendent les connexions TCP */
void clearConnections();                        /* supprime les fils précédemment crées */
send_port_t getPort();                          /* renvoie le prochain port libre pour un connexion TCP */

#endif
