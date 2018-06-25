#include "items.h"

static unsigned short* foods = NULL;        /* le tableau qui contient le nombre de nouritures de chaque sac */
static unsigned short* ammos = NULL;        /* le tableau qui contient le nombre de munitions de chaque sac */
static items_value_t ammo;                  /* la structure donnée au thread de munitions */
static items_value_t food;                  /* la structure donnée au thread de nourritures */
static pthread_t thread_ammos;              /* le thread de munitons */
static pthread_t thread_foods;              /* le thread de nourritures */
static pthread_mutex_t mutex_foods;         /* le mutex pour le tableau de nourritures */
static pthread_mutex_t mutex_ammos;         /* le mutex pour le tableau de munitions */

static void foodsLock()
{
    if(pthread_mutex_lock(&mutex_foods) != 0)
    {
        fprintf(stderr,"[foodsLock] Erreur lors du vérouillage du mutex mutex_foods ");
        exit(EXIT_FAILURE);
    }
}

static void foodsUnlock()
{
    if(pthread_mutex_unlock(&mutex_foods) != 0)
    {
        fprintf(stderr,"[foosdUnlock] Erreur lors du dévérouillage du mutex mutex_foods ");
        exit(EXIT_FAILURE);
    }
}

static void ammosLock()
{
    if(pthread_mutex_lock(&mutex_ammos) != 0)
    {
        fprintf(stderr,"[ammosLock] Erreur lors du vérouillage du mutex mutex_ammos ");
        exit(EXIT_FAILURE);
    }
}

static void ammosUnlock()
{
    if(pthread_mutex_unlock(&mutex_ammos) != 0)
    {
        fprintf(stderr,"[ammosUnlock] Erreur lors du dévérouillage du mutex mutex_ammos ");
        exit(EXIT_FAILURE);
    }
}

/* ajoute des munitions sur la carte */
static void* updateAmmo(void* data)
{
    items_value_t* ammo = (items_value_t*)data;
    unsigned int seed;
    unsigned short line,col;
    unsigned short i;

    seed = rand();

    if(ammo->spawn!=0 && ammo->add!=0 && ammo->count!=0)
    {
        while(1)
        {
            for(i=0 ; i<ammo->count ; ++i)
            {
                line = rand_r(&seed)%GET_HEIGHT;
                col = rand_r(&seed)%GET_WIDTH;
                can(CAN_TILE(line,col));
                {
                    if(GET_TILE(line,col)==EMPTY)
                    {
                        ammosLock();
                        {
                            GET_TILE(line,col) = AMMO;
                            AMMOS(line,col) = ammo->add;
                        }
                        ammosUnlock();
                    }
                }
                go(CAN_TILE(line,col));
            }
            printf("[updateAmmo] ajout de munitions , attente : %d\n",ammo->spawn);

            /* attendre */
            sleep(ammo->spawn);
        }
    }
    pthread_exit(NULL);
}

/* ajout de la nourriture sur la carte */
static void* updateFood(void* data)
{
    items_value_t* food = (items_value_t*)data;
    unsigned int seed;
    unsigned short line,col;
    unsigned short i;

    seed = rand();

    if(food->spawn!=0  && food->add!=0 && food->count!=0)
    {
        while(1)
        {
            for(i=0 ; i<food->count ; ++i)
            {
                line = rand_r(&seed)%GET_HEIGHT;
                col = rand_r(&seed)%GET_WIDTH;
                can(CAN_TILE(line,col));
                {
                    if(GET_TILE(line,col)==EMPTY)
                    {
                        foodsLock();
                        {
                            GET_TILE(line,col) = FOOD;
                            FOODS(line,col) = food->add;
                        }
                        foodsUnlock();
                    }
                }
                go(CAN_TILE(line,col));
            }
            printf("[updateFood] ajout de nourriture , attente : %d\n",food->spawn);

            /* attendre */
            sleep(food->spawn);
        }
    }
    pthread_exit(NULL);
}

