#include "human.h"

static char* humans_array = NULL;                                 /* bitmap des zombies */
static pthread_mutex_t mutex_humans;                              /* le mutex pour la bitmap d'humain */
static pthread_t* thread_life = NULL;
static unsigned short* datas = NULL;

static void humansLock()
{
    if(pthread_mutex_lock(&mutex_humans) != 0)
    {
        perror("[humansLock] Erreur lors du verouillage du mutex mutex_humans ");
        exit(EXIT_FAILURE);
    }
}

static void humansUnlock()
{
    if(pthread_mutex_unlock(&mutex_humans) != 0)
    {
        perror("[zombieUnlock] Erreur lors du deverouillage du mutex mutex_humans ");
        exit(EXIT_FAILURE);
    }
}

static void* decreaseLife(void* data)
{
    unsigned short i;
    unsigned short id = *((unsigned short*)data);
    char ok = 1;

    sleep(HUMAN_LIFE_LOSS);

    while(ok)
    {
        ok = 0;

        can(CAN_HUMAN(id));
        for(i=0 ; i<HUMAN_PER_PLAYER ; ++i)
        {
            if(GET_HUMAN_LIFE(id,i)>0)
            {
                GET_HUMAN_LIFE(id,i)-=1;
                ok = 1;
            }
        }
        go(CAN_HUMAN(id));
        printf("[decreaseLife %d] vie diminué\n",id);

        sleep(HUMAN_LIFE_LOSS);
    }
    pthread_exit(NULL);
}

void decreaseLifeStop(unsigned short id)
{
    void* result;

    if(id>=0 && id<GET_MAX_PLAYER)
    {
        /* annulation du thread  */
        if(pthread_cancel(thread_life[id]))
            fprintf(stderr,"[decreaseLifeStop %d] Erreur lors de l'annulation du thread_life\n",id);
        if(pthread_join(thread_life[id],&result))
            fprintf(stderr,"[decreaseLifeStop %d] Erreur lors du join de thread_life\n",id);
        if(result!=PTHREAD_CANCELED)
            fprintf(stderr,"[decreaseLifeStop %d] Erreur, le thread thread_life n'a pas ete annule\n",id);
        printf("[decreaseLifeStop %d] Stope\n",id);
    }
}

void decreaseLifeStart(unsigned short id)
{
    if(id>=0 && id<GET_MAX_PLAYER)
    {
        decreaseLifeStop(id);

        /* crée le thread */
        if(pthread_create(&thread_life[id], NULL, decreaseLife ,&datas[id]))
        {
            fprintf(stderr,"[decreaseLifeStart %d] Erreur lors de la creation du thread thread_life\n",id);
            exit(EXIT_FAILURE);
        }
        printf("[decreaseLifeStart %d] Lance\n",id);
    }
}

void initHuman(map_t* map)
{
    unsigned short line,col,hum,pla;
    char* home_count = (char*)malloc(sizeof(char)*map->player_max);
    if(home_count==NULL)
    {
        fprintf(stderr,"[initHuman] Error lors de l'allocation dynamique de home_count\n");
        exit(EXIT_FAILURE);
    }
    for(hum=0 ; hum<map->player_max ; ++hum)
        home_count[hum] = 0;

    /* cree les threads et les données */
    thread_life = (pthread_t*)malloc(sizeof(pthread_t)*map->player_max);
    if(thread_life==NULL)
    {
        fprintf(stderr,"[initHuman] Error lors de l'allocation dynamique de thread_life\n");
        exit(EXIT_FAILURE);
    }
    for(hum=0 ; hum<map->player_max ; ++hum)
        memset(&thread_life[hum], 0, sizeof(pthread_t));

    datas = (unsigned short*)malloc(sizeof(unsigned short)*map->player_max);
    if(datas==NULL)
    {
        fprintf(stderr,"[initHuman] Error lors de l'allocation dynamique de datas\n");
        exit(EXIT_FAILURE);
    }
    for(hum=0 ; hum<map->player_max ; ++hum)
        datas[hum] = hum;

    /* crée la bitmap d'humains */
    humans_array = (char*)malloc(sizeof(char)*GET_HEIGHT*GET_WIDTH);
    if(humans_array==NULL)
    {
        fprintf(stderr,"[initHuman] Error lors de l'allocation dynamique de humans_array\n");
        exit(EXIT_FAILURE);
    }

    /* crée les humains */
    for(line=0 ; line<GET_HEIGHT ; ++line)
    {
        for(col=0 ; col<GET_WIDTH ; ++col)
        {
            HUMANS_ARRAY(line,col) = 0;
            if(map->map[line][col]>=HOME(0) && map->map[line][col]<HOME(MAX_PLAYER))
            {
                for(hum=0 ; hum<map->player_max ; ++hum)
                {
                    if(map->map[line][col]==HOME(hum))
                    {
                        if(home_count[hum] == 0)
                        {
                            home_count[hum]=1;
                            can(CAN_HUMAN(hum));
                            for(pla=0 ; pla<HUMAN_PER_PLAYER ; ++pla)
                            {
                                GET_HUMAN_LINE(hum,pla)=line;
                                GET_HUMAN_COL(hum,pla)=col;
                                GET_HUMAN_LIFE(hum,pla)=MAX_HUMAN_LIFE;
                                GET_HUMAN_AMMOS(hum,pla) = 0;
                                GET_HUMAN_FOODS(hum,pla) = 0;
                            }
                            go(CAN_HUMAN(hum));
                        }
                        break;
                    }
                }
            }
        }
    }

    /* crée le mutex */
    if(pthread_mutex_init(&mutex_humans,NULL))
        perror("[initHuman] Error lors de la creation du mutex mutex_humans ");

    printf("[initHuman] humains crees\n");
    free(home_count);
}

