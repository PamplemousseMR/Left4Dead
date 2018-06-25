#ifndef __IPS_H__
#define __IPS_H__

#define _XOPEN_SOURCE 199309L

#include <stdio.h>              /* printf, perror */
#include <stdlib.h>             /* exit */
#include <errno.h>              /* errno */
#include <sys/shm.h>            /* shmget, shmctl */
#include <sys/sem.h>            /* semget, semctl, semop */
#include <sys/msg.h>            /* msgget */

#include "option.h"

/* =============== Variables =============== */

extern int MSG;                 /* la file de messages */
extern int SHMID;               /* la mémoire partagée */
extern int SEM;                 /* les sémaphores */
extern unsigned short* MEM;     /* l'adresse de la memoire partagée */

/* =============== Fonctions =============== */

void initMessage();             /* récupère la file de messages */
void initMemory();              /* récupère la memoire partagée */
void initSemaphores();          /* récupère les semaphores */
void can(int);
void go(int);

#endif
