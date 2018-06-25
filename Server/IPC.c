#include "IPC.h"

int MSG = -1;
int SHMID = -1;
int SEM = -1;
unsigned short* MEM = NULL;

void initMessage()
{
    if((MSG = msgget(MSG_KEY, 0)) == -1)
    {
        perror("[initMessage] Erreur lors de la recuperation de la file");
        exit(EXIT_FAILURE);
    }
    printf("[initMessage] File de messages recuperee\n");
}

void initMemory()
{
    if((SHMID = shmget(MEM_KEY, 0,0)) == -1)
    {
        perror("[initMemory] Erreur lors de la recuperation du segment de memoire");
        exit(EXIT_FAILURE);
    }

    if((MEM = (unsigned short*)shmat(SHMID, NULL, 0)) == (void*)-1)
    {
        perror("[initMemory] Erreur lors de l'attachement du segment de memoire partagee");
        exit(EXIT_FAILURE);
    }

    printf("[initMemory] Memoire partagee recupere\n");
}

void initSemaphores()
{
    if((SEM = semget(SEM_KEY, 0,0)) == -1)
    {
        perror("[initSemaphores] Erreur lors de la recuperation du tableau de semaphores");
        exit(EXIT_FAILURE);
    }

    printf("[initSemaphores] Semaphores recuperes\n");
}


void can(int id)
{
    /* puis-je */
    struct sembuf op;
    op.sem_num = id;
    op.sem_op = -1;
    op.sem_flg = 0;
    if(semop(SEM, &op, 1) == -1)
    {
        perror("[can] Erreur lors de l'operation sur le semaphore ");
        exit(EXIT_FAILURE);
    }
}

void go(int id)
{
    /* vas-y */
    struct sembuf op;
    op.sem_num = id;
    op.sem_op = 1;
    op.sem_flg = 0;
    if(semop(SEM, &op, 1) == -1)
    {
        perror("[go] Erreur lors de l'operation sur le semaphore ");
        exit(EXIT_FAILURE);
    }
}
