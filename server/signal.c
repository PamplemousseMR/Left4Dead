#include "signal.h"

static pid_t pid = -1;

static void manager(int signum)
{
    if(signum == SIGINT && pid==getpid())
    {
        printf("[manager] signal SIGINT recu\n");
        exit(EXIT_SUCCESS);
    }
}

void initSignal()
{
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = manager;
    if(sigaction(SIGINT, &action, NULL) == -1)
    {
        perror("[initSignal] Erreur lors du positionnement du gestionnaire ");
        exit(EXIT_FAILURE);
    }
    pid = getpid();
}