void initItems(map_t* map)
{
    unsigned short line,col;

    /* initialiser les strutures */
    ammo.spawn = map->amo_spawn;
    ammo.count = map->amo_count;
    ammo.add = map->amo_add;

    food.spawn = map->food_spawn;
    food.count = map->food_count;
    food.add = map->food_add;

    /* le tableau ou stocker les données */
    foods = (unsigned short*)malloc(sizeof(unsigned short)*GET_HEIGHT*GET_WIDTH);
    if(foods==NULL)
    {
        fprintf(stderr,"[initItems] Erreur lors de l'allocation dynamique de 'foods'\n");
        exit(EXIT_FAILURE);
    }
    ammos = (unsigned short*)malloc(sizeof(unsigned short)*GET_HEIGHT*GET_WIDTH);
    if(ammos==NULL)
    {
        fprintf(stderr,"[initItems] Erreur lors de l'allocation dynamique de 'ammos'\n");
        exit(EXIT_FAILURE);
    }

    /* initialiser les tableaux */
    for(line=0 ; line<GET_HEIGHT ; ++line)
    {
        for(col=0 ; col<GET_WIDTH ; ++col)
        {
            FOODS(line,col) = 0;
            AMMOS(line,col) = 0;
        }
    }

    /* crée les mutex */
    if(pthread_mutex_init(&mutex_ammos,NULL))
    {
        fprintf(stderr,"[update] Erreur lors de la creation du mutex ");
        exit(EXIT_FAILURE);
    }
    if(pthread_mutex_init(&mutex_foods,NULL))
    {
        fprintf(stderr,"[update] Erreur lors de la creation du mutex ");
        exit(EXIT_FAILURE);
    }

    /* crée les threads */
    if(pthread_create(&thread_ammos, NULL, updateAmmo, (void*)&ammo))
    {
        perror("[update] Erreur lors de la creation du thread updateAmmo ");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&thread_foods, NULL, updateFood, (void*)&food))
    {
        fprintf(stderr,"[update] Erreur lors de la creation du thread updateFood ");
        exit(EXIT_FAILURE);
    }

    printf("[updateItems] items lances\n");
}

void clearItems()
{
    void* result = NULL;

    /* annuler les threads */
    if(pthread_cancel(thread_ammos))
        fprintf(stderr,"[clearItems] Erreur lors de l'annulation du thread thread_ammo ");
    if(pthread_join(thread_ammos,&result))
        fprintf(stderr,"[clearItems] Erreur lors du join de thread_ammo ");
    if(result!=PTHREAD_CANCELED)
        fprintf(stderr,"[clearItems] Erreur, le thread thread_ammos n'a pas ete annulé\n");

    if(pthread_cancel(thread_foods))
        fprintf(stderr,"[clearItems] Erreur lors de l'annulation du thread thread_food ");
    if(pthread_join(thread_foods,&result))
        fprintf(stderr,"[clearItems] Erreur lors du join de thread_foods ");
    if(result!=PTHREAD_CANCELED)
        fprintf(stderr,"[clearItems] Erreur, le thread thread_foods n'a pas ete annulé\n");

    /* supprimer les mutex */
    if(pthread_mutex_destroy(&mutex_ammos))
        fprintf(stderr,"[clearItems] Erreur lors de la destruction de mutex_ammos ");
    if(pthread_mutex_destroy(&mutex_foods))
        fprintf(stderr,"[clearItems] Erreur lors de la destruction de mutex_foods ");

    /* libérer la memoire */
    if(ammos!=NULL)free(ammos);
    if(foods!=NULL)free(foods);

    printf("[clearItems] items supprimes\n");
}

unsigned short getAmmo(unsigned short line, unsigned short col, unsigned short want)
{
    unsigned short get = 0;

    if(line<0 || line>=GET_HEIGHT || col<0 || col>=GET_WIDTH)
        return 0;

    can(CAN_TILE(line,col));
    {
        ammosLock();
        {
            if(AMMOS(line,col)>=want)
            {
                AMMOS(line,col)-=want;
                get = want;
                if(AMMOS(line,col)==0)
                    GET_TILE(line,col) = EMPTY;
            }
            else
            {
                get = AMMOS(line,col);
                AMMOS(line,col) = 0;
                GET_TILE(line,col) = EMPTY;
            }
        }
        ammosUnlock();
    }
    go(CAN_TILE(line,col));
    return get;
}

unsigned short getFood(unsigned short line, unsigned short col, unsigned short want)
{
    unsigned short get = 0;

    if(line<0 || line>=GET_HEIGHT || col<0 || col>=GET_WIDTH)
        return 0;

    can(CAN_TILE(line,col));
    {
        foodsLock();
        {
            if(FOODS(line,col)>=want)
            {
                FOODS(line,col)-=want;
                get = want;
                if(FOODS(line,col)==0)
                    GET_TILE(line,col) = EMPTY;
            }
            else
            {
                get = FOODS(line,col);
                FOODS(line,col) = 0;
                GET_TILE(line,col) = EMPTY;
            }
        }
        foodsUnlock();
    }
    go(CAN_TILE(line,col));
    return get;
}
