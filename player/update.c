#include "update.h"

static camera_t camera;
static map_t* map = NULL;
static zombie_t* zombies = NULL;
static human_t* humans = NULL;
static home_t* homes = NULL;
static pthread_t thread;
static pthread_mutex_t camera_mutex;            /* le mutex pour la camera */
static pthread_mutex_t zombie_mutex;            /* le mutex pour zombies */
static pthread_mutex_t map_mutex;               /* le mutex pour map */
static pthread_mutex_t human_mutex;             /* le mutex pour humans */
static pthread_mutex_t home_mutex;              /* le mutex pour map */

static void mapLock()
{
    if(pthread_mutex_lock(&map_mutex) != 0)
    {
        perror("[mapLock] Error lors du lock du mutex map_mutex ");
        exit(EXIT_FAILURE);
    }
}

static void mapUnlock()
{
    if(pthread_mutex_unlock(&map_mutex) != 0)
    {
        perror("[mapUnlock] Error lors du lock du mutex map_mutex ");
        exit(EXIT_FAILURE);
    }
}

static void cameraLock()
{
    if(pthread_mutex_lock(&camera_mutex) != 0)
    {
        perror("[cameraLock] Error lors du lock du mutex camera_mutex ");
        exit(EXIT_FAILURE);
    }
}

static void cameraUnlock()
{
    if(pthread_mutex_unlock(&camera_mutex) != 0)
    {
        perror("[cameraUnlock] Error lors du lock du mutex camera_mutex ");
        exit(EXIT_FAILURE);
    }
}

short getCameraLine()
{
    short ret;
    cameraLock();
    ret = camera.line;
    cameraUnlock();
    return ret;
}

short getCameraCol()
{
    short ret;
    cameraLock();
    ret = camera.col;
    cameraUnlock();
    return ret;
}

unsigned short getCameraWidth()
{
    unsigned short ret;
    cameraLock();
    ret = camera.width;
    cameraUnlock();
    return ret;
}

unsigned short getCameraHeight()
{
    unsigned short ret;
    cameraLock();
    ret = camera.height;
    cameraUnlock();
    return ret;
}

void setCameraLine(short line)
{
    cameraLock();
    camera.line=line;
    cameraUnlock();
}

void setCameraCol(short col)
{
    cameraLock();
    camera.col=col;
    cameraUnlock();
}

static void zombiesLock()
{
    if(pthread_mutex_lock(&zombie_mutex) != 0)
    {
        perror("[zombiesLock] Error lors du lock du mutex zombie_mutex ");
        exit(EXIT_FAILURE);
    }
}

static void zombiesUnlock()
{
    if(pthread_mutex_unlock(&zombie_mutex) != 0)
    {
        perror("[zombiesUnlock] Error lors du unlock du mutex zombie_mutex ");
        exit(EXIT_FAILURE);
    }
}

static void humansLock()
{
    if(pthread_mutex_lock(&human_mutex) != 0)
    {
        perror("[zombiesLock] Error lors du lock du mutex human_mutex ");
        exit(EXIT_FAILURE);
    }
}

static void humansUnlock()
{
    if(pthread_mutex_unlock(&human_mutex) != 0)
    {
        perror("[zombiesUnlock] Error lors du unlock du mutex human_mutex ");
        exit(EXIT_FAILURE);
    }
}

static void homeLock()
{
    if(pthread_mutex_lock(&home_mutex) != 0)
    {
        perror("[zombiesLock] Error lors du lock du mutex home_mutex ");
        exit(EXIT_FAILURE);
    }
}

static void homeUnlock()
{
    if(pthread_mutex_unlock(&home_mutex) != 0)
    {
        perror("[zombiesUnlock] Error lors du unlock du mutex home_mutex ");
        exit(EXIT_FAILURE);
    }
}


void updateMap()
{
    if(map==NULL)
        return;

    mapLock();
    {
        cameraLock();
        {
            wclear(SHOW);
            drawMap(SHOW,map,camera.line,camera.col,camera.height,camera.width);
            wrefresh(SHOW);
        }
        cameraUnlock();
    }
    mapUnlock();
}

