#include "move.h"

static pthread_t* threads = NULL;
static data_t* datas = NULL;

void cleanNode(void* node)
{
    deleteNode((node_t*)node);
}

void cleanList(void* list)
{
    deleteListWithNode((list_t*)list);
}


static void* sendMovement(void* data)
{
    list_t* list;
    ask_t ask;
    node_t* curr;
    node_t* start;
    unsigned int i;
    nodes_t* current;
    data_t infos = (*(data_t*)data);
    struct timespec tim, tim2;
    unsigned short fail = 0;

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    tim.tv_sec = 0;
    tim.tv_nsec = getType() == TYPE_ZOMBIE ? UPDATE_ZOMBIE : UPDATE_HUMAN;

    start = NULL;
    curr = NULL;
    list = NULL;

    pthread_cleanup_push(cleanNode,start);
    pthread_cleanup_push(cleanNode,curr);
    pthread_cleanup_push(cleanList,list);
    {
        start = createNode();
        curr = createNode();
        getEntityPos(infos.id,&start->line,&start->col);
        if(start->line==-1 || start->col==-1)
            pthread_exit(NULL);

        list = find(start,&infos.end);
        current = list->nodes;

        for(i=0 ; i<list->count && fail<MAX_ASTAR_FAIL; ++i)
        {
            ask.type = ASK_MOVE;
            ask.id = infos.id;
            ask.line = current->node->line;
            ask.col = current->node->col;
            writeSock(&ask, sizeof(ask_t));

            nanosleep(&tim,&tim2);

            /* vérifier que le mouvement a été fait*/
            getEntityPos(infos.id,&curr->line,&curr->col);

            /* erreur, l'entité est morte */
            if(curr->line==-1 || curr->col==-1)
                pthread_exit(NULL);

            /* si le mouvement n'a pas ete effectué */
            if(curr->line!=current->node->line || curr->col!=current->node->col)
            {
                ++fail;
                deleteList(list);
                list = NULL;
                list = find(curr,&infos.end);
                current = list->nodes;
                i=0;
                continue;
            }


            current=current->next;
        }

    }
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);

    pthread_exit(NULL);
}

void initMove()
{
    if(getType() == TYPE_ZOMBIE)
        threads = (pthread_t*)malloc(sizeof(pthread_t)*getZombieMax());
    else
        threads = (pthread_t*)malloc(sizeof(pthread_t)*HUMAN_PER_PLAYER);

    if(threads==NULL)
    {
        fprintf(stderr,"[initMove] Erreur lors de l'allocation dynamique de threads\n");
        exit(EXIT_FAILURE);
    }

    if(getType() == TYPE_ZOMBIE)
        datas = (data_t*)malloc(sizeof(data_t)*getZombieMax());
    else
        datas = (data_t*)malloc(sizeof(data_t)*HUMAN_PER_PLAYER);

    if(datas==NULL)
    {
        fprintf(stderr,"[initMove] Erreur lors de l'allocation dynamique de datas\n");
        exit(EXIT_FAILURE);
    }
}

void clearMove()
{
    unsigned short i;
    void* result;

    /* annuler les threads */
    if(threads!=NULL)
    {
        if(getType()==TYPE_ZOMBIE)
        {
            for(i=0 ; i<getZombieMax() ; ++i)
            {
                pthread_cancel(threads[i]);
                pthread_join(threads[i], &result);
            }
        }
        else
        {
            for(i=0 ; i<HUMAN_PER_PLAYER ; ++i)
            {
                pthread_cancel(threads[i]);
                pthread_join(threads[i], &result);
            }
        }
    }

    /* libération mémoire */
    if(threads!=NULL)
    {
        free(threads);
        threads=NULL;
    }
    if(datas!=NULL)
    {
        free(datas);
        datas=NULL;
    }
}

void entityMove(short id, node_t end)
{
    void* result;

    unsigned short max = HUMAN_PER_PLAYER;
    if(getType() == TYPE_ZOMBIE)
        max = getZombieMax();

    if(datas!=NULL && threads!=NULL && id>=0 && id<max && end.line>=0 && end.line<getMapHeight() && end.col>=0 && end.col<getMapWidth())
    {
        /* annuler le thread si il est déjà lancé */
        pthread_cancel(threads[id]);
        pthread_join(threads[id], &result);

        datas[id].end = end;
        datas[id].id = id;

        /* lancer le thread de mise à jour */
        if(pthread_create(&threads[id], NULL, sendMovement, (void*)(&datas[id])))
        {
            fprintf(stderr,"[entityMove] Erreur lors de la creation du thread ");
            exit(EXIT_FAILURE);
        }
    }
}

