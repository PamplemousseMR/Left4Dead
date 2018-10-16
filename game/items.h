#ifndef __ITEMS_H__
#define __ITEMS_H__

#define _XOPEN_SOURCE 199309L

#include <pthread.h>                                /* thread */
#include <stdlib.h>                                 /* rand */
#include <sys/sem.h>                                /* semop */

#include "IPC.h"

#include "../commun/map.h"
#include "../commun/IPCAccess.h"

/* =============== Defines ===============*/

#define FOODS(line,col) foods[line*GET_WIDTH+col]   /* pour accéder à une case de nourriture */
#define AMMOS(line,col) ammos[line*GET_WIDTH+col]   /* pour accéder à une case de munitions */

/* =============== Structures =============== */

typedef struct                                      /* structure à donner aux threads */
{
   unsigned short spawn;
   unsigned short count;
   unsigned short add;
} items_value_t;

/* =============== Fonctions =============== */

void initItems(map_t*);                             /* boucle principale qui lance les threads de mise à jour */
void clearItems();
unsigned short getAmmo(unsigned short,              /* récupérer des munitions dans un sac */
                       unsigned short,
                       unsigned short);
unsigned short getFood(unsigned short,              /* récupérer de la nouriture dans un sac */
                       unsigned short,
                       unsigned short);

#endif
