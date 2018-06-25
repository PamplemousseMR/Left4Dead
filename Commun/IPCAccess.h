#ifndef __IPCACCESS_H__
#define __IPCACCESS_H__

#include "map.h"

#define GET_HEIGHT MEM[0]
#define GET_WIDTH MEM[1]
#define GET_TILE(line,col) MEM[(line)*GET_WIDTH+(col)+2]
#define GET_MAX_PLAYER MEM[GET_HEIGHT*GET_WIDTH+2]
#define GET_MAX_ZOMBIE MEM[GET_HEIGHT*GET_WIDTH+3]

#define GET_ZOMBIE_LINE(x) MEM[GET_HEIGHT*GET_WIDTH+4+((x)*3)]
#define GET_ZOMBIE_COL(x) MEM[GET_HEIGHT*GET_WIDTH+5+((x)*3)]
#define GET_ZOMBIE_LIFE(x) MEM[GET_HEIGHT*GET_WIDTH+6+((x)*3)]

#define GET_HUMAN_LINE(global,local) MEM[GET_HEIGHT*GET_WIDTH+5+(3*GET_MAX_ZOMBIE)+((global)*HUMAN_PER_PLAYER*5)+((local)*5)]
#define GET_HUMAN_COL(global,local) MEM[GET_HEIGHT*GET_WIDTH+6+(3*GET_MAX_ZOMBIE)+((global)*HUMAN_PER_PLAYER*5)+((local)*5)]
#define GET_HUMAN_LIFE(global,local) MEM[GET_HEIGHT*GET_WIDTH+7+(3*GET_MAX_ZOMBIE)+((global)*HUMAN_PER_PLAYER*5)+((local)*5)]
#define GET_HUMAN_FOODS(global,local) MEM[GET_HEIGHT*GET_WIDTH+8+(3*GET_MAX_ZOMBIE)+((global)*HUMAN_PER_PLAYER*5)+((local)*5)]
#define GET_HUMAN_AMMOS(global,local) MEM[GET_HEIGHT*GET_WIDTH+9+(3*GET_MAX_ZOMBIE)+((global)*HUMAN_PER_PLAYER*5)+((local)*5)]

#define GET_HOME_FOOD(global) MEM[GET_HEIGHT*GET_WIDTH+(3*GET_MAX_ZOMBIE)+(5*HUMAN_PER_PLAYER*5)+6+((global)*2)]
#define GET_HOME_AMMO(global) MEM[GET_HEIGHT*GET_WIDTH+(3*GET_MAX_ZOMBIE)+(5*HUMAN_PER_PLAYER*5)+7+((global)*2)]

#define SEM_NUMBER (5+GET_MAX_PLAYER+(GET_MAX_PLAYER*2))
#define CAN_TILE(line,col) ( ( (line)>(GET_HEIGHT/2) ? 2:0 ) + ( (col)>(GET_WIDTH/2) ? 1:0 ) )
#define CAN_ZOMBIE 4
#define CAN_HUMAN(X) (5+(X))
#define CAN_HOME(X) (5+GET_MAX_PLAYER+(X))

#endif
