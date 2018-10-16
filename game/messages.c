#include "messages.h"

void receiveMessages()
{
    command_t command;
    while(1)
    {
        if(msgrcv(MSG, &command, sizeof(command_t) - sizeof(long), COMMAND_MESSAGE, 0) == -1)
            perror("[receiveMessages] Erreur lors de la reception d'une requete ");
        if(command.global_id==0)
        {
            if(command.command_type==COMMAND_MOVE)
                moveZombie(&command);
            else if(command.command_type==COMMAND_ZOMBIE_YES)
                moveAllonStart();
            else if(command.command_type==COMMAND_ZOMBIE_NO)
                moveAllonStop();
            else if(command.command_type==COMMAND_EAT)
                zombieEat(command.local_id);
            else
                printf("[receiveMessages] COMMAND_TYPE INCONNUE\n");
        }
        else
        {
            if(command.command_type==COMMAND_MOVE)
                moveHuman(&command);
            else if(command.command_type==COMMAND_HUMAN_YES)
                decreaseLifeStart(command.global_id-1);
            else if(command.command_type==COMMAND_HUMAN_NO)
                decreaseLifeStop(command.global_id-1);
            else if(command.command_type==COMMAND_EAT)
                humanEat(command.global_id-1,command.local_id);
            else if(command.command_type==COMMAND_GET)
            {
                command.global_id-=1;
                checkFoodsAndAmmos(&command,1);
            }
            else if(command.command_type==COMMAND_SET)
            {
                command.global_id-=1;
                setFoodAndAmmo(&command);
            }
            else
                printf("[receiveMessages] COMMAND_TYPE INCONNUE\n");
        }
    }
}