void clearHuman()
{
    unsigned short i;
    void* result = NULL;

    if(thread_life!=NULL)
    {
        for(i=0 ; i<GET_MAX_PLAYER ; ++i)
        {
            if(pthread_cancel(thread_life[i]))
                fprintf(stderr,"[clearHuman] Erreur lors de l'annulation du thread thread_life[%d]\n",i);
            if(pthread_join(thread_life[i],&result))
                fprintf(stderr,"[clearHuman] Erreur lors du join de thread_life[%d]\n",i);
            if(result!=PTHREAD_CANCELED)
                fprintf(stderr,"[clearHuman] Erreur, le thread thread_life[%d] n'a pas ete annule\n",i);
        }
    }

    /* destruction du mutex */
    if(pthread_mutex_destroy(&mutex_humans))
        perror("[clearHuman] Erreur lors de la destruction de mutex_humans ");

    /* libération mémoire */
    if(humans_array!=NULL)free(humans_array);
    if(thread_life!=NULL)free(thread_life);
    if(datas!=NULL)free(datas);

    printf("[clearHuman] humains supprimes\n");
}

void checkFoodsAndAmmos(command_t* command,char home)
{
    unsigned short get;
    unsigned short line,col;
    unsigned short tile;

    if(command->global_id<0 || command->global_id>=GET_MAX_PLAYER)
        return;

    can(CAN_HUMAN(command->global_id));
    {
        humansLock();
        {

            line = GET_HUMAN_LINE(command->global_id,command->local_id);
            col = GET_HUMAN_COL(command->global_id,command->local_id);

            can(CAN_TILE(line,col));
            {
                tile = GET_TILE(line,col);
            }
            go(CAN_TILE(line,col));

            if(GET_HUMAN_FOODS(command->global_id,command->local_id)+GET_HUMAN_AMMOS(command->global_id,command->local_id)<HUMAN_MAX_CARRY)
            {
                if(tile==FOOD)
                {

                    get = getFood(line,col,1);
                    GET_HUMAN_FOODS(command->global_id,command->local_id)+=get;
                }
                else if(tile==AMMO)
                {
                    get = getAmmo(line,col,1);
                    GET_HUMAN_AMMOS(command->global_id,command->local_id)+=get;
                }
                else if(home && tile==HOME(command->global_id))
                {
                    can(CAN_HOME(command->global_id));
                    if(GET_HOME_FOOD(command->global_id)>0)
                    {
                        GET_HOME_FOOD(command->global_id)-=1;
                        GET_HUMAN_FOODS(command->global_id,command->local_id)+=1;
                    }
                    if(GET_HOME_AMMO(command->global_id)>0)
                    {
                        GET_HOME_AMMO(command->global_id)-=1;
                        GET_HUMAN_AMMOS(command->global_id,command->local_id)+=1;
                    }
                    go(CAN_HOME(command->global_id));
                }
            }
        }
        humansUnlock();
    }
    go(CAN_HUMAN(command->global_id));
}