void updatePlayer()
{
    unsigned short i;

    if(map==NULL || zombies==NULL || humans==NULL)
        return;

    wclear(PLAYER);
    if(getType()==TYPE_ZOMBIE)
    {
        zombiesLock();
        {
            drawZombies(zombies,map->zombie_max);
        }
        zombiesUnlock();
    }
    else
    {
        humansLock();
        {
            drawHumans(humans,map->player_max*HUMAN_PER_PLAYER,getGlobalId());
        }
        humansUnlock();
        wprintw(PLAYER,"\n");
        for(i=0 ; i<PLAYER_COL ; ++i)waddch(PLAYER,'=');
        wprintw(PLAYER,"\n");
        wprintw(PLAYER,"REPERE :\n  MUNITIONS - %d\n  NOURRITURES - %d\n",homes->ammo,homes->foods);
    }
    wrefresh(PLAYER);
}

static void cleanMapReceive(void* map)
{
    unsigned short* map_receive;
    map_receive = (unsigned short*)map;
    if(map!=NULL)free(map_receive);
}

static void* updateData(void* data)
{
    unsigned i;
    unsigned short* map_receive = NULL;

    map_receive = (unsigned short*)malloc(sizeof(unsigned short)*map->height*map->width);
    if(map_receive==NULL)
    {
        fprintf(stderr,"[updateData] Erreur lors de l'allocation dynamique de la carte\n");
        exit(EXIT_FAILURE);
    }
    pthread_cleanup_push(cleanMapReceive,map_receive);

    while(1)
    {
        mapLock();
        {
            readSock(map_receive, sizeof(unsigned short)*map->height*map->width);
        }
        mapUnlock();

        zombiesLock();
        {
            readSock(zombies, sizeof(zombie_t)*map->zombie_max);
        }
        zombiesUnlock();

        humansLock();
        {
            readSock(humans, sizeof(human_t)*map->player_max*HUMAN_PER_PLAYER);
        }
        humansUnlock();

        if(getType()==TYPE_HUMAN)
        {
            homeLock();
            {
                readSock(homes, sizeof(home_t));
            }
            homeUnlock();
        }

        mapLock();
        {
            for(i=0 ; i<map->height*map->width ; ++i)
            {
                map->map[i/map->width][i%map->width] = map_receive[i];
                if(getType()==TYPE_ZOMBIE && map_receive[i]>=HIDEOUT(0) && map_receive[i]<HIDEOUT(MAX_HIDEOUT))
                    map->map[i/map->width][i%map->width] = OBSTACLE;
            }
        }
        mapUnlock();

        mapLock();
        {
            zombiesLock();
            {
                for(i=0 ; i<map->zombie_max ; ++i)
                {
                    if(zombies[i].life!=0)
                        map->map[zombies[i].line][zombies[i].col] = ZOMBIE;
                }
            }
            zombiesUnlock();

            humansLock();
            {
                for(i=0 ; i<map->player_max*HUMAN_PER_PLAYER ; ++i)
                {
                    if(humans[i].life!=0 && (map->map[humans[i].line][humans[i].col]==EMPTY || map->map[humans[i].line][humans[i].col]==FOOD || map->map[humans[i].line][humans[i].col]==AMMO))
                        map->map[humans[i].line][humans[i].col] = HUMAN(i/HUMAN_PER_PLAYER);
                }
            }
            humansUnlock();
        }
        mapUnlock();

        updatePlayer();
        updateMap();
    }

    pthread_cleanup_pop(1);

    pthread_exit(NULL);
}

void initUpdate()
{
    map_size_t map_size;

    /* Lecture de la taille de la carte */
    readSock(&map_size, sizeof(map_size_t));

    map = createMap(map_size.height,map_size.width);

    /* Lecture du nombre de zombies */
    readSock(&map->zombie_max, sizeof(unsigned short));

    zombies = (zombie_t*)malloc(sizeof(zombie_t)*map->zombie_max);
    if(zombies==NULL)
    {
        fprintf(stderr,"[initMap] Erreur lors de l'allocation dnyamique de zombie\n");
        exit(EXIT_FAILURE);
    }

    /* Lecture du nombre d'humains */
    readSock(&map->player_max, sizeof(unsigned short));

    humans = (human_t*)malloc(sizeof(human_t)*map->player_max*HUMAN_PER_PLAYER);
    if(zombies==NULL)
    {
        fprintf(stderr,"[initMap] Erreur lors de l'allocation dynamique de humans\n");
        exit(EXIT_FAILURE);
    }

    if(getType()==TYPE_HUMAN)
    {
        homes = (home_t*)malloc(sizeof(home_t));
        if(homes==NULL)
        {
            fprintf(stderr,"[initMap] Erreur lors de l'allocation dynamique de homes\n");
            exit(EXIT_FAILURE);
        }
    }

    /* placer la caméra */
    camera.height = SHOW_LINE;
    camera.width = SHOW_COL;
    camera.col = map->width/2 - camera.width/2;
    camera.line = map->height/2 - camera.height/2;

    /* crée les mutex */
    pthread_mutex_init(&zombie_mutex,NULL);
    pthread_mutex_init(&map_mutex,NULL);
    pthread_mutex_init(&human_mutex,NULL);
    pthread_mutex_init(&home_mutex,NULL);

    /* lancer le thread de mise à jour */
    if(pthread_create(&thread, NULL, updateData, NULL))
    {
        perror("[initMap] Erreur lors de la creation du thread ");
        exit(EXIT_FAILURE);
    }
}

