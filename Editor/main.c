#include <stdlib.h>                                 /* Pour exit, EXIT_SUCCESS, atexit */
#include <unistd.h>                                 /* Pour _exit */
#include <ncurses.h>                                /* MEVENT */

#include "ncurses.h"
#include "signal.h"
#include "option.h"

#include "../Commun/map.h"

/* =============== Variables =============== */

static map_t* map = NULL;                           /* La map en mémoire */
#define buffer_size 10                              /* taille max du buffer pour les saisies clavier */
static char val_buffer[buffer_size];                /* le buffer servant à stocker les saisies clavier*/
static unsigned short val_pos = 0;                  /* le curseur du buffer */
static unsigned short network = 0;                  /* le réseau de cachettes actuel */
static unsigned short home = 0;                     /* le réseau de repères actuel */

/* =============== Fonctions =============== */

static char getCamInteraction(char c,camera_t*);    /* renvoie true si une interaction avec la caméra a été faite */
static char getPropertieChange(int);                /* renvoie true si une interaction avec les propriétés a été faite */
static char getHideoutNet(int);                     /* renvoie true si une interaction avec le réseau a été faite */
static void updateShow(camera_t*);                  /* met à jour la map */
static void updateProperties();                     /* met à jour les propriétés */
static void updateTools();                          /* met à jour les outils */
static void clearBuffer();                          /* vide le buffer */
static void update();                               /* la boucle principale */
static void clearProg();                            /* appelé à la fin du programme pour nettoyer la mémoire */

/* =============== Interactions clavier/souris =============== */

static char getCamInteraction(char c,camera_t* cam)
{
    switch(c)
    {
    case 'z' :
        if(cam->line < map->height-cam->height/2) cam->line++;
        return 1;
        break;
    case 's' :
        if(cam->line > -cam->height/2) cam->line--;
        return 1;
        break;
    case 'd' :
        if(cam->col > -cam->width/2) cam->col--;
        return 1;
        break;
    case 'q' :
        if(cam->col < map->width-cam->width/2) cam->col++;
        return 1;
        break;
    }
    return 0;
}

static char getPropertieChange(int c)
{
    switch(c)
    {
    case '0':
        if(val_pos>0)
        {
            val_buffer[val_pos] = '0';
            if(val_pos<buffer_size-1)val_pos++;
        }
        break;
    case '1':
        val_buffer[val_pos] = '1';
        if(val_pos<buffer_size-1)val_pos++;
        break;
    case '2':
        val_buffer[val_pos] = '2';
        if(val_pos<buffer_size-1)val_pos++;
        break;
    case '3':
        val_buffer[val_pos] = '3';
        if(val_pos<buffer_size-1)val_pos++;
        break;
    case '4':
        val_buffer[val_pos] = '4';
        if(val_pos<buffer_size-1)val_pos++;
        break;
    case '5':
        val_buffer[val_pos] = '5';
        if(val_pos<buffer_size-1)val_pos++;
        break;
    case '6':
        val_buffer[val_pos] = '6';
        if(val_pos<buffer_size-1)val_pos++;
        break;
    case '7':
        val_buffer[val_pos] = '7';
        if(val_pos<buffer_size-1)val_pos++;
        break;
    case '8':
        val_buffer[val_pos] = '8';
        if(val_pos<buffer_size-1)val_pos++;
        break;
    case '9':
        val_buffer[val_pos] = '9';
        if(val_pos<buffer_size-1)val_pos++;
        break;
    case KEY_BACKSPACE :
        if(val_pos>0)val_pos--;
        val_buffer[val_pos] = '\0';
        break;
    case '\n':
        return 1;
        break;
    }
    return 0;
}

static char getHideoutNet(int c)
{
    switch(c)
    {
    case '+':
        if(TOOL==HIDEOUT_TOOL)
        {
            if(network<MAX_HIDEOUT-1)
                network++;
        }
        else if(TOOL==HOME_TOOL)
        {
            if(home<MAX_PLAYER-1)
                home++;
        }
        return 1;
        break;
    case '-':
        if(TOOL==HIDEOUT_TOOL)
        {
            if(network>0)
                network--;
        }else if(TOOL==HOME_TOOL)
        {
            if(home>0)
                home--;
        }
        return 1;
        break;
    }
    return 0;
}

/* =============== Mise à jour =============== */

static void updateShow(camera_t* cam)
{
    wclear(SHOW);
    drawMap(SHOW,map,cam->line,cam->col,cam->height,cam->width);
    wrefresh(SHOW);
}

static void updateProperties()
{
    wclear(PROPERTIES);
    drawProperties(map->player_max, map->food_spawn, map->food_count, map->food_add, map->amo_spawn, map->amo_count, map->amo_add, map->zombie_spawn, map->zombie_max, map->height, map->width);
    wrefresh(PROPERTIES);
}

static void updateTools()
{
    wclear(TOOLS);
    drawTools(network+1,home+1);
    wrefresh(TOOLS);
}

static void clearBuffer()
{
    unsigned short i;
    for(i=0 ; i<buffer_size ; ++i)
        val_buffer[i] = '\0';
    val_pos = 0;
}


/* =============== Boucle principale =============== */

