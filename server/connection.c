#include "connection.h"

static pid_t* childs = NULL;
static unsigned short array_size = 0;
static char* ports = NULL;
static pthread_t thread_child_receive;
static int pipefd = -1;

/* =============== fils =============== */

static int socketfd = -1;
static int sockclient = -1;
static int childid = -1;
static char ok = 1;
static zombie_t* zombies = NULL;
static human_t* humans = NULL;
static home_t* homes = NULL;
static pthread_t thread_child_receive;

static void managerChild(int signum)
{
    command_t command;
    void* result = NULL;

    if(signum == SIGUSR1)
    {
        printf("[managerChild %d] signal SIGUSR1 recu\n",childid);

        /* annulé le deplacement des zombies */
        if(childid==0)
        {
            memset(&command, 0, sizeof(command_t));
            command.type=COMMAND_MESSAGE;
            command.global_id=0;
            command.command_type=COMMAND_ZOMBIE_NO;
            if(msgsnd(MSG, &command, sizeof(command_t) - sizeof(long), 0) == -1)
                fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi du message\n",childid);
        }

        /* annuler le thread */
        if(pthread_cancel(thread_child_receive))
            fprintf(stderr,"[managerChild %d] Erreur lors de l'annulation du thread thread_child_receive\n",childid);
        if(pthread_join(thread_child_receive,&result))
            fprintf(stderr,"[managerChild %d] Erreur lors du join de thread_child_receive\n",childid);
        if(result!=PTHREAD_CANCELED)
            fprintf(stderr,"[managerChild %d] Erreur, le thread thread_child_receive n'a pas ete annulé\n",childid);

        /* Fermeture des sockets */
        if(close(sockclient) == -1)
            fprintf(stderr,"[managerChild %d] Erreur lors de la fermeture de la socket de communication\n",childid);
        if(close(socketfd) == -1)
            fprintf(stderr,"[managerChild %d] Erreur lors de la fermeture de la socket de connexion\n",childid);

        /* libération mémoire */
        if(zombies!=NULL)free(zombies);
        if(humans!=NULL)free(humans);
        if(homes!=NULL)free(homes);

        _exit(EXIT_SUCCESS);
    }
}

static void* receiveSocket(void* data)
{
    ask_t ask;
    command_t command;
    while(1)
    {
        memset(&command, 0, sizeof(command_t));

        if(recv(sockclient, &ask, sizeof(ask_t), 0) == -1)
        {
            fprintf(stderr,"[receiveSocket %d] Erreur lors de la reception\n",childid);
            ok = 0;
            pthread_exit(NULL);
        }
        else
        {
            if(ask.type==ASK_MOVE)
            {
                command.type=COMMAND_MESSAGE;
                command.global_id=childid;
                command.local_id=ask.id;
                command.line=ask.line;
                command.col=ask.col;
                command.command_type=COMMAND_MOVE;
                if(msgsnd(MSG, &command, sizeof(command_t) - sizeof(long), 0) == -1)
                    fprintf(stderr,"[receiveSocket %d] Erreur lors de l'envoi du message\n",childid);
                else
                    printf("[receiveSocket %d] move id(%d,%d) to(%d,%d)\n",childid,command.global_id,command.local_id,command.line,command.col);
            }
            else if(ask.type==ASK_DISCONECT)
            {
                ok = 0;
                printf("[receiveSocket %d] deconnexion\n",childid);
                pthread_exit(NULL);
            }
            else if(ask.type==ASK_EAT)
            {
                command.type=COMMAND_MESSAGE;
                command.global_id=childid;
                command.local_id=ask.id;
                command.command_type=COMMAND_EAT;
                if(msgsnd(MSG, &command, sizeof(command_t) - sizeof(long), 0) == -1)
                    fprintf(stderr,"[receiveSocket %d] Erreur lors de l'envoi du message\n",childid);
                else
                    printf("[receiveSocket %d] mange id(%d,%d)\n",childid,command.global_id,command.local_id);
            }
            else if(ask.type==ASK_GET)
            {
                command.type=COMMAND_MESSAGE;
                command.global_id=childid;
                command.local_id=ask.id;
                command.command_type=COMMAND_GET;
                if(msgsnd(MSG, &command, sizeof(command_t) - sizeof(long), 0) == -1)
                    fprintf(stderr,"[receiveSocket %d] Erreur lors de l'envoi du message\n",childid);
                else
                    printf("[receiveSocket %d] prendre id(%d,%d)\n",childid,command.global_id,command.local_id);
            }
            else if(ask.type==ASK_SET)
            {
                command.type=COMMAND_MESSAGE;
                command.global_id=childid;
                command.local_id=ask.id;
                command.command_type=COMMAND_SET;
                if(msgsnd(MSG, &command, sizeof(command_t) - sizeof(long), 0) == -1)
                    fprintf(stderr,"[receiveSocket %d] Erreur lors de l'envoi du message\n",childid);
                else
                    printf("[receiveSocket %d] laisser id(%d,%d)\n",childid,command.global_id,command.local_id);
            }
            else
                fprintf(stderr,"[receiveSocket %d] Erreur lors de la reception, type inconnu\n",childid);
        }
    }
    pthread_exit(NULL);
}