void moveHuman(command_t* command)
{
    unsigned short line,col;

    if(command->global_id==0 || command->line<0 || command->line>=GET_HEIGHT || command->col<0 || command->col>=GET_WIDTH)
        return;

    command->global_id = command->global_id-1;

    can(CAN_HUMAN(command->global_id));
    {
        can(CAN_TILE(command->line,command->col));
        {
            humansLock();
            {

                line = GET_HUMAN_LINE(command->global_id,command->local_id);
                col = GET_HUMAN_COL(command->global_id,command->local_id);

                /* vérification du mouvement */
                if(GET_HUMAN_LIFE(command->global_id,command->local_id)!=0)
                {
                    if(
                            (GET_TILE(command->line,command->col)==EMPTY
                             || GET_TILE(command->line,command->col)==FOOD
                             || GET_TILE(command->line,command->col)==AMMO
                             || GET_TILE(command->line,command->col)==HOME(command->global_id)
                             || (GET_TILE(command->line,command->col)>=HIDEOUT(0) && GET_TILE(command->line,command->col)<HIDEOUT(MAX_HIDEOUT))
                             )
                            && noZombie(command->line,command->col)
                            && HUMANS_ARRAY(command->line,command->col)==0
                            )
                    {
                        /* déplacement à travers une cachette */
                        if(GET_TILE(command->line,command->col)>=HIDEOUT(0) && GET_TILE(command->line,command->col)<HIDEOUT(MAX_HIDEOUT) && GET_TILE(line,col)==GET_TILE(command->line,command->col))
                        {
                            GET_HUMAN_LINE(command->global_id,command->local_id) = command->line;
                            GET_HUMAN_COL(command->global_id,command->local_id) = command->col;
                            if(GET_TILE(command->line,command->col)!=HOME(command->global_id) && !(GET_TILE(command->line,command->col)>=HIDEOUT(0) && GET_TILE(command->line,command->col)<HIDEOUT(MAX_HIDEOUT)))
                                HUMANS_ARRAY(command->line,command->col)=1;
                            if(GET_TILE(line,col)!=HOME(command->global_id) && !(GET_TILE(line,col)>=HIDEOUT(0) && GET_TILE(line,col)<HIDEOUT(MAX_HIDEOUT)))
                                HUMANS_ARRAY(line,col)=0;
                        }
                        /* deplacement entre reperes */
                        else if(GET_TILE(line,col)==HOME(command->global_id) && GET_TILE(command->line,command->col)==HOME(command->global_id))
                        {
                            GET_HUMAN_LINE(command->global_id,command->local_id) = command->line;
                            GET_HUMAN_COL(command->global_id,command->local_id) = command->col;
                            if(GET_TILE(command->line,command->col)!=HOME(command->global_id) && !(GET_TILE(command->line,command->col)>=HIDEOUT(0) && GET_TILE(command->line,command->col)<HIDEOUT(MAX_HIDEOUT)))
                                HUMANS_ARRAY(command->line,command->col)=1;
                            if(GET_TILE(line,col)!=HOME(command->global_id) && !(GET_TILE(line,col)>=HIDEOUT(0) && GET_TILE(line,col)<HIDEOUT(MAX_HIDEOUT)))
                                HUMANS_ARRAY(line,col)=0;
                        }
                        /* déplacement sur une ligne */
                        else if(line==command->line)
                        {
                            if(col+1 == command->col || col-1 == command->col)
                            {
                                GET_HUMAN_LINE(command->global_id,command->local_id) = command->line;
                                GET_HUMAN_COL(command->global_id,command->local_id) = command->col;
                                if(GET_TILE(command->line,command->col)!=HOME(command->global_id) && !(GET_TILE(command->line,command->col)>=HIDEOUT(0) && GET_TILE(command->line,command->col)<HIDEOUT(MAX_HIDEOUT)))
                                    HUMANS_ARRAY(command->line,command->col)=1;
                                if(GET_TILE(line,col)!=HOME(command->global_id) && !(GET_TILE(line,col)>=HIDEOUT(0) && GET_TILE(line,col)<HIDEOUT(MAX_HIDEOUT)))
                                    HUMANS_ARRAY(line,col)=0;
                            }
                        }
                        /* déplacement sur une colonne */
                        else if(col==command->col)
                        {
                            if(line+1 == command->line || line-1 == command->line)
                            {
                                GET_HUMAN_LINE(command->global_id,command->local_id) = command->line;
                                GET_HUMAN_COL(command->global_id,command->local_id) = command->col;
                                if(GET_TILE(command->line,command->col)!=HOME(command->global_id) && !(GET_TILE(command->line,command->col)>=HIDEOUT(0) && GET_TILE(command->line,command->col)<HIDEOUT(MAX_HIDEOUT)))
                                    HUMANS_ARRAY(command->line,command->col)=1;
                                if(GET_TILE(line,col)!=HOME(command->global_id) && !(GET_TILE(line,col)>=HIDEOUT(0) && GET_TILE(line,col)<HIDEOUT(MAX_HIDEOUT)))
                                    HUMANS_ARRAY(line,col)=0;
                            }
                        }
                    }
                }
            }
            humansUnlock();
        }
        go(CAN_TILE(command->line,command->col));
    }
    go(CAN_HUMAN(command->global_id));

    checkFoodsAndAmmos(command,0);
}

