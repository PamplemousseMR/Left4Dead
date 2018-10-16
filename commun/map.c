#include "map.h"

/* =============== La structur map =============== */

map_t* createMap(unsigned short height, unsigned short width)
{
    short i, j;

    map_t* map = (map_t*)malloc(sizeof(map_t));
    if(map == NULL)
    {
        fprintf(stderr,"[createMap] erreur lors de l'allocation dynamique de la carte\n");
        exit(EXIT_FAILURE);
    }


    if(height<MIN_HEIGHT)height=MIN_HEIGHT;
    else if(height>MAX_HEIGHT)height=MAX_HEIGHT;
    if(width<MIN_WIDTH)width=MIN_WIDTH;
    else if(width>MAX_WIDTH)width=MAX_WIDTH;

    map->height = height;
    map->width = width;
    map->map = (unsigned short**)malloc(sizeof(unsigned short*)*height);
    if(map->map == NULL)
    {
        fprintf(stderr,"[createMap] erreur lors de l'allocation dynamique du tableau de la carte\n");
        exit(EXIT_FAILURE);
    }
    for(i=0 ; i<height ; ++i)
    {
        map->map[i] = (unsigned short*)malloc(sizeof(unsigned short)*width);
        if(map->map[i] == NULL)
        {
            fprintf(stderr,"[createMap] erreur lors de l'allocation dynamique de la ligne %d du tableau de la carte\n",i);
            exit(EXIT_FAILURE);
        }
        for(j=0 ; j<width ; j++)
            map->map[i][j] = EMPTY;
    }

    map->player_max = MIN_PLAYER;
    map->food_spawn = MIN_AMO_SPAWN;
    map->food_count = MIN_FOOD_COUNT;
    map->food_add = MIN_FOOD_ADD;
    map->amo_spawn = MIN_AMO_SPAWN;
    map->amo_count = MIN_AMO_COUNT;
    map->amo_add = MIN_AMO_ADD;
    map->zombie_spawn = MIN_ZOMBIE_SPAWN;
    map->zombie_max = MIN_ZOMBIE;

    return map;
}

void deleteMap(map_t* map)
{
    unsigned short i;
    for(i=0 ; i<map->height ; ++i)
        free(map->map[i]);
    free(map->map);
    free(map);
}

unsigned short setHeight(map_t* map, unsigned short val)
{
    unsigned short i,j;
    for(i=0 ; i<map->height ; ++i)
        free(map->map[i]);
    free(map->map);
    if(val<MIN_HEIGHT)
        map->height = MIN_HEIGHT;
    else if(val>MAX_HEIGHT)
        map->height = MAX_HEIGHT;
    else
        map->height = val;
    map->map = (unsigned short**)malloc(sizeof(unsigned short*)*map->height);
    if(map->map == NULL)
    {
        fprintf(stderr,"[setHeight] erreur lors de l'allocation dynamique du tableau de la carte\n");
        exit(EXIT_FAILURE);
    }
    for(i=0 ; i<map->height ; ++i)
    {
        map->map[i] = (unsigned short*)malloc(sizeof(unsigned short)*map->width);
        if(map->map[i] == NULL)
        {
            fprintf(stderr,"[setHeight] erreur lors de l'allocation dynamique de la ligne %d du tableau de la carte\n",i);
            exit(EXIT_FAILURE);
        }
        for(j=0 ; j<map->width ; j++)
            map->map[i][j] = EMPTY;
    }
    return map->player_max;
}

unsigned short setWidth(map_t* map, unsigned short val)
{
    unsigned short i,j;
    for(i=0 ; i<map->height ; ++i)
        free(map->map[i]);
    free(map->map);
    if(val<MIN_WIDTH)
        map->width = MIN_WIDTH;
    else if(val>MAX_WIDTH)
        map->width = MAX_WIDTH;
    else
        map->width = val;
    map->map = (unsigned short**)malloc(sizeof(unsigned short*)*map->height);
    if(map->map == NULL)
    {
        fprintf(stderr,"[setWidth] erreur lors de l'allocation dynamique du tableau de la carte\n");
        exit(EXIT_FAILURE);
    }
    for(i=0 ; i<map->height ; ++i)
    {
        map->map[i] = (unsigned short*)malloc(sizeof(unsigned short)*map->width);
        if(map->map[i] == NULL)
        {
            fprintf(stderr,"[setWidth] erreur lors de l'allocation dynamique de la ligne %d du tableau de la carte\n",i);
            exit(EXIT_FAILURE);
        }
        for(j=0 ; j<map->width ; j++)
            map->map[i][j] = EMPTY;
    }
    return map->player_max;
}

