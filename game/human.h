#ifndef __HUMAN_H__
#define __HUMAN_H__

#define _XOPEN_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>                                                    /* mutex */

#include "IPC.h"
#include "zombie.h"
#include "items.h"

#include "../commun/IPCAccess.h"
#include "../commun/map.h"
#include "../commun/messagesStruct.h"

#define HUMANS_ARRAY(line,col) humans_array[(line)*GET_WIDTH+(col)]     /* pour accéder à une case de human_array */

void initHuman(map_t*);
void clearHuman();
void moveHuman(command_t*);
char noHuman(unsigned short,unsigned short);
char getHumain(unsigned short,unsigned short,unsigned short*, unsigned short*);
void killHuman(unsigned short,unsigned short);
void decreaseLifeStop(unsigned short);
void decreaseLifeStart(unsigned short);
void humanEat(unsigned short,unsigned short);
void checkFoodsAndAmmos(command_t*,char);
void setFoodAndAmmo(command_t*);

#endif