char noHuman(unsigned short line,unsigned short col)
{
    char temp;
    humansLock();
    temp = (HUMANS_ARRAY(line,col)==0);
    humansUnlock();
    return temp;
}

char getHumain(unsigned short line,unsigned short col,unsigned short* global, unsigned short* local)
{
    unsigned int glob,loc;
    unsigned short tile;

    humansLock();
    {
        can(CAN_TILE(line,col));
        {
            tile = GET_TILE(line,col);

            if(HUMANS_ARRAY(line,col)==1 && !(tile>=HOME(0) && tile<HOME(MAX_PLAYER)) && !(tile>=HIDEOUT(0) && tile<HIDEOUT(MAX_PLAYER)))
            {
                for(glob=0 ; glob<GET_MAX_PLAYER ; ++glob)
                {
                    can(CAN_HUMAN(glob));
                    {
                        for(loc=0 ; loc<HUMAN_PER_PLAYER ; ++loc)
                        {
                            if(GET_HUMAN_LINE(glob,loc)==line && GET_HUMAN_COL(glob,loc)==col)
                            {
                                *global = glob;
                                *local = loc;
                                go(CAN_HUMAN(glob));
                                go(CAN_TILE(line,col));
                                humansUnlock();
                                return 1;
                            }
                        }
                    }
                    go(CAN_HUMAN(glob));
                }
            }
        }
        go(CAN_TILE(line,col));
    }
    humansUnlock();
    return 0;
}

void killHuman(unsigned short global,unsigned short local)
{
    unsigned short line,col;

    if(global>=GET_MAX_PLAYER || local>=HUMAN_PER_PLAYER)
        return;

    humansLock();
    {
        can(CAN_HUMAN(global));
        {
            line = GET_HUMAN_LINE(global,local);
            col = GET_HUMAN_COL(global,local);
            GET_HUMAN_LIFE(global,local) = 0;
            HUMANS_ARRAY(line,col) = 0;
        }
        go(CAN_HUMAN(global));
    }
    humansUnlock();
}

void humanEat(unsigned short global,unsigned short local)
{
    if(global>=GET_MAX_PLAYER || local>=HUMAN_PER_PLAYER)
        return;

    humansLock();
    {
        can(CAN_HUMAN(global));
        {
            if(GET_HUMAN_FOODS(global,local)>0)
            {
                GET_HUMAN_FOODS(global,local)-=1;
                if(GET_HUMAN_LIFE(global,local)<MAX_HUMAN_LIFE && GET_HUMAN_LIFE(global,local)>0)
                    GET_HUMAN_LIFE(global,local)+=1;
            }
        }
        go(CAN_HUMAN(global));
    }
    humansUnlock();

    printf("[humanEat] l'humain(%d,%d) mange\n",global,local);
}

void setFoodAndAmmo(command_t* command)
{
    if(command->global_id<0 || command->global_id>=GET_MAX_PLAYER)
            return;

    can(CAN_HUMAN(command->global_id));
    {
        can(CAN_TILE(GET_HUMAN_LINE(command->global_id,command->local_id),GET_HUMAN_COL(command->global_id,command->local_id)));
        {
            if(GET_TILE(GET_HUMAN_LINE(command->global_id,command->local_id),GET_HUMAN_COL(command->global_id,command->local_id))==HOME(command->global_id))
            {

                if(GET_HUMAN_FOODS(command->global_id,command->local_id)>0)
                {
                    GET_HUMAN_FOODS(command->global_id,command->local_id)-=1;
                    can(CAN_HOME(command->global_id));
                    {
                        GET_HOME_FOOD(command->global_id)+=1;
                    }
                    go(CAN_HOME(command->global_id));
                }
                if(GET_HUMAN_AMMOS(command->global_id,command->local_id)>0)
                {
                    GET_HUMAN_AMMOS(command->global_id,command->local_id)-=1;
                    can(CAN_HOME(command->global_id));
                    {
                        GET_HOME_AMMO(command->global_id)+=1;
                    }
                    go(CAN_HOME(command->global_id));
                }
            }
        }
        go(CAN_TILE(GET_HUMAN_LINE(command->global_id,command->local_id),GET_HUMAN_COL(command->global_id,command->local_id)));
    }
    go(CAN_HUMAN(command->global_id));
}
