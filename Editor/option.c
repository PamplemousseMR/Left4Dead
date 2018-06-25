#include "option.h"

char FILE_NAME[MAX_NAME_SIZE] = DEFAULT_NAME;
char FILE_LOAD[MAX_NAME_SIZE] = DEFAULT_NAME;
char LOAD = 0;

static void printOptions()
{
    printf("Options :\n"\
           "\th : affiche l'aide\n"\
           "\tn : permet de specifier le nom du fichier de sauvegarde\n"
           "\tl : permet de specifier un fichier a charger\n");
}

void getOptions(int argc,char* argv[])
{
    int c;

    while((c = getopt(argc, argv, "hn:l:")) != -1)
    {
        switch(c)
        {
        case 'h' :
            printOptions();
            exit(EXIT_SUCCESS);
            break;
        case 'n' :
            if(strlen(optarg) >= MAX_NAME_SIZE)
            {
                fprintf(stderr,"[getOption] Le nom a une taille superieure ou egale a %d\n",MAX_NAME_SIZE);
                exit(EXIT_FAILURE);
            }
            strcpy(FILE_NAME,optarg);
            break;
        case 'l' :
            if(strlen(optarg) >= MAX_NAME_SIZE)
            {
                fprintf(stderr,"[getOption] Le nom a une taille superieure ou egale a %d\n",MAX_NAME_SIZE);
                exit(EXIT_FAILURE);
            }
            strcpy(FILE_LOAD,optarg);
            LOAD = 1;
            break;
        }
    }
}
