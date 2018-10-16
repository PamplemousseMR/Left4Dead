#include "ncurses.h"

WINDOW* SHOW = NULL;
WINDOW* TOOLS = NULL;
WINDOW* PROPERTIES = NULL;
WINDOW* LOG = NULL;

short TOOL = 0;
short PROPERTIE = 0;
short BUTTON = -1;

static WINDOW* SHOW_BOX = NULL;             /* boîte de la fenêtre principale */
static WINDOW* TOOLS_BOX = NULL;            /* boîte de la fenêtre d'outils */
static WINDOW* PROPERTIES_BOX = NULL;       /* boîte de la fenêtre de proprietes */
static WINDOW* LOG_BOX = NULL;              /* boîte de la fenêtre des logs */
static WINDOW* SAVE_BOX = NULL;             /* boîte du bouton 'save' */
static WINDOW* QUIT_BOX = NULL;             /* boîte du bouton 'quit' */

static const char* text[5] = {"ERASE ","CEMETERY ","OBSTACLE ", "HOME","HIDEOUT"};

void initNcurses()
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    refresh();
    noecho();
    mousemask(BUTTON1_CLICKED, NULL);
    #ifndef __APPLE__
    if(has_mouse() != TRUE)
    {
        fprintf(stderr, "[initNcurses] Aucune souris n'est détectée\n");
        exit(EXIT_FAILURE);
    }
    #endif
}

void endNcurses()
{
    endwin();
}

void initWindows()
{
    TOOLS = newwin(TOOLS_LINE, TOOLS_COL, 1, 1);
    wrefresh(TOOLS);
    TOOLS_BOX = newwin(TOOLS_LINE_BOX, TOOLS_COL_BOX, 0, 0);
    box(TOOLS_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(TOOLS_BOX);

    SHOW = newwin(SHOW_LINE, SHOW_COL, TOOLS_LINE_BOX+1, 1);
    wrefresh(SHOW);
    SHOW_BOX = newwin(SHOW_LINE_BOX, SHOW_COL_BOX, TOOLS_LINE_BOX, 0);
    box(SHOW_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(SHOW_BOX);

    PROPERTIES = newwin(PROPERTIES_LINE, PROPERTIES_COL, 1, TOOLS_COL_BOX+1);
    wrefresh(PROPERTIES);
    PROPERTIES_BOX = newwin(PROPERTIES_LINE_BOX, PROPERTIES_COL_BOX, 0, TOOLS_COL_BOX);
    box(PROPERTIES_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(PROPERTIES_BOX);

    LOG = newwin(LOG_LINE, LOG_COL, PROPERTIES_LINE_BOX+2*BUTTON_LINE_BOX+1,TOOLS_COL_BOX+1);
    wrefresh(LOG);
    LOG_BOX = newwin(LOG_LINE_BOX, LOG_COL_BOX, PROPERTIES_LINE_BOX+2*BUTTON_LINE_BOX, TOOLS_COL_BOX);
    box(LOG_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(LOG_BOX);

    SAVE_BOX = newwin(BUTTON_LINE_BOX, BUTTON_COL_BOX, PROPERTIES_LINE_BOX, TOOLS_COL_BOX);
    box(SAVE_BOX, ACS_VLINE, ACS_HLINE);
    mvwprintw(SAVE_BOX,BUTTON_LINE/2, BUTTON_COL/2-2,"SAVE");
    wrefresh(SAVE_BOX);

    QUIT_BOX = newwin(BUTTON_LINE_BOX, BUTTON_COL_BOX, PROPERTIES_LINE_BOX+BUTTON_LINE_BOX, TOOLS_COL_BOX);
    box(QUIT_BOX, ACS_VLINE, ACS_HLINE);
    mvwprintw(QUIT_BOX,BUTTON_LINE/2, BUTTON_COL/2-2,"QUIT");
    wrefresh(QUIT_BOX);
}

void clearWindows()
{
    delwin(TOOLS);
    delwin(TOOLS_BOX);
    delwin(SHOW);
    delwin(SHOW_BOX);
    delwin(PROPERTIES);
    delwin(PROPERTIES_BOX);
    delwin(LOG);
    delwin(LOG_BOX);
    delwin(SAVE_BOX);
    delwin(QUIT_BOX);
}

void drawTools(short network,short home)
{
    short i;
    for(i=0 ; i<5 ; ++i)
    {
        if(i==TOOL)
            wattron(TOOLS,COLOR_PAIR(1));
        wprintw(TOOLS, text[i]);
        if(i==4)
            wprintw(TOOLS, "(%i)", network);
        if(i==3)
            wprintw(TOOLS, "(%i) ", home);
        if(i==TOOL)
            wattroff(TOOLS,COLOR_PAIR(1));
    }
}

void drawProperties(unsigned short  num_player,unsigned short  food_spawn,unsigned short  food_count,unsigned short  food_add,unsigned short  amo_spawn,unsigned short  amo_count,unsigned short  amo_add,unsigned short  zombie_spawn,unsigned short  zombie_max, unsigned short height, unsigned short width)
{
    int i;
    mvwprintw(PROPERTIES,0,PROPERTIES_COL/2-5,"Properties\n");
    waddch(PROPERTIES,'\n');
    if(PROPERTIE==0) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,2,0,"  Max player : %d/%d\n",num_player,MAX_PLAYER);
    if(PROPERTIE==0) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==1) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,3,0,"  Food spawn : %d/%d\n",food_spawn,MAX_FOOD_SPAWN);
    if(PROPERTIE==1) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==2) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,4,0,"  Food count : %d/%d\n",food_count,MAX_FOOD_COUNT);
    if(PROPERTIE==2) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==3) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,5,0,"    Food add : %d/%d\n",food_add,MAX_FOOD_ADD);
    if(PROPERTIE==3) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==4) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,6,0,"  Ammo spawn : %d/%d\n",amo_spawn,MAX_AMO_SPAWN);
    if(PROPERTIE==4) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==5) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,7,0,"  Ammo count : %d/%d\n",amo_count,MAX_AMO_COUNT);
    if(PROPERTIE==5) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==6) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,8,0,"    Ammo add : %d/%d\n",amo_add,MAX_AMO_ADD);
    if(PROPERTIE==6) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==7) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,9,0,"Zombie spawn : %d/%d\n",zombie_spawn,MAX_ZOMBIE_SPAWN);
    if(PROPERTIE==7) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==8) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,10,0,"  Zombie max : %d/%d\n",zombie_max,MAX_ZOMBIE);
    if(PROPERTIE==8) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==9) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,11,0,"  Map height : %d/%d\n",height,MAX_HEIGHT);
    if(PROPERTIE==9) wattroff(PROPERTIES,COLOR_PAIR(1));
    if(PROPERTIE==10) wattron(PROPERTIES,COLOR_PAIR(1));
    mvwprintw(PROPERTIES,12,0,"   Map width : %d/%d\n",width,MAX_WIDTH);
    if(PROPERTIE==10) wattroff(PROPERTIES,COLOR_PAIR(1));
    waddch(PROPERTIES,'\n');
    for(i=0 ; i<PROPERTIES_COL ; ++i)waddch(PROPERTIES,'=');
    wprintw(PROPERTIES,"\n\n");
    wprintw(PROPERTIES, "TOOL : click on the specified tool.\n"\
                        "PROPERTY : click on the specified property,\n"\
                        "           type the new value,\n"\
                        "           press enter.\n"\
                        "HIDEOUT : press + or - to change network.\n"\
                        "MOVE : use ZQSD to move the map.");

}