void clearUpdate()
{
    void* result;

    /* annuler le thread */
    pthread_cancel(thread);
    pthread_join(thread, &result);
    if (result != PTHREAD_CANCELED)
        fprintf(stderr,"[clearMap] Probleme lors de l'annulation du thread\n");

    if(map!=NULL)
    {
        deleteMap(map);
        map=NULL;
    }
    if(zombies!=NULL)
    {
        free(zombies);
        zombies=NULL;
    }
    if(humans!=NULL)
    {
        free(humans);
        humans=NULL;
    }
    if(homes!=NULL)
    {
        free(homes);
        homes=NULL;
    }
    pthread_mutex_destroy(&zombie_mutex);
    pthread_mutex_destroy(&map_mutex);
    pthread_mutex_destroy(&human_mutex);
    pthread_mutex_destroy(&home_mutex);
}

short getId(unsigned short line,unsigned short col)
{
    unsigned short i;

    if(map==NULL || zombies==NULL || humans==NULL)
        return -1;

    if(getType() == TYPE_ZOMBIE)
    {
        zombiesLock();
        {
            for(i=0 ; i<map->zombie_max ; ++i)
            {
                if(zombies[i].line==line && zombies[i].col==col)
                {
                    zombiesUnlock();
                    return i;
                }
            }
        }
        zombiesUnlock();
    }
    else
    {
        humansLock();
        {
            for(i=getGlobalId()*HUMAN_PER_PLAYER ; i<getGlobalId()*HUMAN_PER_PLAYER+HUMAN_PER_PLAYER ; ++i)
            {
                if(humans[i].line==line && humans[i].col==col)
                {
                    humansUnlock();
                    return i-(getGlobalId()*HUMAN_PER_PLAYER);
                }
            }
        }
        humansUnlock();
    }
    return -1;
}

unsigned short getTile(unsigned short line,unsigned short col)
{
    unsigned short temp;
    if(map==NULL)
        return 0;

    mapLock();
    {
        temp = map->map[line][col];
    }
    mapUnlock();
    return temp;
}

void getEntityPos(unsigned short id,int* line,int* col)
{
    if(getType()==TYPE_ZOMBIE)
    {
        zombiesLock();
        {
            if(zombies[id].life!=0)
            {
                *line = zombies[id].line;
                *col = zombies[id].col;
            }
            else
            {
                *line = -1;
                *col = -1;
            }
        }
        zombiesUnlock();
    }
    else
    {
        humansLock();
        {
            if(humans[id].life!=0)
            {
                *line = humans[getGlobalId()*HUMAN_PER_PLAYER+id].line;
                *col = humans[getGlobalId()*HUMAN_PER_PLAYER+id].col;
            }
            else
            {
                *line = -1;
                *col = -1;
            }
        }
        humansUnlock();
    }
}

unsigned short getZombieMax()
{
    if(map==NULL)
        return 0;

    return map->zombie_max;
}

unsigned short getPlayerMax()
{
    if(map==NULL)
        return 0;

    return map->player_max;
}

unsigned short getMapWidth()
{
    if(map==NULL)
        return 0;

    return map->width;
}

unsigned short getMapHeight()
{
    if(map==NULL)
        return 0;

    return map->height;
}

void entityEat(short id)
{
    ask_t ask;
    if(id!=-1)
    {
        ask.type = ASK_EAT;
        ask.id = id;
        writeSock(&ask, sizeof(ask_t));
    }
}

void entityGet(short id)
{
    ask_t ask;
    if(id!=-1)
    {
        ask.type = ASK_GET;
        ask.id = id;
        writeSock(&ask, sizeof(ask_t));
    }
}

void entitySet(short id)
{
    ask_t ask;
    if(id!=-1)
    {
        ask.type = ASK_SET;
        ask.id = id;
        writeSock(&ask, sizeof(ask_t));
    }
}
