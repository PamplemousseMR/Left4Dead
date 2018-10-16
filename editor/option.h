#ifndef __OPTION_H__
#define __OPTION_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>                         /* exit */
#include <stdio.h>                          /* fprintf */
#include <getopt.h>                         /* getopt */
#include <string.h>                         /* strcpy */

#include "../commun/default.h"

/* =============== Options =============== */

extern char FILE_NAME[MAX_NAME_SIZE];       /* le fichier de sauvegarde de la map */
extern char FILE_LOAD[MAX_NAME_SIZE];       /* le fichier à charger */
extern char LOAD;                           /* à 1 si un fichier doit être chargé */

/* =============== Fonctions =============== */

void getOptions(int argc,char* argv[]);     /* enregistre les options dans les variables 'extern' */

#endif
