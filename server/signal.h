#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>             /* exit */
#include <signal.h>             /* sigXXX */
#include <stdio.h>              /* printf */
#include <sys/types.h>          /* pid_t */
#include <unistd.h>             /* getpid */


void initSignal();              /* initialise le signal SIGINT */

#endif
