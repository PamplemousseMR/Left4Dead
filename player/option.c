#include "option.h"

struct in_addr ADD_SERVER;
int PORT_UDP_SERVER = DEFAULT_PORT_UDP_SERVER;
int PORT_UDP_PLAYER = DEFAULT_PORT_UDP_PLAYER;

static void printOptions()
{
    printf("Options :\n"
           "\th : afficher l'aide\n"
           "\tp [port] : permet de specifier le port de connexion du serveur\n"
           "\tu [port] : permet de specifier le port de connexion du joueur\n"
           "\ta [adresse] : permet de specifier l'addresse du server\n");
}

void getOption(int argc,char* argv[])
{
    int c;

    if(inet_pton(AF_INET, DEFAULT_ADD_SERVER, &ADD_SERVER) != 1)
    {
        perror("[getOptions] Erreur lors de la conversion de l'adresse ");
        exit(EXIT_FAILURE);
    }
    while((c = getopt(argc, argv, "hp:u:a:")) != -1)
    {
        switch(c)
        {
        case 'h' :
            printOptions();
            exit(EXIT_SUCCESS);
            break;
        case 'p' :
            PORT_UDP_SERVER = atoi(optarg);
            break;
        case 'u' :
            PORT_UDP_PLAYER = atoi(optarg);
            break;
        case 'a' :
            if(inet_pton(AF_INET, optarg, &ADD_SERVER) != 1)
            {
                perror("[getOptions] Erreur lors de la conversion de l'adresse du serveur ");
                exit(EXIT_FAILURE);
            }
            break;
        case '?' :
            fprintf(stderr,"[getOption] Option non reconnue\n");
            exit(EXIT_FAILURE);
            break;
        }
    }
}
