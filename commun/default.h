#ifndef __DEFAULT_H__
#define __DEFAULT_H__

#define DEFAULT_NAME  "DEFAULT_MAP.map"     /* le nom du fichier de sauvegarde par défaut */
#define MAX_NAME_SIZE 128                   /* la taille maximum du nom de fichier */

#define DEFAULT_MSG_KEY 1956                /* clé de la file de messages par défaut */
#define DEFAULT_SEM_KEY 1875                /* clé du tableau de sémaphores par défaut */
#define DEFAULT_MEM_KEY 1963                /* clé de la memoire partagée par défaut */

#define DEFAULT_PORT_UDP_SERVER 1024        /* le port de connexion UDP par défaut pour le serveur */
#define DEFAULT_PORT_UDP_PLAYER 1025        /* le port de connexion UDP par défaut pour le joueur */
#define DEFAULT_PORT_TCP 1026               /* le premier port de connexion par défaut */
#define DEFAULT_ADD_SERVER "0.0.0.0"        /* l'adresse du serveur par défaut */

#define DEFAULT_PIPE_NAME "FORK_PIPE"       /* le nom du tube par défaut */
#define MAX_PIPE_NAME_SIZE 15               /* la taille maximum du nom du tube */

#define MAX_ASTAR_ITERATION 10000           /* nombre max d'iterations pour l'algorithme A* */
#define MAX_ASTAR_FAIL 20                   /* nombre maximum de fois que le chemin peut changer en cours de route */

#endif
