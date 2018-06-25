#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>                                                     /* rand */
#include <pthread.h>                                                    /* thread */
#include <stdlib.h>                                                     /* NULL */
#include <stdio.h>                                                      /* printf */

#include "IPC.h"
#include "human.h"

#include "../Commun/messagesStruct.h"
#include "../Commun/IPCAccess.h"
#include "../Commun/map.h"

/* =============== Defines =============== */

#define ZOMBIES_ARRAY(line,col) zombies_array[(line)*GET_WIDTH+(col)]   /* pour accéder à une case de zombies_array */

/* =============== Fonctions =============== */

void initZombies(map_t*);                                               /* initialise la création des zombies */
void clearZombies();                                                    /* supprime les threads et la mémoire */
void moveZombie(command_t*);                                            /* déplace un zombie */
char noZombie(unsigned short,unsigned short);                           /* renvoie 1 s'il n'y a pas de zombie à la case specifiée */
void moveAllonStart();
void moveAllonStop();
void checkHumainAround(unsigned short);
void zombieEat(unsigned short);

#endif
