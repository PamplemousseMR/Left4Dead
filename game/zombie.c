#include "zombie.h"

static char* zombies_array = NULL;                                  /* bitmap des zombies */
static pthread_t thread_zombies;                                    /* le thread d'ajout de zombies */
static unsigned int cemetery_count = 0;                             /* le nombre de cimetières sur la map */
static unsigned short spawn;                                        /* la valeur pour la fréquence d'apparition des zombies */
static pthread_mutex_t mutex_zombies;                               /* le mutex pour la bitmap de zombies */
static pthread_t thread_zombies_move;                                    /* le thread de déplacement des zombies */

static void zombiesLock()
{
    if(pthread_mutex_lock(&mutex_zombies) != 0)
    {
        fprintf(stderr,"[zombiesLock] Erreur lors du verouillage du mutex mutex_zombies ");
        exit(EXIT_FAILURE);
    }
}

static void zombieUnlock()
{
    if(pthread_mutex_unlock(&mutex_zombies) != 0)
    {
        fprintf(stderr,"[zombieUnlock] Erreur lors du deverouillage du mutex mutex_zombies ");
        exit(EXIT_FAILURE);
    }
}

/* l'id dans la memoire partagée où l'on peut ajouter un zombie */
static short getZombie()
{
    unsigned short i;

    for(i=0 ; i<GET_MAX_ZOMBIE ; ++i)
        if(GET_ZOMBIE_LIFE(i)==0)
            return i;
    return -1;
}

static void addZombie(unsigned short line, unsigned short col, unsigned short id)
{
    can(CAN_ZOMBIE);
    {
        GET_ZOMBIE_COL(id) = col;
        GET_ZOMBIE_LINE(id) = line;
        GET_ZOMBIE_LIFE(id) = MAX_ZOMBIE_LIFE;
    }
    go(CAN_ZOMBIE);
    ZOMBIES_ARRAY(line,col)=1;
}

/* ajoute des zombies */
static void* createZombie(void* data)
{
    unsigned int seed;
    unsigned short val,temp;
    short line,col;
    char ok;
    short zombie_add;

    seed = rand();

    if(cemetery_count!=0)
    {
        while(1)
        {
            zombiesLock();
            {
                zombie_add = getZombie();
                if(zombie_add != -1)
                {

                    val = (rand_r(&seed)%cemetery_count)+1;
                    temp = 0;
                    ok = 1;
                    line = 0;
                    col = 0;

                    /* trouver un cimetière au hasard */
                    for(line=0 ; line<GET_HEIGHT && ok ; ++line)
                    {
                        for(col=0 ; col<GET_WIDTH && ok ; ++col)
                        {
                            can(CAN_TILE(line,col));
                            {
                                if(GET_TILE(line,col)==CEMETERY)
                                {
                                    ++temp;
                                    if(temp == val)
                                        ok = 0;
                                }
                            }
                            go(CAN_TILE(line,col));
                        }
                    }

                    --line;
                    --col;
                    ok=1;

                    /* ajouter un zombie autour du cimetière */
                    if(line+1<GET_HEIGHT)
                    {
                        can(CAN_TILE(line+1,col));
                        {
                            if(GET_TILE(line+1,col)==EMPTY && ZOMBIES_ARRAY(line+1,col)==0 && noHuman(line+1,col))
                            {
                                addZombie(line+1,col,zombie_add);
                                ok = 0;
                            }
                        }
                        go(CAN_TILE(line+1,col));
                    }
                    if(ok && line-1>=0)
                    {
                        can(CAN_TILE(line-1,col));
                        {
                            if(GET_TILE(line-1,col)==EMPTY && ZOMBIES_ARRAY(line-1,col)==0 && noHuman(line-1,col))
                            {
                                addZombie(line-1,col,zombie_add);
                                ok = 0;
                            }
                        }
                        go(CAN_TILE(line-1,col));
                    }
                    if(ok && col+1<GET_WIDTH)
                    {
                        can(CAN_TILE(line,col+1));
                        {
                            if(GET_TILE(line,col+1)==EMPTY && ZOMBIES_ARRAY(line,col+1)==0 && noHuman(line,col+1))
                            {
                                addZombie(line,col+1,zombie_add);
                                ok = 0;
                            }
                        }
                        go(CAN_TILE(line,col+1));
                    }
                    if(ok && col-1>=0)
                    {
                        can(CAN_TILE(line,col-1));
                        {
                            if(GET_TILE(line,col-1)==EMPTY && ZOMBIES_ARRAY(line,col-1)==0 && noHuman(line,col-1))
                            {
                                addZombie(line,col-1,zombie_add);
                                ok = 0;
                            }
                        }
                        go(CAN_TILE(line,col-1));
                    }
                    if(ok)
                        printf("[createZombie] pas de place pour le zombie : %d\n",spawn);
                    else
                        printf("[createZombie] ajout de zombie : %d\n",spawn);
                }
                else
                    printf("[createZombie] zombie plein : %d\n",spawn);
            }
            zombieUnlock();
            sleep(spawn);
        }
    }
    else
        printf("[createZombie] aucun cimetiere\n");
    pthread_exit(NULL);
}

