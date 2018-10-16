#include "IPC.h"

int MSG = -1;
int SHMID = -1;
int SEM = -1;
unsigned short* MEM = NULL;

void initMessage()
{
    /* cree la file de messages */
    if((MSG = msgget(MSG_KEY, S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL)) == -1)
    {
        if(errno == EEXIST)
            fprintf(stderr, "[initMessage] file (cle=%d) existante\n", MSG_KEY);
        else
            perror("[initMessage] Erreur lors de la creation de la file");
        exit(EXIT_FAILURE);
    }
    printf("[initMessage] File de messages cree\n");
}

void clearMessage()
{
    /* recuperer la file et la supprimer */
    if(msgctl(MSG, IPC_RMID, 0) == -1)
        perror("[clearMessage] Erreur lors de la supression de la file ");
    else
        printf("[clearMessage] File de messages supprimee\n");
}

void initMemory(map_t* map)
{
    unsigned int i,j;
    int size = 2;                               /* longeur/largeur */
    size+=map->height*map->width;               /* la carte */
    size+=1;                                    /* nombre de joueurs */
    size+=1;                                    /* nombre de zombie */
    size+=map->zombie_max*3;                    /* zombie (line,col,life) */
    size+=map->player_max*HUMAN_PER_PLAYER*5;   /* humain (line,col,life,food,ammo) */
    size+=map->player_max*2;                    /* home   (food,ammo) */

    /* cree la memoire partagée */
    if((SHMID = shmget(MEM_KEY, sizeof(unsigned short)*size, S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL)) == -1)
    {
        if(errno == EEXIST)
            fprintf(stderr, "[initMemory] Le segment de memoire partagee (cle=%d) existe deja\n", MEM_KEY);
        else
            perror("[initMemory] Erreur lors de la creation du segment de memoire");
        exit(EXIT_FAILURE);
    }

    /* attache la memoire partgée */
    if((MEM = (unsigned short*)shmat(SHMID, NULL, 0)) == (void*)-1)
    {
        perror("[initMemory] Erreur lors de l'attachement du segment de memoire partagee");
        exit(EXIT_FAILURE);
    }

    /* remplir la memoire */
    GET_HEIGHT = map->height;
    GET_WIDTH = map->width;
    for(i=0 ; i<map->height ; ++i)
    {
        for(j=0 ; j<map->width ; ++j)
            GET_TILE(i,j) = map->map[i][j];
    }
    GET_MAX_PLAYER = map->player_max;
    GET_MAX_ZOMBIE = map->zombie_max;
    for(i=0 ; i<map->zombie_max ; ++i)
    {
        GET_ZOMBIE_LINE(i) = 1;
        GET_ZOMBIE_COL(i) = 1;
        GET_ZOMBIE_LIFE(i) = 0;
    }
    for(i=0 ; i<map->player_max ; ++i)
    {
        for(j=0 ; j<HUMAN_PER_PLAYER ; ++j)
        {
            GET_HUMAN_LINE(i,j)=0;
            GET_HUMAN_COL(i,j)=0;
            GET_HUMAN_LIFE(i,j)=0;
            GET_HUMAN_FOODS(i,j)=0;
            GET_HUMAN_AMMOS(i,j)=0;
        }
    }

    for(i=0 ; i<map->player_max ; ++i)
    {
        GET_HOME_FOOD(i) = HOME_DEFAULT_FOOD;
        GET_HOME_AMMO(i) = HOME_DEFAULT_AMMO;
    }

    printf("[initMemory] Memoire partagee cree\n");
}

void clearMemory()
{
    /* récupérer la memoire partagée et la supprimer */
    if(shmctl(SHMID, IPC_RMID, 0) == -1)
        perror("[clearMemory] Erreur lors de la suppression du segment de memoire partagee");
    else
        printf("[clearMemory] Memoire partagee supprimee\n");
}

void initSemaphores()
{
    int i;
    unsigned short* init;
    init = (unsigned short*)malloc(sizeof(unsigned short)*SEM_NUMBER);
    if(init==0)
    {
        fprintf(stderr,"[initItems] Erreur lors de l'allocation dynamique de 'foods'\n");
        exit(EXIT_FAILURE);
    }
    for(i=0 ; i<SEM_NUMBER ; ++i)
        init[i] = 1;

    /* Création du tableau de sémaphores */
    if((SEM = semget(SEM_KEY, SEM_NUMBER, S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL)) == -1)
    {
        if(errno == EEXIST)
            fprintf(stderr, "[initSemaphores] Tableau de semaphores (cle=%d) existant\n", SEM_KEY);
        else
            perror("[initSemaphores] Erreur lors de la creation du tableau de semaphores");
        exit(EXIT_FAILURE);
    }

    /* Initialisation des sémaphores */
    if(semctl(SEM, 0, SETALL, init) == -1)
    {
        perror("[initSemaphores] Erreur lors de l'initialisation des semaphores");
        exit(EXIT_FAILURE);
    }

    printf("[initSemaphores] Semaphores crees\n");
    free(init);
}

void clearSemaphores()
{
    /* récupéré les sémaphores et les supprimé */
    if(semctl(SEM, 0, IPC_RMID) == -1)
        perror("[clearSemaphores] Erreur lors de la suppresion du tableau de semaphores");
    else
        printf("[initSemaphores] Semaphores supprimes\n");
}

void deleteIPC()
{
    printf("[deleteIPC] nettoyage\n");
    /* File */
    if((MSG = msgget(MSG_KEY, 0)) == -1)
        perror("[deleteIPC] La file de messages n'existe pas");
    else
    {
        if(msgctl(MSG, IPC_RMID, 0) == -1)
            perror("[deleteIPC] Erreur lors de la suppression de la file ");
        else
            printf("[deleteIPC] La file de messages est supprimee\n");
    }

    /* Mémoire */
    if((SHMID = shmget(MEM_KEY, 0, 0)) == -1)
        perror("[deleteIPC] la memoire partagee n'existe pas");
    else
    {
        if(shmctl(SHMID, IPC_RMID, 0) == -1)
            perror("[deleteIPC] Erreur lors de la suppression du segment de memoire partagee");
        else
            printf("[deleteIPC] La memoire partagee est supprimee\n");
    }

    /* sémaphores */
    if((SEM = semget(SEM_KEY, 0, 0)) == -1)
        perror("[deleteIPC] Le tableau de semaphores n'existe pas");
    else
    {
        if(semctl(SEM, 0, IPC_RMID) == -1)
            perror("[deleteIPC] Erreur lors de la suppresion du tableau de semaphores");
        else
            printf("[deleteIPC] Les semaphores sont supprimes\n");
    }
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
