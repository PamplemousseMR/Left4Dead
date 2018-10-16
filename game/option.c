#include "option.h"

key_t MSG_KEY = (key_t)DEFAULT_MSG_KEY;
key_t SEM_KEY = (key_t)DEFAULT_SEM_KEY;
key_t MEM_KEY = (key_t)DEFAULT_MEM_KEY;
int CLEAR = 0;
char FILE_NAME[MAX_NAME_SIZE] = DEFAULT_NAME;

static void printOptions()
{
    printf("Options :\n"
           "\th : afficher l'aide\n"
           "\tc [cle] : permet de specifier la cle de la file de messages\n"
           "\ts [cle] : permet de specifier la cle du tableau de semaphores\n"
           "\tm [cle] : permet de specifier la cle du segment de memoire partagee\n"
           "\tn [nom] : permet de specifier le nom de la map a charger\n"
           "\td : permet de specifier de nettoyer les IPC en cas de probleme lors d'une precedente creation\n");
}

void getOption(int argc,char* argv[])
{
    int c;
    while((c = getopt(argc, argv, "hc:s:m:dn:")) != -1)
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
        case 'n' :
            if(strlen(optarg) >= MAX_NAME_SIZE)
            {
                fprintf(stderr,"[getOption] Le nom a une taille superieure ou egale a %d\n",MAX_NAME_SIZE);
                exit(EXIT_FAILURE);
            }
            strcpy(FILE_NAME,optarg);
            break;
        case 'd' :
            CLEAR = 1;
            break;
        case '?' :
            fprintf(stderr,"[getOption] Option non reconnue\n");
            exit(EXIT_FAILURE);
            break;
        }
    }
}
