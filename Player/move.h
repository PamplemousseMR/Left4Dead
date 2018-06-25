#ifndef __MOVE_H__
#define __MOVE_H__

#define _XOPEN_SOURCE 199309L

#include <pthread.h>                            /* thread */
#include <time.h>                               /* nanosleep */

#include "Astar/astar.h"
#include "Astar/list.h"
#include "update.h"

#include "../Commun/default.h"

typedef struct
{
    short id;
    node_t end;
}data_t;

void initMove();
void clearMove();
void entityMove(short,node_t);

#endif
