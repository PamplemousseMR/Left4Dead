#include "connection.h"

static int sockfd = -1;
static PLAYER_TYPE type = TYPE_HUMAN;
static unsigned short global_id;

void initConnections()
{
    int sockfdsend;
    int sockfdreceive;
    struct sockaddr_in serv;
    struct sockaddr_in pla;
    struct sockaddr_in servTCP;
    send_port_t send_port;
    get_port_t data;

    /* Création de la socket de réception */
    if((sockfdreceive = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("[initConnections] Erreur lors de la creation de la socket de reception UDP ");
        exit(EXIT_FAILURE);
    }

    /* Création de l'adresse de réception */
    memset(&pla, 0, sizeof(struct sockaddr_in));
    pla.sin_family = AF_INET;
    pla.sin_port = htons(PORT_UDP_PLAYER);
    pla.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Nommage de la socket de réception */
    if(bind(sockfdreceive, (struct sockaddr*)&pla, sizeof(struct sockaddr_in)) == -1)
    {
        perror("[initConnections] Erreur lors du nommage de la socket de reception UDP ");
        exit(EXIT_FAILURE);
    }

    /* Création de la socket d'envoi */
    if((sockfdsend = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("[initConnections] Erreur lors de la creation de la socket d'envoi UDP ");
        exit(EXIT_FAILURE);
    }

    /* Création de l'adresse d'envoi */
    memset(&serv, 0, sizeof(struct sockaddr_in));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT_UDP_SERVER);
    serv.sin_addr = ADD_SERVER;

    /* Envoi du message */
    data.port = PORT_UDP_PLAYER;
    if(sendto(sockfdsend, &data, sizeof(get_port_t), 0, (struct sockaddr*)&serv, sizeof(struct sockaddr_in)) == -1)
    {
        perror("[initConnections] Erreur lors de l'envoi du message ");
        exit(EXIT_FAILURE);
    }

    /* réception du message */
    if(recvfrom(sockfdreceive, &send_port, sizeof(send_port_t), 0, NULL, NULL) == -1)
    {
        perror("[initConnections] Erreur lors de la reception du message ");
        exit(EXIT_FAILURE);
    }

    /* Fermeture des sockets */
    if(close(sockfdsend) == -1)
    {
        perror("[initConnections] Erreur lors de la fermeture de la socket de d'envoi UDP ");
        exit(EXIT_FAILURE);
    }
    if(close(sockfdreceive) == -1)
    {
        perror("[initConnections] Erreur lors de la fermeture de la socket de reception UDP ");
        exit(EXIT_FAILURE);
    }

    /* vérification de la validité du port */
    if(send_port.port == -1)
    {
        printf("[initConnections] Le serveur est plein\n");
        exit(EXIT_SUCCESS);
    }
    type = send_port.type;
    global_id = send_port.global_id-1;

    /* Création de la socket TCP */
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        perror("[initConnections] Erreur lors de la creation de la socket TCP ");
        exit(EXIT_FAILURE);
    }

    /* Remplissage de la structure */
    memset(&servTCP, 0, sizeof(struct sockaddr_in));
    servTCP.sin_family = AF_INET;
    servTCP.sin_addr = ADD_SERVER;
    servTCP.sin_port = htons(send_port.port);

    /* Connexion au serveur */
    if(connect(sockfd, (struct sockaddr*)&servTCP, sizeof(servTCP)) == -1)
    {
        perror("[initConnections] Erreur lors de la connexion au socket TCP ");
        exit(EXIT_FAILURE);
    }
}

void clearConnections()
{
    ask_t ask;

    /* envoi de la déconnection */
    ask.type = ASK_DISCONECT;
    if(send(sockfd, &ask, sizeof(ask_t)+1,0) == -1)
        perror("[clearConnections] Erreur lors de l'envoi de la demande' ");

    /* Fermeture des sockets */
    if(close(sockfd) == -1)
    {
        perror("[clearConnections] Erreur lors de la fermeture de la socket TCP ");
        exit(EXIT_FAILURE);
    }
}

void readSock(void* buf,size_t count)
{
    if(recv(sockfd, buf, count,0) == -1)
        perror("[readSock] Erreur lors de la lecture ");
}

void writeSock(void* buf, size_t count)
{
    if(send(sockfd, buf, count,0) == -1)
        perror("[writeSock] Erreur lors de l'envoi ");
}

PLAYER_TYPE getType()
{
    return type;
}

unsigned short getGlobalId()
{
    return global_id;
}
