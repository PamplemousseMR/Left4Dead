#include "ncurses.h"

WINDOW* SHOW = NULL;
WINDOW* PLAYER = NULL;

static WINDOW* SHOW_BOX = NULL;
static WINDOW* PLAYER_BOX = NULL;

static WINDOW* EAT_BOX = NULL;
static WINDOW* GET_BOX = NULL;
static WINDOW* SET_BOX = NULL;

void initNcurses()
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    refresh();
    noecho();
    mousemask(ALL_MOUSE_EVENTS, NULL);
    if(has_mouse() != TRUE)
    {
        fprintf(stderr, "[initNcurses] Aucune souris n'est détectée\n");
        exit(EXIT_FAILURE);
    }
}

void endNcurses()
{
    endwin();
}

void initWindows()
{
    SHOW = newwin(SHOW_LINE, SHOW_COL, 1, 1);
    wrefresh(SHOW);
    SHOW_BOX = newwin(SHOW_LINE_BOX, SHOW_COL_BOX, 0, 0);
    box(SHOW_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(SHOW_BOX);

    PLAYER = newwin(PLAYER_LINE, PLAYER_COL, 1, SHOW_COL_BOX+1);
    wrefresh(PLAYER);
    PLAYER_BOX = newwin(PLAYER_LINE_BOX, PLAYER_COL_BOX, 0, SHOW_COL_BOX);
    box(PLAYER_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(PLAYER_BOX);

    if(getType()==TYPE_ZOMBIE)
    {
        EAT_BOX = newwin(BUTTON_LINE_BOX, BUTTON_COL_BOX, PLAYER_LINE_BOX, SHOW_COL_BOX);
        box(EAT_BOX, ACS_VLINE, ACS_HLINE);
        mvwprintw(EAT_BOX,BUTTON_LINE_BOX/2+1, BUTTON_COL_BOX/2-3,"MANGER");
        wrefresh(EAT_BOX);
    }
    else
    {
        EAT_BOX = newwin(BUTTON_LINE_BOX, BUTTON_COL_BOX/3, PLAYER_LINE_BOX, SHOW_COL_BOX);
        box(EAT_BOX, ACS_VLINE, ACS_HLINE);
        mvwprintw(EAT_BOX,BUTTON_LINE_BOX/2+1, (BUTTON_COL_BOX/3)/2-3,"MANGER");
        wrefresh(EAT_BOX);

        GET_BOX = newwin(BUTTON_LINE_BOX, BUTTON_COL_BOX/3, PLAYER_LINE_BOX, SHOW_COL_BOX+(BUTTON_COL_BOX/3));
        box(GET_BOX, ACS_VLINE, ACS_HLINE);
        mvwprintw(GET_BOX,BUTTON_LINE_BOX/2+1, (BUTTON_COL_BOX/3)/2-4,"PRENDRE");
        wrefresh(GET_BOX);

        SET_BOX = newwin(BUTTON_LINE_BOX, BUTTON_COL_BOX/3, PLAYER_LINE_BOX, SHOW_COL_BOX+(BUTTON_COL_BOX/3*2));
        box(SET_BOX, ACS_VLINE, ACS_HLINE);
        mvwprintw(SET_BOX,BUTTON_LINE_BOX/2+1, (BUTTON_COL_BOX/3)/2-4,"LAISSER");
        wrefresh(SET_BOX);
    }
}

void clearWindows()
{
    delwin(SHOW);
    delwin(SHOW_BOX);
    delwin(PLAYER);
    delwin(PLAYER_BOX);
    delwin(EAT_BOX);
    if(getType()==TYPE_HUMAN)
    {
        delwin(GET_BOX);
        delwin(SET_BOX);
    }
}

void drawZombies(zombie_t* zombies, unsigned short size)
{
    unsigned short i;
    if(zombies!=NULL)
    {
        for(i=0 ; i<size ; ++i)
        {
            if(getSelectedId()==i)
                wattron(PLAYER,COLOR_PAIR(1));
            if(zombies[i].life!=0)
                wprintw(PLAYER,"Z %d/%d(%d,%d)\n",zombies[i].life,MAX_ZOMBIE_LIFE,zombies[i].line,zombies[i].col);
            else
                wprintw(PLAYER,"Z %d/%d(_,_)\n",zombies[i].life,MAX_ZOMBIE_LIFE);
            if(getSelectedId()==i)
                wattroff(PLAYER,COLOR_PAIR(1));
        }
    }
}

void drawHumans(human_t* humans,unsigned short size,unsigned short id)
{
    unsigned short i;
    if(humans!=NULL)
    {
        for(i=0 ; i<size ; ++i)
        {
            if(i/HUMAN_PER_PLAYER==id)
            {
                if(getSelectedId()==i-(id*HUMAN_PER_PLAYER))
                    wattron(PLAYER,COLOR_PAIR(1));
                if(humans[i].life!=0)
                    wprintw(PLAYER,"H %d/%d(%d,%d) (A:%d,F%d)\n",humans[i].life,MAX_HUMAN_LIFE,humans[i].line,humans[i].col,humans[i].ammos,humans[i].foods);
                else
                    wprintw(PLAYER,"H %d/%d(_,_)\n",humans[i].life,MAX_HUMAN_LIFE);
                if(getSelectedId()==i-(id*HUMAN_PER_PLAYER))
                    wattroff(PLAYER,COLOR_PAIR(1));
            }
        }
    }
}

short getClickId(MEVENT* event)
{
    if(event->x>SHOW_COL_BOX && event->y<PLAYER_LINE_BOX)
        return event->y-1;
    return -1;
}

char getClickEat(MEVENT* event)
{
    if(getType()==TYPE_ZOMBIE)
    {
        if(event->x>SHOW_COL_BOX && event->y>=PLAYER_LINE_BOX)
            return 1;
    }
    else
    {
        if(event->x>SHOW_COL_BOX && event->x<SHOW_COL_BOX+(BUTTON_COL_BOX/3) && event->y>=PLAYER_LINE_BOX)
            return 1;
    }
    return 0;
}

char getClickGet(MEVENT* event)
{
    if(getType()==TYPE_ZOMBIE)
        return 0;
    if(event->x>=SHOW_COL_BOX+(BUTTON_COL_BOX/3) && event->x<SHOW_COL_BOX+(BUTTON_COL_BOX/3*2) && event->y>=PLAYER_LINE_BOX)
        return 1;
    return 0;
}

char getClickSet(MEVENT* event)
{
    if(getType()==TYPE_ZOMBIE)
        return 0;
    if(event->x>=SHOW_COL_BOX+(BUTTON_COL_BOX/3*2) && event->y>=PLAYER_LINE_BOX)
        return 1;
    return 0;
}