void initZombies(map_t* map)
{
    unsigned short line,col;
    spawn = map->zombie_spawn;

    /* crée la bitmap de zombie */
    zombies_array = (char*)malloc(sizeof(char)*GET_HEIGHT*GET_WIDTH);
    if(zombies_array==NULL)
    {
        fprintf(stderr,"[initZombies] Error lors de l'allocation dynamique de zombies_array\n");
        exit(EXIT_FAILURE);
    }
    for(line=0 ; line<GET_HEIGHT ; ++line)
    {
        for(col=0 ; col<GET_WIDTH ; ++col)
            ZOMBIES_ARRAY(line,col) = 0;
    }

    /* compter le nombre de cimetière */
    for(line=0 ; line<GET_HEIGHT ; ++line)
    {
        for(col=0 ; col<GET_WIDTH ; ++col)
        {
            can(CAN_TILE(line,col));
            {
                if(GET_TILE(line,col)==CEMETERY)
                    cemetery_count++;
            }
            go(CAN_TILE(line,col));
        }
    }

    /* crée le mutex */
    if(pthread_mutex_init(&mutex_zombies,NULL))
        fprintf(stderr,"[initZombies] Error lors de la creation du mutex mutex_zombies\n");

    /* crée le thread */
    if(pthread_create(&thread_zombies, NULL, createZombie ,NULL))
    {
        fprintf(stderr,"[initZombies] Error lors de la creation du thread thread_zombies\n");
        exit(EXIT_FAILURE);
    }

    printf("[initZombies] zombies cree\n");
}

void clearZombies()
{
    void* result = NULL;

    /* annulation des threads */
    if(pthread_cancel(thread_zombies))
        fprintf(stderr,"[clearZombies] Erreur lors de l'annulation du thread thread_zombies\n");
    if(pthread_join(thread_zombies,&result))
        fprintf(stderr,"[clearZombies] Erreur lors du join de thread_zombies\n");
    if(result!=PTHREAD_CANCELED)
        fprintf(stderr,"[clearZombies] Erreur, le thread thread_zombies n'a pas ete annulé\n");

    if(pthread_cancel(thread_zombies_move))
        fprintf(stderr,"[clearZombies] Erreur lors de l'annulation du thread thread_zombies_move\n");
    if(pthread_join(thread_zombies_move,&result))
        fprintf(stderr,"[clearZombies] Erreur lors du join de thread_zombies_move\n");
    if(result!=PTHREAD_CANCELED)
        fprintf(stderr,"[clearZombies] Erreur, le thread thread_zombies_move n'a pas ete annulé\n");

    /* destruction du mutex */
    if(pthread_mutex_destroy(&mutex_zombies))
        fprintf(stderr,"[clearZombies] Erreur lors de la destruction de mutex_zombies\n");

    /* libération mémoire */
    if(zombies_array!=NULL)free(zombies_array);

    printf("[clearZombies] zombies supprimme\n");
}

char noZombie(unsigned short line,unsigned short col)
{
    char temp = 0;
    zombiesLock();
    {
        temp = (ZOMBIES_ARRAY(line,col)==0);
    }
    zombieUnlock();
    return temp;
}

void moveZombie(command_t* command)
{
    unsigned short line,col;

    if(command->global_id!=0 || command->line<0 || command->line>=GET_HEIGHT || command->col<0 || command->col>=GET_WIDTH)
        return;

    zombiesLock();
    {
        can(CAN_TILE(command->line,command->col));
        {
            can(CAN_ZOMBIE);
            {

                line = GET_ZOMBIE_LINE(command->local_id);
                col = GET_ZOMBIE_COL(command->local_id);

                /* vérification du mouvement */
                if(ZOMBIES_ARRAY(line,col)==1 && GET_ZOMBIE_LIFE(command->local_id)!=0)
                {
                    /* déplacement sur la largeur */
                    if(line==command->line)
                    {
                        if(col+1 == command->col || col-1 == command->col)
                        {
                            if((GET_TILE(command->line,command->col)==EMPTY || GET_TILE(command->line,command->col)==FOOD || GET_TILE(command->line,command->col)==AMMO) && ZOMBIES_ARRAY(command->line,command->col)==0 && noHuman(command->line,command->col))
                            {
                                ZOMBIES_ARRAY(line,col)=0;
                                GET_ZOMBIE_LINE(command->local_id) = command->line;
                                GET_ZOMBIE_COL(command->local_id) = command->col;
                                ZOMBIES_ARRAY(command->line,command->col)=1;
                            }
                        }
                    }
                    /* déplacement sur la hauteur */
                    else if(col==command->col)
                    {
                        if(line+1 == command->line || line-1 == command->line)
                        {
                            if((GET_TILE(command->line,command->col)==EMPTY || GET_TILE(command->line,command->col)==FOOD || GET_TILE(command->line,command->col)==AMMO) && ZOMBIES_ARRAY(command->line,command->col)==0 && noHuman(command->line,command->col))
                            {
                                ZOMBIES_ARRAY(line,col)=0;
                                GET_ZOMBIE_LINE(command->local_id) = command->line;
                                GET_ZOMBIE_COL(command->local_id) = command->col;
                                ZOMBIES_ARRAY(command->line,command->col)=1;
                            }
                        }
                    }
                }

            }
            go(CAN_ZOMBIE);
        }
        go(CAN_TILE(command->line,command->col));
    }
    zombieUnlock();

    checkHumainAround(command->local_id);
}