unsigned short setNumPlayer(map_t* map ,unsigned short val)
{
    if(val<MIN_PLAYER)
        map->player_max = MIN_PLAYER;
    else if(val>MAX_PLAYER)
        map->player_max = MAX_PLAYER;
    else
        map->player_max = val;
    return map->player_max;
}

unsigned short setFoodSpawn(map_t* map ,unsigned short val)
{
    if(val<MIN_FOOD_SPAWN)
        map->food_spawn = MIN_FOOD_SPAWN;
    else if(val>MAX_FOOD_SPAWN)
        map->food_spawn = MAX_FOOD_SPAWN;
    else
        map->food_spawn = val;
    return map->food_spawn;
}

unsigned short setFoodCount(map_t* map ,unsigned short val)
{
    if(val<MIN_FOOD_COUNT)
        map->food_count = MIN_FOOD_COUNT;
    else if(val>MAX_FOOD_COUNT)
        map->food_count = MAX_FOOD_COUNT;
    else
        map->food_count = val;
    return map->food_count;
}

unsigned short setFoodAdd(map_t* map ,unsigned short val)
{
    if(val<MIN_FOOD_ADD)
        map->food_add = MIN_FOOD_ADD;
    else if(val>MAX_FOOD_ADD)
        map->food_add = MAX_FOOD_ADD;
    else
        map->food_add = val;
    return map->food_add;
}

unsigned short setAmoSpawn(map_t* map ,unsigned short val)
{
    if(val<MIN_AMO_SPAWN)
        map->amo_spawn = MIN_AMO_SPAWN;
    else if(val>MAX_AMO_SPAWN)
        map->amo_spawn = MAX_AMO_SPAWN;
    else
        map->amo_spawn = val;
    return map->amo_spawn;
}

unsigned short setAmoCount(map_t* map ,unsigned short val)
{
    if(val<MIN_AMO_COUNT)
        map->amo_count = MIN_AMO_COUNT;
    else if(val>MAX_AMO_COUNT)
        map->amo_count = MAX_AMO_COUNT;
    else
        map->amo_count = val;
    return map->amo_count;
}

unsigned short setAmoAdd(map_t* map ,unsigned short val)
{
    if(val<MIN_AMO_ADD )
        map->amo_add = MIN_AMO_ADD ;
    else if(val>MAX_AMO_ADD )
        map->amo_add = MAX_AMO_ADD ;
    else
        map->amo_add = val;
    return map->amo_add;
}

unsigned short setZombieSpawn(map_t* map ,unsigned short val)
{
    if(val<MIN_ZOMBIE_SPAWN)
        map->zombie_spawn = MIN_ZOMBIE_SPAWN;
    else if(val>MAX_ZOMBIE_SPAWN)
        map->zombie_spawn = MAX_ZOMBIE_SPAWN;
    else
        map->zombie_spawn = val;
    return map->zombie_spawn;
}

unsigned short setZombieMax(map_t* map ,unsigned short val)
{
    if(val<MIN_ZOMBIE)
        map->zombie_max = MIN_ZOMBIE;
    else if(val>MAX_ZOMBIE)
        map->zombie_max = MAX_ZOMBIE;
    else
        map->zombie_max = val;
    return map->zombie_max;
}

/* =============== Dessiné la map =============== */

