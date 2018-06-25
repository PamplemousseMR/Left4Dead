#ifndef __UPDATEMAP_H__
#define __UPDATEMAP_H__

#include <stdlib.h>                                     /* NULL */
#include <pthread.h>                                    /* thread */

#include "connection.h"
#include "ncurses.h"

#include "../Commun/messagesStruct.h"
#include "../Commun/map.h"

/* =============== Fonctions =============== */

void initUpdate();                                      /* lance le thread qui met à jour la carte */
void updateMap();                                       /* met à jour la carte dans ncurses */
void updatePlayer();                                    /* met à jour l'affichage des joueurs */
void clearUpdate();                                     /* supprime le thread */
short getId(unsigned short,unsigned short);             /* retourne l'id de l'entité selectionnée */
unsigned short getTile(unsigned short,unsigned short);  /* retourne l'element de la carte */
void getEntityPos(unsigned short,int*,int*);            /* affecte la position d'un zombie */
unsigned short getZombieMax();                          /* retourne le nombre max de zombies */
unsigned short getPlayerMax();                          /* retourne le nombre max d'humains */
unsigned short getMapWidth();                           /* retourne la largeur de la carte */
unsigned short getMapHeight();                          /* retourne la hauteur de la carte */
void entityEat(short);
void entityGet(short);
void entitySet(short);
short getCameraLine();
short getCameraCol();
unsigned short getCameraWidth();
unsigned short getCameraHeight();
void setCameraLine(short);
void setCameraCol(short);

#endif