static void* moveAllonThread()
{
    command_t command;
    unsigned short i;
    int rnd;

    struct timespec tim,tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = UPDATE_ZOMBIE;

    while(1)
    {
        for(i=0 ; i<GET_MAX_ZOMBIE ; ++i)
        {
            rnd = rand()%4;

            command.global_id=0;
            command.local_id=i;
            pthread_testcancel();
            can(CAN_ZOMBIE);
            {

                if(rnd==0)
                {
                    command.line=GET_ZOMBIE_LINE(i);
                    command.col=GET_ZOMBIE_COL(i)+1;
                }
                else if(rnd==1)
                {
                    command.line=GET_ZOMBIE_LINE(i)+1;
                    command.col=GET_ZOMBIE_COL(i);
                }
                else if(rnd==2)
                {
                    command.line=GET_ZOMBIE_LINE(i)-1;
                    command.col=GET_ZOMBIE_COL(i);
                }
                else
                {
                    command.line=GET_ZOMBIE_LINE(i);
                    command.col=GET_ZOMBIE_COL(i)-1;
                }
            }
            go(CAN_ZOMBIE);
            pthread_testcancel();
            command.command_type=COMMAND_MOVE;
            moveZombie(&command);
        }

        printf("[moveAllonThread] deplacement des zombies\n");
        nanosleep(&tim,&tim2);
    }
    pthread_exit(NULL);
}

void moveAllonStop()
{
    void* result = NULL;

    /* annulation du thread  */
    if(pthread_cancel(thread_zombies_move))
        fprintf(stderr,"[moveAllonStop] Erreur lors de l'annulation du thread thread_zombies_move\n");
    if(pthread_join(thread_zombies_move,&result))
        fprintf(stderr,"[moveAllonStop] Erreur lors du join de thread_zombies_move\n");
    if(result!=PTHREAD_CANCELED)
        fprintf(stderr,"[moveAllonStop] Erreur, le thread thread_zombies_move n'a pas ete annule\n");

    printf("[moveAllonStop] Zombies stope\n");
}

void moveAllonStart()
{
    moveAllonStop();

    /* crée le thread */
    if(pthread_create(&thread_zombies_move, NULL, moveAllonThread ,NULL))
    {
        fprintf(stderr,"[moveAllonStart] Error lors de la creation du thread thread_zombies\n");
        exit(EXIT_FAILURE);
    }

    printf("[moveAllonStop] Zombies demmare\n");
}

void checkHumainAround(unsigned short local)
{
    unsigned short col,line;
    unsigned short hum_glob;
    unsigned short hum_loc;
    char ok = 0;
    char kill = 0;
    short zombie_add;

    if(local >= GET_MAX_ZOMBIE)
        return;

    can(CAN_ZOMBIE);
    {
        line = GET_ZOMBIE_LINE(local);
        col = GET_ZOMBIE_COL(local);
    }
    go(CAN_ZOMBIE);

    if(line-1>=0)
    {
        if(getHumain(line-1,col,&hum_glob,&hum_loc))
        {
            line-=1;
            kill = 1;
            ok = 0;
        }
    }
    if(!ok && line+1<GET_HEIGHT)
    {
        if(getHumain(line+1,col,&hum_glob,&hum_loc))
        {
            line+=1;
            kill = 1;
            ok = 0;
        }
    }
    if(!ok && col+1<GET_WIDTH)
    {
        if(getHumain(line,col+1,&hum_glob,&hum_loc))
        {
            col+=1;
            kill = 1;
            ok = 0;
        }
    }
    if(!ok && col-1>=0)
    {
        if(getHumain(line,col-1,&hum_glob,&hum_loc))
        {
            col-=1;
            kill = 1;
            ok = 0;
        }
    }

    if(kill)
    {
        zombiesLock();
        {
            zombie_add = getZombie();
            if(zombie_add!=-1)
                addZombie(line,col,zombie_add);
            killHuman(hum_glob,hum_loc);
        }
        zombieUnlock();
    }
}

void zombieEat(unsigned short local)
{
    if(local >= GET_MAX_ZOMBIE)
        return;
    checkHumainAround(local);
    printf("[zombieEat] le zombie %d mange\n",local);
}
