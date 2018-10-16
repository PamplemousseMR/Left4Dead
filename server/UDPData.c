#include "UDPData.h"

static int sockfdreceive = -1;

void initUDPData()
{
    struct sockaddr_in serv;
    struct sockaddr_in add;
    send_port_t send_port;
    get_port_t get_port;
    int sockfdsend;
    socklen_t len = sizeof add;

    /* Création de la socket */
    if((sockfdreceive = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("[initUDPData] Erreur lors de la creation de la socket ");
        exit(EXIT_FAILURE);
    }
    memset(&add, 0, sizeof(struct sockaddr_in));

    /* Création de l'adresse du serveur */
    memset(&serv, 0, sizeof(struct sockaddr_in));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT_UDP);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Nommage de la socket */
    if(bind(sockfdreceive, (struct sockaddr*)&serv, sizeof(struct sockaddr_in)) == -1)
    {
        perror("[initUDPData] Erreur lors du nommage de la socket ");
        exit(EXIT_FAILURE);
    }

    /* Attente de requêtes des clients */
    while(1)
    {
        printf("[initUDPData] attente sur le port %d\n",PORT_UDP);
        if(recvfrom(sockfdreceive, &get_port, sizeof(get_port_t), 0, (struct sockaddr*)&add, &len) == -1)
            perror("[initUDPData] Erreur lors de la reception du message ");
        else
        {
            /* Création de la socket pour le client */
            if((sockfdsend = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
                perror("[initUDPData] Erreur lors de la creation de la socket ");
            else
            {
                /* Création de l'adresse du client */
                add.sin_port = htons(get_port.port);

                /* Envoi du message */
                memset(&send_port, 0, sizeof(send_port_t));
                send_port = getPort();
                if(sendto(sockfdsend, &send_port, sizeof(send_port_t), 0, (struct sockaddr*)&add, sizeof(struct sockaddr_in)) == -1)
                    perror("[initUDPData] Erreur lors de l'envoi du message ");

                /* Fermeture de la socket du client */
                else if(close(sockfdsend) == -1)
                    perror("[initUDPData] Erreur lors de la fermeture de la socket ");

                printf("[initUDPData] envoi du port %d a un client\n",send_port.port);
            }
        }
    }
}

void clearUDPData()
{
    if(close(sockfdreceive) == -1)
        perror("[clearUDPData] Erreur lors de la fermeture de la socket ");
}
