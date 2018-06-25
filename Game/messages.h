#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#define _XOPEN_SOURCE 199309L

#include <sys/msg.h>                    /* msgrcv */

#include "IPC.h"
#include "zombie.h"
#include "human.h"

#include "../Commun/messagesStruct.h"

/* =============== Fonctions =============== */

void receiveMessages();                 /* Attend des messages dans la file de messages */

#endif
