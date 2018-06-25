#include "getkey.h"

static short id = -1;

void initGetkey()
{
    char c;
    MEVENT event;
    int result;
    node_t node;
    unsigned short max = getType() == TYPE_ZOMBIE ? getZombieMax() : HUMAN_PER_PLAYER;

    while(1)
    {
        c = getch();

        result = getmouse(&event);

        if(result == OK)
        {
            if(event.bstate & BUTTON1_CLICKED)
            {
                if(getClickEat(&event))
                {
                    entityEat(id);
                    updatePlayer();
                }
                else if(getClickGet(&event))
                {
                    entityGet(id);
                }
                else if(getClickSet(&event))
                {
                    entitySet(id);
                }
                else
                {
                    id = getId(event.y+getCameraLine()-1,event.x+getCameraCol()-1);
                    if(getClickId(&event)!=-1)
                    {
                        id=getClickId(&event);
                        if(id>max)
                            id = max-1;
                    }
                    updatePlayer();
                }
            }
            else if((event.bstate & BUTTON3_CLICKED) || (event.bstate & BUTTON2_CLICKED))
            {
                node.col = event.x+getCameraCol()-1;
                node.line = event.y+getCameraLine()-1;
                entityMove(id,node);
            }
        }
        else
        {
            switch(c)
            {
            case 'z' :
                if(getCameraLine() < getMapHeight()-getCameraHeight()/2) setCameraLine(getCameraLine()+1);
                break;
            case 's' :
                if(getCameraLine() > -getCameraHeight()/2) setCameraLine(getCameraLine()-1);
                break;
            case 'd' :
                if(getCameraCol() > -getCameraWidth()/2) setCameraCol(getCameraCol()-1);
                break;
            case 'q' :
                if(getCameraCol() < getMapWidth()-getCameraWidth()/2) setCameraCol(getCameraCol()+1);
                break;
            }
        }
    }
}

short getSelectedId()
{
    return id;
}