static void drawObstacle(WINDOW* win,map_t* map,short line, short col)
{
    char draw = 0;
    /* ========== quadruple ========== */
    if(line-1>=0 && line+1<map->height && col-1>=0 && col+1<map->width)
    {
        if(map->map[line-1][col]==OBSTACLE && map->map[line+1][col]==OBSTACLE
                && map->map[line][col+1]==OBSTACLE && map->map[line][col-1]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_PLUS);
        }
    }
    /* ========== triple ========== */
    /* gauche haut droite */
    if(!draw && line-1>=0 && col+1<map->width && col-1>=0)
    {
        if(map->map[line-1][col]==OBSTACLE && map->map[line][col+1]==OBSTACLE && map->map[line][col-1]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_BTEE);
        }
    }
    /* gauche bas droite */
    if(!draw && line+1<map->height && col+1<map->width && col-1>=0)
    {
        if(map->map[line+1][col]==OBSTACLE && map->map[line][col+1]==OBSTACLE && map->map[line][col-1]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_TTEE);
        }
    }
    /* haut droite bas */
    if(!draw && line-1>=0 && line+1<map->height && col+1<map->width)
    {
        if(map->map[line-1][col]==OBSTACLE && map->map[line][col+1]==OBSTACLE && map->map[line+1][col]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_LTEE);
        }
    }
    /* haut gauche bas */
    if(!draw && line-1>=0 && line+1<map->height && col-1>=0)
    {
        if(map->map[line-1][col]==OBSTACLE && map->map[line][col-1]==OBSTACLE && map->map[line+1][col]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_RTEE);
        }
    }
    /* ========== double ========== */
    /* haut droite */
    if(!draw && line-1>=0 && col+1<map->width)
    {
        if(map->map[line-1][col]==OBSTACLE && map->map[line][col+1]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_LLCORNER);
        }
    }
    /* droite bas */
    if(!draw && line+1<map->height && col+1<map->width)
    {
        if(map->map[line+1][col]==OBSTACLE && map->map[line][col+1]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_ULCORNER);
        }
    }
    /* bas gauche */
    if(!draw && line+1<map->height && col-1>=0)
    {
        if(map->map[line+1][col]==OBSTACLE && map->map[line][col-1]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_URCORNER);
        }
    }
    /* gauche haut */
    if(!draw && line-1>=0 && col-1>=0)
    {
        if(map->map[line-1][col]==OBSTACLE && map->map[line][col-1]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_LRCORNER);
        }
    }
    /* mono/double line */
    /* bas */
    if(!draw && line+1<map->height)
    {
        if(map->map[line+1][col]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_VLINE);
        }
    }
    /* haut */
    if(!draw && line-1>=0)
    {
        if(map->map[line-1][col]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_VLINE);
        }
    }
    /* gauche */
    if(!draw && col-1>=0)
    {
        if(map->map[line][col-1]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_HLINE);
        }
    }
    /* droite */
    if(!draw && col+1<map->width)
    {
        if(map->map[line][col+1]==OBSTACLE)
        {
            draw = 1;
            waddch(win,ACS_HLINE);
        }
    }
    if(!draw)
        waddch(win,ACS_PLUS);
}

