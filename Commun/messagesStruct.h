#ifndef __MESSAGESSTRUCT_H__
#define __MESSAGESSTRUCT_H__

#include <netinet/in.h>         /* sin_addr */

typedef int PLAYER_TYPE;
#define TYPE_ZOMBIE 1
#define TYPE_HUMAN 0

typedef struct
{
    unsigned int port;
}get_port_t;

typedef struct
{
    unsigned short id;
    char occupied;
}set_port_t;

typedef struct
{
    unsigned int port;
    PLAYER_TYPE type;
    unsigned short global_id;
}send_port_t;

typedef struct
{
    unsigned short width,height;
}map_size_t;

typedef struct
{
    unsigned short line,col;
    unsigned short life;
}zombie_t;

typedef struct
{
    unsigned short line,col;
    unsigned short life;
    unsigned short foods,ammos;
}human_t;

typedef struct
{
    unsigned short foods;
    unsigned short ammo;
}home_t;

#define ASK_MOVE 0
#define ASK_DISCONECT 1
#define ASK_EAT 2
#define ASK_GET 3
#define ASK_SET 4
typedef struct
{
    int type;
    unsigned short line,col;
    unsigned short id;
}ask_t;

#define COMMAND_MESSAGE 1
#define COMMAND_MOVE 0
#define COMMAND_ZOMBIE_YES 1
#define COMMAND_ZOMBIE_NO 2
#define COMMAND_HUMAN_YES 3
#define COMMAND_HUMAN_NO 4
#define COMMAND_EAT 5
#define COMMAND_GET 6
#define COMMAND_SET 7
typedef struct
{
    long type;
    unsigned short command_type;
    unsigned short line,col;
    unsigned short global_id;
    unsigned short local_id;
}command_t;

#endif