static void waitPlayer(unsigned int id)
{
    struct sockaddr_in servaddresse;
    struct sigaction action;
    set_port_t set_port;
    map_size_t map_size;
    struct timespec tim, tim2;
    unsigned short max_zombie,max_player;
    unsigned short i,j;
    command_t command;

    childid = id;

    /* crée les données de zombie et d'humains à envoyer au client */
    zombies = (zombie_t*)malloc(sizeof(zombie_t)*GET_MAX_ZOMBIE);
    if(zombies==NULL)
    {
        fprintf(stderr,"[waitPlayer %d] Erreur lors du malloc de zombies\n",childid);
        _exit(EXIT_FAILURE);
    }

    humans = (human_t*)malloc(sizeof(human_t)*GET_MAX_PLAYER*HUMAN_PER_PLAYER);
    if(humans==NULL)
    {
        fprintf(stderr,"[waitPlayer %d] Erreur lors du malloc de humans\n",childid);
        _exit(EXIT_FAILURE);
    }

    homes = (home_t*)malloc(sizeof(home_t));
    if(homes==NULL)
    {
        fprintf(stderr,"[waitPlayer %d] Erreur lors du malloc de homes\n",childid);
        _exit(EXIT_FAILURE);
    }

    /* le timespec pour le sleep */
    tim.tv_sec = 0;
    tim.tv_nsec = UPDATE_MAP;

    /* mise en place du gestionnaire pour l'arrêt du fils */
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = managerChild;
    if(sigaction(SIGUSR1, &action, NULL) == -1)
    {
        fprintf(stderr,"[waitPlayer %d] Erreur lors du positionnement du gestionnaire\n",childid);
        _exit(EXIT_FAILURE);
    }

    /* Création de la socket */
    if((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        fprintf(stderr,"[waitPlayer %d] Erreur lors de la creation de la socket\n",childid);
        _exit(EXIT_FAILURE);
    }

    /* Création de l'adresse du serveur */
    memset(&servaddresse, 0, sizeof(struct sockaddr_in));
    servaddresse.sin_family = AF_INET;
    servaddresse.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddresse.sin_port = htons(PORT_TCP+childid);

    /* Nommage de la socket */
    if(bind(socketfd, (struct sockaddr*)&servaddresse, sizeof(struct sockaddr_in)) == -1)
    {
        fprintf(stderr,"[waitPlayer%d ] Erreur lors du nommage de la socket\n",childid);
        _exit(EXIT_FAILURE);
    }

    /* Ouverture du tube */
    if((pipefd = open(PIPE_NAME, O_WRONLY)) == -1)
    {
        fprintf(stderr, "[waitPlayer %d] Erreur lors de l'ouverture du tube '%s'", childid, PIPE_NAME);
        _exit(EXIT_FAILURE);
    }

    /* boucle lors d'une déconnexion */
    while(1)
    {
        /* remise à zero */
        ok = 1;
        memset(&command, 0, sizeof(command_t));
        memset(&set_port, 0, sizeof(set_port_t));

        /* Mise en mode passif de la socket */
        if(listen(socketfd, 1) == -1)
        {
            fprintf(stderr,"[waitPlayer %d] Erreur lors de la mise en mode passif\n",childid);
            _exit(EXIT_FAILURE);
        }

        if(childid==0)
        {
            command.type=COMMAND_MESSAGE;
            command.global_id=0;
            command.command_type=COMMAND_ZOMBIE_YES;
            if(msgsnd(MSG, &command, sizeof(command_t) - sizeof(long), 0) == -1)
                fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi du message\n",childid);
        }

        /* Attente d'une connexion */
        if((sockclient = accept(socketfd, NULL, NULL)) == -1)
        {
            fprintf(stderr,"[waitPlayer %d] Erreur lors de la demande de connexion\n",childid);
            _exit(EXIT_FAILURE);
        }

        if(childid>0)
        {
            command.type=COMMAND_MESSAGE;
            command.global_id=childid;
            command.command_type=COMMAND_HUMAN_YES;
            if(msgsnd(MSG, &command, sizeof(command_t) - sizeof(long), 0) == -1)
                fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi du message\n",childid);
        }

        if(childid==0)
        {
            command.type=COMMAND_MESSAGE;
            command.global_id=0;
            command.command_type=COMMAND_ZOMBIE_NO;
            if(msgsnd(MSG, &command, sizeof(command_t) - sizeof(long), 0) == -1)
                fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi du message\n",childid);
        }

        /* crée le thread */
        if(pthread_create(&thread_child_receive, NULL, receiveSocket, NULL))
        {
            fprintf(stderr,"[waitPlayer %d] Erreur lors de la creation du thread\n",childid);
            _exit(EXIT_FAILURE);
        }

        /* envoi de l'occupation du port */
        set_port.id = childid;
        set_port.occupied = 1;
        if(write(pipefd, &set_port, sizeof(set_port_t)) == -1)
        {
            fprintf(stderr,"[waitPlayer %d] Erreur lors de l'ecriture du message dans le tube\n",childid);
            _exit(EXIT_FAILURE);
        }

        printf("[waitPlayer %d] un client c'est connecté\n",childid);

        /* envoi de la taille de la map */
        map_size.width = GET_WIDTH;
        map_size.height = GET_HEIGHT;
        if(send(sockclient, &map_size, sizeof(map_size_t),0) == -1)
        {
            fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi de la taille ",childid);
            _exit(EXIT_FAILURE);
        }

        /* envoi du nombre de zombies */
        max_zombie = GET_MAX_ZOMBIE;
        if(send(sockclient, &max_zombie, sizeof(unsigned short),0) == -1)
        {
            fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi du nombre de zombie ",childid);
            _exit(EXIT_FAILURE);
        }

        /* envoi du nombre d'humains */
        max_player = GET_MAX_PLAYER;
        if(send(sockclient, &max_player, sizeof(unsigned short),0) == -1)
        {
            fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi du nombre de zombie ",childid);
            _exit(EXIT_FAILURE);
        }

        while(ok)
        {
            can(CAN_ZOMBIE);
            {
                for(i=0 ; i<GET_MAX_ZOMBIE ; ++i)
                {
                    zombies[i].line = GET_ZOMBIE_LINE(i);
                    zombies[i].col = GET_ZOMBIE_COL(i);
                    zombies[i].life = GET_ZOMBIE_LIFE(i);
                }
            }
            go(CAN_ZOMBIE);

            for(i=0 ; i<GET_MAX_PLAYER ; ++i)
            {
                can(CAN_HUMAN(i));
                {
                    for(j=0 ; j<HUMAN_PER_PLAYER ; ++j)
                    {
                        humans[i*HUMAN_PER_PLAYER+j].line = GET_HUMAN_LINE(i,j);
                        humans[i*HUMAN_PER_PLAYER+j].col = GET_HUMAN_COL(i,j);
                        humans[i*HUMAN_PER_PLAYER+j].life = GET_HUMAN_LIFE(i,j);
                        humans[i*HUMAN_PER_PLAYER+j].ammos = GET_HUMAN_AMMOS(i,j);
                        humans[i*HUMAN_PER_PLAYER+j].foods = GET_HUMAN_FOODS(i,j);
                    }
                }
                go(CAN_HUMAN(i));
            }

            if(childid!=0)
            {
                can(CAN_HOME(childid-1));
                homes->ammo=GET_HOME_AMMO(childid-1);
                homes->foods=GET_HOME_FOOD(childid-1);
                go(CAN_HOME(childid-1));
            }

            can(0);
            can(1);
            can(2);
            can(3);
            {
                if(send(sockclient, &GET_TILE(0,0), sizeof(unsigned short)*GET_HEIGHT*GET_WIDTH,0) == -1)
                {
                    fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi de la map ",childid);
                    ok = 0;
                }
            }
            go(3);
            go(2);
            go(1);
            go(0);

            if(ok && send(sockclient, zombies, sizeof(zombie_t)*GET_MAX_ZOMBIE,0) == -1)
            {
                fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi des zombies ",childid);
                ok = 0;
                continue;
            }

            if(ok && send(sockclient, humans, sizeof(human_t)*GET_MAX_PLAYER*HUMAN_PER_PLAYER,0) == -1)
            {
                fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi des humains ",childid);
                ok = 0;
                continue;
            }

            if(childid!=0)
            {
                if(ok && send(sockclient, homes, sizeof(home_t),0) == -1)
                {
                    fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi des reperes ",childid);
                    ok = 0;
                    continue;
                }
            }

            nanosleep(&tim,&tim2);
        }

        if(childid>0)
        {
            command.type=COMMAND_MESSAGE;
            command.global_id=childid;
            command.command_type=COMMAND_HUMAN_NO;
            if(msgsnd(MSG, &command, sizeof(command_t) - sizeof(long), 0) == -1)
                fprintf(stderr,"[waitPlayer %d] Erreur lors de l'envoi du message\n",childid);
        }

        /* envoi du relachement du port */
        set_port.id = childid;
        set_port.occupied = 0;
        if(write(pipefd, &set_port, sizeof(set_port_t)) == -1)
        {
            fprintf(stderr,"[waitPlayer %d] Erreur lors de l'ecriture du message dans le tube\n",childid);
            _exit(EXIT_FAILURE);
        }

        printf("[waitPlayer %d] un client s'est deconnecté\n",childid);
    }

    _exit(EXIT_SUCCESS);
}

/* =============== père =============== */

static void* receiveMessages(void* data)
{
    set_port_t set_port;
    while(1)
    {
        if(read(pipefd, &set_port, sizeof(set_port_t)) == -1)
        {
            perror("[receiveMessages] Erreur lors de la lecture du message dans le tube ");
            exit(EXIT_FAILURE);
        }
        ports[set_port.id] = set_port.occupied;
        printf("[receiveMessages] le port %d a l'etat %d\n",set_port.id+PORT_TCP,set_port.occupied);
    }
    pthread_exit(NULL);
}

void initConnections()
{
    unsigned short i;
    array_size = GET_MAX_PLAYER+1;

    /* crée le tableau de fils */
    childs = (pid_t*)malloc(sizeof(pid_t)*array_size);
    if(childs == NULL)
    {
        fprintf(stderr,"[initConnections] Erreur lors du malloc des fils\n");
        exit(EXIT_FAILURE);
    }

    /* crée le tableau d'occupation des ports */
    ports = (char*)malloc(sizeof(char)*array_size);
    if(ports == NULL)
    {
        fprintf(stderr,"[initConnections] Erreur lors du malloc de l'occupation des port\n");
        exit(EXIT_FAILURE);
    }
    for(i=0 ; i<array_size ; ++i)
        ports[i] = 0;

    /* Création du tube */
    if(mkfifo(PIPE_NAME, S_IRUSR | S_IWUSR) == -1)
    {
        if(errno != EEXIST) {
            fprintf(stderr, "[initConnections]  Erreur lors de la creation du tube '%s'", PIPE_NAME);
            perror(" ");
            exit(EXIT_FAILURE);
        }
        else
            fprintf(stderr, "[initConnections] Le tube '%s' existe deja.\n", PIPE_NAME);
    }

    /* crée les fils */
    for(i=0 ; i<array_size ; ++i)
    {
        if((childs[i] = fork()) == -1)
        {
            perror("[initConnections] Erreur lors de la creation du processus fils ");
            exit(EXIT_FAILURE);
        }
        if(childs[i] == 0)
            waitPlayer(i);
    }

    /* Ouverture du tube */
    if((pipefd = open(PIPE_NAME, O_RDONLY)) == -1)
    {
        fprintf(stderr, "[initConnections] Erreur lors de l'ouverture du tube '%s'", PIPE_NAME);
        perror(" ");
        exit(EXIT_FAILURE);
    }

    /* crée le thread qui attends les messages dans le tube */
    if(pthread_create(&thread_child_receive, NULL, receiveMessages, NULL))
    {
        perror("[initConnections] Erreur lors de la creation du thread\n");
        exit(EXIT_FAILURE);
    }

    printf("[initConnections] fils de socket cree\n");
}

void clearConnections()
{
    unsigned short i;

    /* Suppression des fils */
    if(childs!=NULL)
    {
        for(i=0 ; i<array_size ; ++i)
        {
            if(kill(childs[i], SIGUSR1) == -1)
                perror("[clearConnections] Erreur lors de l'envoi de SIGINT au fils ");
        }
        free(childs);
    }

    /* Suppression du thread */
    if (pthread_cancel(thread_child_receive) != 0)
        perror("[clearConnections] Erreur lors de l'annulation du thread ");

    /* Fermeture du tube */
    if(close(pipefd) == -1)
    {
        perror("[clearConnections] Erreur lors de la fermeture du tube ");
        exit(EXIT_FAILURE);
    }

    /* Suppression du tube */
    if(unlink(PIPE_NAME) == -1)
    {
        if(errno != ENOENT)
        {
            fprintf(stderr, "[clearConnections] Erreur lors de la suppression du tube '%s'", PIPE_NAME);
            perror(" ");
            exit(EXIT_FAILURE);
        }
    }

    if(ports!=NULL)free(ports);
    if(childs!=NULL)free(childs);

    printf("[clearConnections] connexions supprimees\n");
}

send_port_t getPort()
{
    unsigned short i;
    send_port_t send_port;
    memset(&send_port, 0, sizeof(send_port_t));
    for(i=0 ; i<array_size ; ++i)
        if(ports[i]==0)
        {
            send_port.port = PORT_TCP+i;
            send_port.type = i==0 ? TYPE_ZOMBIE : TYPE_HUMAN;
            send_port.global_id = i;
            return send_port;
        }
    send_port.port = -1;
    return send_port;
}