void drawMap(WINDOW* win, map_t* map, short line, short col, unsigned short height, unsigned short width)
{
    short i,j;

    if(line>=map->height || col>=map->width)
        return;

    for(i=0 ; i<height ; ++i)
    {
        for(j=0 ; j<width ; ++j)
        {
            wmove(win,i,j);
            if(i+line>=0 && i+line<map->height && j+col>=0 && j+col<map->width)
            {
                switch(map->map[i+line][j+col])
                {
                case EMPTY:
                    wattron(win,COLOR_PAIR(DRAW_EMPTY_COLOR));
                    waddch(win,DRAW_EMPTY);
                    wattroff(win,COLOR_PAIR(DRAW_EMPTY_COLOR));
                    break;
                case CEMETERY:
                    wattron(win,COLOR_PAIR(DRAW_CEMETERY_COLOR));
                    waddch(win,DRAW_CEMETERY);
                    wattroff(win,COLOR_PAIR(DRAW_CEMETERY_COLOR));
                    break;
                case OBSTACLE:
                    wattron(win,COLOR_PAIR(DRAW_OBSTACLE_COLOR));
                    drawObstacle(win,map,i+line,j+col);
                    wattroff(win,COLOR_PAIR(DRAW_OBSTACLE_COLOR));
                    break;
                case FOOD:
                    wattron(win,COLOR_PAIR(DRAW_FOOD_COLOR));
                    waddch(win,DRAW_FOOD);
                    wattroff(win,COLOR_PAIR(DRAW_FOOD_COLOR));
                    break;
                case AMMO:
                    wattron(win,COLOR_PAIR(DRAW_AMMO_COLOR));
                    waddch(win,DRAW_AMMO);
                    wattroff(win,COLOR_PAIR(DRAW_AMMO_COLOR));
                    break;
                case ZOMBIE:
                    wattron(win,COLOR_PAIR(DRAW_ZOMBIE_COLOR));
                    waddch(win,DRAW_ZOMBIE);
                    wattroff(win,COLOR_PAIR(DRAW_ZOMBIE_COLOR));
                    break;
                }
                if(map->map[i+line][j+col]>=HIDEOUT(0) && map->map[i+line][j+col]<HIDEOUT(MAX_HIDEOUT))
                {
                    wattron(win,COLOR_PAIR(DRAW_HIDEOUT_COLOR(map->map[i+line][j+col])));
                    waddch(win,DRAW_HIDEOUT);
                    wattroff(win,COLOR_PAIR(DRAW_HIDEOUT_COLOR(map->map[i+line][j+col])));
                }
                else if(map->map[i+line][j+col]>=HOME(0) && map->map[i+line][j+col]<HOME(MAX_PLAYER))
                {
                    wattron(win,COLOR_PAIR(DRAW_HOME_COLOR(map->map[i+line][j+col])));
                    waddch(win,DRAW_HOME);
                    wattroff(win,COLOR_PAIR(DRAW_HOME_COLOR(map->map[i+line][j+col])));
                }
                else if(map->map[i+line][j+col]>=HUMAN(0) && map->map[i+line][j+col]<HUMAN(MAX_PLAYER))
                {
                    wattron(win,COLOR_PAIR(DRAW_HUMAN_COLOR(map->map[i+line][j+col])));
                    waddch(win,DRAW_HUMAN);
                    wattroff(win,COLOR_PAIR(DRAW_HUMAN_COLOR(map->map[i+line][j+col])));
                }
            }
            else if(i+line==-1 && col+j>=0 && col+j<map->width)
                waddch(win,ACS_S7);
            else if(i+line==map->height && col+j>=0 && col+j<map->width)
                waddch(win,ACS_S7);
            else if(j+col==map->width && i+line>=0 && i+line<map->height)
                waddch(win,ACS_VLINE);
            else if(j+col==-1 && i+line>=0 && i+line<map->height)
                waddch(win,ACS_VLINE);
            else if(j+col==-1 && i+line==-1)
                waddch(win,ACS_ULCORNER);
            else if(j+col==-1 && i+line==map->height)
                waddch(win,ACS_LLCORNER);
            else if(j+col==map->width && i+line==map->height)
                waddch(win,ACS_LRCORNER);
            else if(j+col==map->width && i+line==-1)
                waddch(win,ACS_URCORNER);
            else
                waddch(win,DRAW_EMPTY);
        }
    }
}

void initColor()
{
    unsigned int i;

    if(has_colors() == FALSE)
    {
        printf("[initCurse] le terminal ne supporte pas les couleurs\n");
        exit(EXIT_FAILURE);
    }
    start_color();

    init_color(17,127*1000/255,127*1000/255,127*1000/255);
    init_color(18,45*1000/255,75*1000/255,28*1000/255);
    init_color(19,139*1000/255,69*1000/255,19*1000/255);

    init_color(33,255*1000/255, 0*1000/255, 0*1000/255);
    init_color(48,255*1000/255, 49*1000/255, 0*1000/255);
    init_color(23,255*1000/255, 98*1000/255, 0*1000/255);
    init_color(49,255*1000/255, 147*1000/255, 0*1000/255);
    init_color(27,255*1000/255, 196*1000/255, 0*1000/255);
    init_color(38,255*1000/255, 245*1000/255, 0*1000/255);
    init_color(50,216*1000/255, 255*1000/255, 0*1000/255);
    init_color(32,167*1000/255, 255*1000/255, 0*1000/255);
    init_color(20,118*1000/255, 255*1000/255, 0*1000/255);
    init_color(47,69*1000/255, 255*1000/255, 0*1000/255);
    init_color(46,20*1000/255, 255*1000/255, 0*1000/255);
    init_color(34,0*1000/255, 255*1000/255, 29*1000/255);
    init_color(37,0*1000/255, 255*1000/255, 78*1000/255);
    init_color(45,0*1000/255, 255*1000/255, 127*1000/255);
    init_color(39,0*1000/255, 255*1000/255, 176*1000/255);
    init_color(28,0*1000/255, 255*1000/255, 225*1000/255);
    init_color(31,0*1000/255, 236*1000/255, 255*1000/255);
    init_color(21,0*1000/255, 187*1000/255, 255*1000/255);
    init_color(43,0*1000/255, 138*1000/255, 255*1000/255);
    init_color(26,0*1000/255, 89*1000/255, 255*1000/255);
    init_color(29,0*1000/255, 40*1000/255, 255*1000/255);
    init_color(35,9*1000/255, 0*1000/255, 255*1000/255);
    init_color(44,58*1000/255, 0*1000/255, 255*1000/255);
    init_color(24,107*1000/255, 0*1000/255, 255*1000/255);
    init_color(22,156*1000/255, 0*1000/255, 255*1000/255);
    init_color(36,205*1000/255, 0*1000/255, 255*1000/255);
    init_color(40,254*1000/255, 0*1000/255, 255*1000/255);
    init_color(41,255*1000/255, 0*1000/255, 207*1000/255);
    init_color(30,255*1000/255, 0*1000/255, 158*1000/255);
    init_color(42,255*1000/255, 0*1000/255, 109*1000/255);
    init_color(25,255*1000/255, 0*1000/255, 60*1000/255);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(DRAW_ZOMBIE_COLOR, COLOR_RED, 18);
    init_pair(DRAW_FOOD_COLOR, COLOR_GREEN, 18);
    init_pair(DRAW_AMMO_COLOR, COLOR_YELLOW, 18);
    init_pair(DRAW_CEMETERY_COLOR, 17, 19);
    init_pair(DRAW_OBSTACLE_COLOR, COLOR_BLACK, 17);
    init_pair(DRAW_EMPTY_COLOR, 18, 18);
    for(i=0 ; i<MAX_PLAYER ; ++i)
    {
        init_pair(DRAW_HOME_COLOR(HOME(i)), 20+i, 17);
        init_pair(DRAW_HUMAN_COLOR(HUMAN(i)), 20+i, 18);
    }

    for(i=0 ; i<MAX_HIDEOUT ; ++i)
    {
        init_pair(DRAW_HIDEOUT_COLOR(HIDEOUT(i)), 20+i, 17);
    }
}