char getSelectedProperty(int c, MEVENT* event)
{
    if(event->x>=TOOLS_COL_BOX && event->y<PROPERTIES_LINE_BOX)
    {
        PROPERTIE = event->y-3;
        PROPERTIE = PROPERTIE>=11? 10 : PROPERTIE;
        PROPERTIE = PROPERTIE<0? 0 : PROPERTIE;
        return 1;
    }
    return 0;
}

char getSelectedTool(int c,MEVENT* event)
{
    short i;
    int cumul = 0;
    if(event->y<TOOLS_LINE_BOX && event->x<TOOLS_COL_BOX)
    {
        for(i=0 ; i<5 ; ++i)
        {
            cumul+=strlen(text[i]);
            if(i==3)cumul+=4;
            if(event->x < cumul)
            {
                TOOL = i;
                return 1;
            }
        }
    }
    return 0;
}

char getSelectedTile(int c,unsigned short* line, unsigned short* col,MEVENT* event)
{
    if(event->y<SHOW_LINE_BOX+TOOLS_LINE_BOX && event->y>TOOLS_LINE_BOX && event->x<SHOW_COL_BOX)
    {
        *col = event->x;
        *line = event->y-TOOLS_LINE_BOX;
        return 1;
    }
    return 0;
}

char getSelectedButton(int c, MEVENT* event)
{
    if(event->y>=PROPERTIES_LINE_BOX && event->y<PROPERTIES_LINE_BOX+BUTTON_LINE_BOX && event->x>SHOW_COL_BOX)
    {
        BUTTON = SAVE;
        return 1;
    }
    else if(event->y>=PROPERTIES_LINE_BOX+BUTTON_LINE_BOX && event->y<PROPERTIES_LINE_BOX+2*BUTTON_LINE_BOX && event->x>SHOW_COL_BOX)
    {
        BUTTON = QUIT;
        return 1;
    }
    return 0;
}
