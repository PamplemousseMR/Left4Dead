#ifndef __COMMUN_H__
#define __COMMUN_H__

#include <stdio.h>                          /* printf */
#include <stdlib.h>                         /* NULL */
#include <ncurses.h>                        /* WINDOW */
#include <fcntl.h>                          /* open */
#include <unistd.h>                         /* write */
#include <sys/stat.h>                       /* O_... S_... */

/* =============== Valeurs enregistrées dans le fichier binaire =============== */

#define EMPTY 0                             /* case vide */
#define HOME(x) (1+(x))                     /* repère */
#define HIDEOUT(x) (21+(x))                 /* cachette */
#define OBSTACLE 51                         /* obstacle */
#define CEMETERY 52                         /* cimetière */
#define TRAP 53                             /* piège */
#define ZOMBIE 54                           /* zombie */
#define FOOD 55                             /* nourriture */
#define AMMO 56                             /* munition */
#define HUMAN(X) (57+(X))                   /* humain */

/* =============== Valeur max et min de chaque atribut =============== */

#define HUMAN_PER_PLAYER 5                  /* nombre d'humains par joueur */
#define MAX_ZOMBIE_LIFE 2                   /* vie maximum d'un zombie */
#define MAX_HUMAN_LIFE 4                    /* vie maximum d'un humain */
#define HUMAN_LIFE_LOSS 60                  /* temp entre chaque perte de vie d'un humain */
#define HUMAN_MAX_CARRY 25                  /* nombre maximum que peut porter un humain */
#define HOME_DEFAULT_FOOD 5
#define HOME_DEFAULT_AMMO 5

#define UPDATE_MAP 600000000                /* temps d'attente entre chaque envoi de la map au client */
#define UPDATE_ZOMBIE 750000000             /* temps d'attente entre chaque 'pas' d'un zombie */
#define UPDATE_HUMAN 650000000              /* temps d'attente entre chaque 'pas' d'un humain */

#define MAX_PLAYER 20                       /* maximum de joueurs */
#define MAX_HIDEOUT 30                      /* maximum de cachettes */
#define MAX_FOOD_COUNT 1024                 /* nombre de sacs de nourriture à chaque spawn */
#define MAX_FOOD_ADD 1024                   /* nombre de nourriture dans chaque sac */
#define MAX_FOOD_SPAWN 1024                 /* nombre de sacs qui spawn toutes les minutes */
#define MAX_AMO_COUNT 1024                  /* nombre de sacs de munition à chaque spawn */
#define MAX_AMO_ADD 1024                    /* nombre de munitions dans chaque sac n */
#define MAX_AMO_SPAWN 1024                  /* nombre de sacs qui spawn toutes les minutes */
#define MAX_ZOMBIE_SPAWN 1024               /* nombre maximum de zombies qui peuvent apparaître en même temps */
#define MAX_ZOMBIE 20                       /* nombre max de zombie en jeu */
#define MAX_HEIGHT 100                      /* taille maximum de la carte en hauteur */
#define MAX_WIDTH 100                       /* taille maximum de la carte en largeur */

#define MIN_PLAYER 2
#define MIN_HIDEOUT 0
#define MIN_FOOD_COUNT 0
#define MIN_FOOD_ADD 0
#define MIN_FOOD_SPAWN 0
#define MIN_AMO_COUNT 0
#define MIN_AMO_ADD 0
#define MIN_AMO_SPAWN 0
#define MIN_ZOMBIE_SPAWN 1
#define MIN_ZOMBIE 2
#define MIN_HEIGHT 10
#define MIN_WIDTH 10

/* =============== Pour dessiner la carte sous ncurses ========== */

#define DRAW_EMPTY ' '                      /* dessiner du vide */
#define DRAW_CEMETERY ACS_PLMINUS           /* dessiner un cimetière */
#define DRAW_HIDEOUT ACS_PI                 /* dessiner une cachette */
#define DRAW_HOME ACS_BLOCK                 /* dessiner un repère */
#define DRAW_FOOD '+'                       /* dessiner de la nourriture */
#define DRAW_AMMO ACS_STERLING              /* dessiner un sac de munitions */
#define DRAW_ZOMBIE 'Z'                     /* dessiner un zombie */
#define DRAW_HUMAN 'H'                      /* dessiner un humain */

#define DRAW_OBSTACLE_COLOR 2               /* couleur des obstacles */
#define DRAW_EMPTY_COLOR 3                  /* couleur du vide */
#define DRAW_CEMETERY_COLOR 4               /* couleur des cimetières */
#define DRAW_FOOD_COLOR 5                   /* couleur de la nourriture */
#define DRAW_AMMO_COLOR 6                   /* couleur des munitions */
#define DRAW_ZOMBIE_COLOR 7                 /* couleur des zombies */
#define DRAW_HOME_COLOR(X) (8+(X)-1)        /* couleur des maisons */
#define DRAW_HUMAN_COLOR(X) (8+(X)-57+MAX_PLAYER)   /* couleur des humains */
#define DRAW_HIDEOUT_COLOR(X) (8+(X)-21+MAX_PLAYER+MAX_PLAYER) /* couleur des cachettes */

/* =============== Les erreurs de retour de la sauvegarde/chargement ========== */

#define NO_ERROR 1
#define ERROR_HEIGHT 2
#define ERROR_WIDTH 3
#define ERROR_NAX_PLAYER 4
#define ERROR_FOOD_SPAWN 5
#define ERROR_FOOD_COUNT 6
#define ERROR_FOOD_ADD 7
#define ERROR_AMO_SPAWN 8
#define ERROR_AMO_COUNT 9
#define ERROR_AMO_ADD 10
#define ERROR_NUM_ZOMBIE 11
#define ERROR_ZOMBIE_SPAWN 12
#define ERROR_TILE 13
#define ERROR_HOME 14
#define ERROR_OPEN 15

/* =============== Structures =============== */

typedef struct                              /* représente une caméra pour la carte */
{
    short col,line;
    unsigned short width,height;
} camera_t;

typedef struct                              /* represente la carte */
{
    unsigned short** map;
    unsigned short width;
    unsigned short height;
    unsigned short player_max;
    unsigned short food_spawn;
    unsigned short food_count;
    unsigned short food_add;
    unsigned short amo_spawn;
    unsigned short amo_count;
    unsigned short amo_add;
    unsigned short zombie_spawn;
    unsigned short zombie_max;
}map_t;

/* =============== Fonctions =============== */

map_t* createMap(unsigned short, unsigned short);
void deleteMap(map_t*);

unsigned short setHeight(map_t*, unsigned short);
unsigned short setWidth(map_t*, unsigned short);
unsigned short setNumPlayer(map_t*,unsigned short);
unsigned short setFoodSpawn(map_t*,unsigned short);
unsigned short setFoodCount(map_t*,unsigned short);
unsigned short setFoodAdd(map_t*,unsigned short);
unsigned short setAmoSpawn(map_t*,unsigned short);
unsigned short setAmoCount(map_t*,unsigned short);
unsigned short setAmoAdd(map_t*,unsigned short);
unsigned short setZombieSpawn(map_t*,unsigned short);
unsigned short setZombieMax(map_t*,unsigned short);

void initColor();
void drawMap(WINDOW*, map_t*, short, short, unsigned short, unsigned short);
short saveMap(map_t*,char*);
map_t* loadMap(char*,short*);
short checkMap(map_t*);

#endif