/* =============== Sauvegarder la map dans un fichier =============== */

#define READ_WRITE_FUNCTION(name,func) \
    static short name(int fd,map_t* map) \
{ \
    unsigned short i,j; \
    for(i=0 ; i<map->height ; ++i) \
{ \
    for(j=0 ; j<map->width ; ++j) \
{ \
    if(func(fd,&map->map[i][j],sizeof(unsigned char)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur avec la carte "); \
    return ERROR_TILE; \
    } \
    } \
    } \
    if(func(fd,&map->player_max,sizeof(unsigned char)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur lors de la lecture du nombre de joueurs "); \
    return ERROR_NAX_PLAYER; \
    } \
    if(func(fd,&map->food_spawn,sizeof(unsigned short)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur lors de la lecture du spawn de nourriture "); \
    return ERROR_FOOD_SPAWN; \
    } \
    if(func(fd,&map->food_count,sizeof(unsigned char)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur lors de la lecture du nombre de sacs de nourriture "); \
    return ERROR_FOOD_COUNT; \
    } \
    if(func(fd,&map->food_add,sizeof(unsigned short)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur lors de la lecture du nombre de nourriture "); \
    return ERROR_FOOD_ADD; \
    } \
    if(func(fd,&map->amo_spawn,sizeof(unsigned short)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur lors de la lecture du spawn de munitions "); \
    return ERROR_AMO_SPAWN; \
    } \
    if(func(fd,&map->amo_count,sizeof(unsigned char)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur lors de la lecture du nombre de sacs de munitions "); \
    return ERROR_AMO_COUNT; \
    } \
    if( func(fd,&map->amo_add,sizeof(unsigned short)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur lors de la lecture du nombre de munitions "); \
    return ERROR_AMO_ADD; \
    } \
    if(func(fd,&map->zombie_spawn,sizeof(unsigned short)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur avec le spawn de zombies "); \
    return ERROR_ZOMBIE_SPAWN; \
    } \
    if(func(fd,&map->zombie_max,sizeof(unsigned char)) == -1) \
{ \
    perror("[READ_WRITE_FUNCTION] Erreur avec le nombre de zombies "); \
    return ERROR_NAX_PLAYER; \
    } \
    return NO_ERROR; \
    }


READ_WRITE_FUNCTION(readMap,read)
READ_WRITE_FUNCTION(writeMap,write)

#undef READ_WRITE_FUNCTION

short saveMap(map_t* map,char* name)
{
    int fd;
    short error = NO_ERROR;

    if((fd = open(name, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1)
    {
        perror("[save] Erreur lors de l'ouverture du fichier");
        return ERROR_OPEN;
    }

    if(write(fd,&map->width,sizeof(unsigned short))==-1)
    {
        perror("[save] Erreur lors de l'enregistrement de la largeur");
        return ERROR_WIDTH;
    }
    if(write(fd,&map->height,sizeof(unsigned short))==-1)
    {
        perror("[save] Erreur lors de l'enregistrement de la hauteur");
        return ERROR_HEIGHT;
    }

    error=writeMap(fd,map);

    close(fd);

    return error;
}

/* =============== Vérifier la carte =============== */

map_t* loadMap(char* name, short* err)
{
    int fd;
    unsigned short height, width;
    map_t* map =NULL;
    *err = NO_ERROR;

    if((fd = open(name, O_RDONLY, S_IRUSR|S_IWUSR)) == -1)
    {
        perror("[save] Erreur lors de l'ouverture du fichier");
        *err = ERROR_OPEN;
        return NULL;
    }

    else if(read(fd,&width,sizeof(unsigned short)) == -1)
    {
        perror("[load] Erreur lors de la lecture de la largeur");
        *err = ERROR_WIDTH;
        return NULL;
    }
    if(read(fd,&height,sizeof(unsigned short)) == -1)
    {
        perror("[load] Erreur lors de la lecture la hauteur");
        *err = ERROR_HEIGHT;
        return NULL;
    }

    map = createMap(height,width);
    *err=readMap(fd,map);
    close(fd);

    if(*err!=NO_ERROR)
    {
        deleteMap(map);
        map = NULL;
    }
    return map;
}

short checkMap(map_t* map)
{
    unsigned short i,j,k;
    char* home_count;

    if(map->height<MIN_HEIGHT || map->height>MAX_HEIGHT)
        return ERROR_HEIGHT;
    if(map->width<MIN_WIDTH || map->width>MAX_WIDTH)
        return ERROR_WIDTH;
    if(map->player_max<MIN_PLAYER || map->player_max>MAX_PLAYER)
        return ERROR_NAX_PLAYER;
    if(map->food_spawn<MIN_FOOD_SPAWN || map->food_spawn>MAX_FOOD_SPAWN)
        return ERROR_FOOD_SPAWN;
    if(map->food_count<MIN_FOOD_COUNT || map->food_count>MAX_FOOD_COUNT)
        return ERROR_FOOD_COUNT;
    if(map->food_add<MIN_FOOD_ADD || map->food_add>MAX_FOOD_ADD)
        return ERROR_FOOD_ADD;
    if(map->amo_spawn<MIN_AMO_SPAWN || map->amo_spawn>MAX_AMO_SPAWN)
        return ERROR_AMO_SPAWN;
    if(map->amo_count<MIN_AMO_COUNT || map->amo_count>MAX_AMO_COUNT)
        return ERROR_AMO_COUNT;
    if(map->amo_add<MIN_AMO_ADD || map->amo_add>MAX_AMO_ADD)
        return ERROR_AMO_ADD;
    if(map->zombie_max<MIN_ZOMBIE || map->zombie_max>MAX_ZOMBIE)
        return ERROR_NUM_ZOMBIE;
    if(map->zombie_spawn<MIN_ZOMBIE_SPAWN || map->zombie_spawn>MAX_ZOMBIE_SPAWN)
        return ERROR_ZOMBIE_SPAWN;

    home_count = (char*)malloc(sizeof(char)*map->player_max);
    if(home_count == NULL)
    {
        fprintf(stderr,"[checkMap] Erreur lors de l'allocation dynamique\n");
        exit(EXIT_FAILURE);
    }
    for(k=0 ; k<map->player_max ; ++k)
        home_count[k] = 0;

    for(i=0 ; i<map->height ; ++i)
    {
        for(j=0 ; j<map->width ; ++j)
        {
            if(map->map[i][j]>=HOME(0) && map->map[i][j]<HOME(MAX_PLAYER))
            {
                for(k=0 ; k<map->player_max ; ++k)
                {
                    if(map->map[i][j]==HOME(k))
                    {
                        if (k<map->player_max)
                            home_count[k] = 1;
                        else
                        {
                            free(home_count);
                            return ERROR_HOME;
                        }
                    }
                }
            }
        }
    }

    for(k=0 ; k<map->player_max ; ++k)
    {
        if(home_count[k]==0)
        {
            free(home_count);
            return ERROR_HOME;
        }
    }

    free(home_count);

    return NO_ERROR;
}