static void update()
{
    camera_t cam;
    int c;
    char ok;
    unsigned short col,line;
    MEVENT event;
    short error;

    /* placer la camera */
    cam.height = SHOW_LINE;
    cam.width = SHOW_COL;
    cam.col = map->width/2 - cam.width/2;
    cam.line = map->height/2 - cam.height/2;

    /* faire un premier affichage */
    updateShow(&cam);
    updateTools();
    updateProperties();
    clearBuffer();

    /* boucle d'attente d'évènements */
    c = '\0';
    ok = 1;
    while(ok)
    {
        c = getch();
        if(c == KEY_MOUSE) /* Évènement souris */
        {
            if(getmouse(&event) == OK)
            {
                if(getSelectedTool(c,&event)) /* clic sur un outil */
                    updateTools();
                else if(getSelectedProperty(c,&event)) /* clic sur une proprieté */
                {
                    clearBuffer();
                    updateProperties();
                }
                else if(getSelectedTile(c,&line,&col,&event)) /* clic dans la carte */
                {
                    col = col+cam.col-1;
                    line = line+cam.line-1;
                    if(col>=0 && col<map->width && line>=0 && line<map->height)
                    {
                        switch(TOOL)
                        {
                        case ERASE_TOOL:
                            map->map[line][col] = EMPTY;
                            break;
                        case CEMETERY_TOOL:
                            map->map[line][col] = CEMETERY;
                            break;
                        case OBSTACLE_TOOL:
                            map->map[line][col] = OBSTACLE;
                            break;
                        case HIDEOUT_TOOL:
                            map->map[line][col] = HIDEOUT(network);
                            break;
                        case HOME_TOOL:
                            map->map[line][col] = HOME(home);
                            break;
                        }
                        updateShow(&cam);
                    }
                }
                else if(getSelectedButton(c,&event)) /* clic sur un bouton */
                {
                    if(BUTTON == QUIT)
                        ok = 0;
                    if(BUTTON == SAVE)
                    {
                        wclear(LOG);
                        if((error = saveMap(map,FILE_NAME))!=1)
                                wprintw(LOG,"SAVE FAIL : %d\n",error);
                        else
                            wprintw(LOG,"SAVE TO : %s\n",FILE_NAME);
                        wrefresh(LOG);
                    }

                }
                clearBuffer();
            }
        }
        else if(getCamInteraction(c,&cam)) /* touches Z, Q, S, D */
        {
            clearBuffer();
            updateShow(&cam);
        }
        else if(getPropertieChange(c)) /* pavé numérique */
        {
            switch(PROPERTIE)
            {
            case PLAYER_MAX:
                setNumPlayer(map,atoi(val_buffer));
                break;
            case FOOD_SPAWN:
                setFoodSpawn(map,atoi(val_buffer));
                break;
            case FOOD_COUNT:
                setFoodCount(map,atoi(val_buffer));
                break;
            case FOOD_ADD:
                setFoodAdd(map,atoi(val_buffer));
                break;
            case AMO_SPAWN:
                setAmoSpawn(map,atoi(val_buffer));
                break;
            case AMO_COUNT:
                setAmoCount(map,atoi(val_buffer));
                break;
            case AMO_ADD:
                setAmoAdd(map,atoi(val_buffer));
                break;
            case ZOMBIE_SPAWN:
                setZombieSpawn(map,atoi(val_buffer));
                break;
            case ZOMBIE_MAX:
                setZombieMax(map,atoi(val_buffer));
                break;
            case MAP_HEIGHT:
                setHeight(map,atoi(val_buffer));
                cam.col = map->width/2 - cam.width/2;
                cam.line = map->height/2 - cam.height/2;
                updateShow(&cam);
                break;
            case MAP_WIDTH:
                setWidth(map,atoi(val_buffer));
                cam.col = map->width/2 - cam.width/2;
                cam.line = map->height/2 - cam.height/2;
                updateShow(&cam);
                break;
            }
            clearBuffer();
            wclear(LOG);
            wprintw(LOG,"BUFFER : %s\n",val_buffer);
            wrefresh(LOG);
            updateProperties();
        }
        else if(getHideoutNet(c)) /* + ou - */
            updateTools();
        else /* si rien ne s'est passé */
        {
            wclear(LOG);
            wprintw(LOG,"BUFFER : %s\n",val_buffer);
            wrefresh(LOG);
        }
    }
}

/* =============== Main =============== */

static void clearProg()
{
    clearWindows();
    endNcurses();
    if(map != NULL)
        deleteMap(map);
    _exit(EXIT_SUCCESS);
}

int main(int argc,char* argv[])
{
    short error;

    /* récuperer les options*/
    getOptions(argc,argv);

    /* placer la fonction lors de l'arrêt du programme */
    if(atexit(clearProg) != 0)
    {
        perror("[main] Probleme lors de l'enregistrement ");
        exit(EXIT_FAILURE);
    }

    /* initialiser le programme */
    initSignal();
    initNcurses();
    initColor();
    initWindows();

    /* charge une map s'il y en a une */
    if(LOAD)
    {
        wclear(LOG);
        wprintw(LOG,"LOAD MAP\n");
        wrefresh(LOG);
        map = loadMap(FILE_LOAD,&error);
        if(error!=NO_ERROR)
        {
            wclear(LOG);
            wprintw(LOG,"MAP %s LOAD FAIL : ERROR %d\n",FILE_LOAD,error);
            wrefresh(LOG);
            map = NULL;
        }
    }
    /* sinon en crée une vide */
    if(map == NULL)
        map = createMap(MIN_HEIGHT,MIN_WIDTH);

    /* lance la boucle principal */
    update();

    return EXIT_SUCCESS;
}
