#ifndef __IPS_H__
#define __IPS_H__

#define _XOPEN_SOURCE 199309L

#include <stdio.h>              /* printf, perror */
#include <stdlib.h>             /* exit */
#include <sys/stat.h>           /* S_IRUSR */
#include <errno.h>              /* errno */
#include <sys/shm.h>            /* shmget, shmctl */
#include <sys/sem.h>            /* semget, semctl, semop */
#include <sys/msg.h>            /* msgget */

#include "option.h"

#include "../commun/map.h"
#include "../commun/IPCAccess.h"

/* =============== Variables =============== */

extern int MSG;                 /* La file de messages */
extern int SHMID;               /* la mémoire partagee */
extern int SEM;                 /* les sémaphores */
extern unsigned short* MEM;     /* l'adresse de la mémoire partagée */

/* =============== Fonctions =============== */

void initMessage();             /* crée la file de messages */
void clearMessage();            /* supprime la file de messages */
void initMemory(map_t*);        /* crée la memoire partagée */
void clearMemory();             /* supprime la memoire partagée */
void initSemaphores();          /* crée les sémaphores */
void clearSemaphores();         /* supprime les sémaphores */
void deleteIPC();               /* supprime tous les outils IPC */
void can(int);                  /* puis-je pour un sémaphore */
void go(int);                   /* vas-y pour un sémaphore */

#endif
