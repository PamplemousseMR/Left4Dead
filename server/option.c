#include "option.h"

key_t MSG_KEY = (key_t)DEFAULT_MSG_KEY;
key_t SEM_KEY = (key_t)DEFAULT_SEM_KEY;
key_t MEM_KEY = (key_t)DEFAULT_MEM_KEY;
int PORT_TCP = DEFAULT_PORT_TCP;
int PORT_UDP = DEFAULT_PORT_UDP_SERVER;
char PIPE_NAME[MAX_PIPE_NAME_SIZE] = DEFAULT_PIPE_NAME;

static void printOptions()
{
    printf("Options :\n"
           "\th : afficher l'aide\n"
           "\tc [cle] : permet de specifier la cle de la file de messages\n"
           "\ts [cle] : permet de specifier la cle du tableau de semaphores\n"
           "\tm [cle] : permet de specifier la cle du segment de memoire partagee\n"
           "\tp [port] : permet de specifier la premmier port de connexion TCP\n"
           "\tu [port] : permet de specifier le port de connexion UDP\n"
           "\tn [nom] : permet de specifier nom du tube nommee\n");
}

void getOption(int argc,char* argv[])
{
    int c;
    while((c = getopt(argc, argv, "hc:s:m:p:u:n:")) != -1)
    {
        switch(c)
        {
        case 'h' :
            printOptions();
            exit(EXIT_SUCCESS);
            break;
        case 'c' :
            MSG_KEY = (key_t)strtol(optarg, NULL, 10);
            if(MSG_KEY <= 0)
            {
                fprintf(stderr,"[getOption] La cle de la file de messages ne doit pas etre <= 0\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 's' :
            SEM_KEY = (key_t)strtol(optarg, NULL, 10);
            if(SEM_KEY <= 0)
            {
                fprintf(stderr,"[getOption] La cle du tableau de semaphores ne doit pas etre <= 0\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'm' :
            MEM_KEY = (key_t)strtol(optarg, NULL, 10);
            if(MEM_KEY <= 0)
            {
                fprintf(stderr,"[getOption] La cle du segment de memoire partagee ne doit pas etre <= 0\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'p' :
            PORT_TCP = atoi(optarg);
            break;
        case 'u' :
            PORT_UDP = atoi(optarg);
            break;
        case 'n' :
            if(strlen(optarg)>=MAX_PIPE_NAME_SIZE)
            {
                fprintf(stderr,"[getOption] La taille du nom du tube est superieur ou egale a %d\n",MAX_PIPE_NAME_SIZE);
                exit(EXIT_FAILURE);
            }
            strcpy(PIPE_NAME,optarg);
            break;
        case '?' :
            fprintf(stderr,"[getOption] Option non reconnue\n");
            exit(EXIT_FAILURE);
            break;
        }
    }
}
