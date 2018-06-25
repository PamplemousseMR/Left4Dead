#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>             /* exit */
#include <signal.h>             /* sigXXX */
#include <stdio.h>              /* printf */

/* =============== Fonctions =============== */

void initSignal();              /* initialise le signal SIGINT */

#endif
