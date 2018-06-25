#include "astar.h"

static node_t* getBestNode(list_t* openList)
{
    unsigned int i;
    nodes_t* current;
    node_t* ret = NULL;
    unsigned int f = -1;

    current = openList->nodes;
    for(i=0 ; i<openList->count ; ++i)
    {
        if(current->node->cost_f < f)
        {
            f = current->node->cost_f;
            ret = current->node;
        }
        current = current->next;
    }

    removeList(openList,ret);

    return ret;
}

static char compareNode(node_t* first, node_t* second)
{
    return (first->col==second->col && first->line==second->line);
}

static void addSuccessor(list_t* list,node_t* node)
{
    node_t* neighbour;
    unsigned short line,col;

    if(
            (getType()==TYPE_ZOMBIE && node->line-1>=0 && (getTile(node->line-1,node->col)==EMPTY || getTile(node->line-1,node->col)==FOOD || getTile(node->line-1,node->col)==AMMO))
            || (getType()==TYPE_HUMAN && node->line-1>=0 && ((getTile(node->line-1,node->col)>=HIDEOUT(0) && getTile(node->line-1,node->col)<HIDEOUT(MAX_HIDEOUT)) || getTile(node->line-1,node->col)==HOME(getGlobalId()) || getTile(node->line-1,node->col)==EMPTY || getTile(node->line-1,node->col)==FOOD || getTile(node->line-1,node->col)==AMMO))
            )
    {
        neighbour = createNode();
        neighbour->col = node->col;
        neighbour->line = node->line-1;
        addList(list,neighbour);
    }
    if(
            (getType()==TYPE_ZOMBIE && node->line+1<getMapHeight() && (getTile(node->line+1,node->col)==EMPTY || getTile(node->line+1,node->col)==FOOD || getTile(node->line+1,node->col)==AMMO))
            || (getType()==TYPE_HUMAN && node->line+1<getMapHeight() && ((getTile(node->line+1,node->col)>=HIDEOUT(0) && getTile(node->line+1,node->col)<HIDEOUT(MAX_HIDEOUT)) || getTile(node->line+1,node->col)==HOME(getGlobalId()) || getTile(node->line+1,node->col)==EMPTY || getTile(node->line+1,node->col)==FOOD || getTile(node->line+1,node->col)==AMMO))
            )
    {
        neighbour = createNode();
        neighbour->col = node->col;
        neighbour->line = node->line+1;
        addList(list,neighbour);
    }
    if(
            (getType()==TYPE_ZOMBIE && node->col+1<getMapWidth() && (getTile(node->line,node->col+1)==EMPTY || getTile(node->line,node->col+1)==FOOD || getTile(node->line,node->col+1)==AMMO))
            || (getType()==TYPE_HUMAN && node->col+1<getMapWidth() && ((getTile(node->line,node->col+1)>=HIDEOUT(0) && getTile(node->line,node->col+1)<HIDEOUT(MAX_HIDEOUT)) || getTile(node->line,node->col+1)==HOME(getGlobalId()) || getTile(node->line,node->col+1)==EMPTY || getTile(node->line,node->col+1)==FOOD || getTile(node->line,node->col+1)==AMMO))
            )
    {
        neighbour = createNode();
        neighbour->col = node->col+1;
        neighbour->line = node->line;
        addList(list,neighbour);
    }
    if(
            (getType()==TYPE_ZOMBIE && node->col-1>=0 && (getTile(node->line,node->col-1)==EMPTY || getTile(node->line,node->col-1)==FOOD || getTile(node->line,node->col-1)==AMMO))
            || (getType()==TYPE_HUMAN && node->col-1>=0 && ((getTile(node->line,node->col-1)>=HIDEOUT(0) && getTile(node->line,node->col-1)<HIDEOUT(MAX_HIDEOUT)) || getTile(node->line,node->col-1)==HOME(getGlobalId()) || getTile(node->line,node->col-1)==EMPTY || getTile(node->line,node->col-1)==FOOD || getTile(node->line,node->col-1)==AMMO))
            )
    {
        neighbour = createNode();
        neighbour->col = node->col-1;
        neighbour->line = node->line;
        addList(list,neighbour);
    }

    /* vérification pour les déplacements entre les cachettes */
    if(getType()==TYPE_HUMAN && getTile(node->line,node->col)>=HIDEOUT(0) && getTile(node->line,node->col)<HIDEOUT(MAX_HIDEOUT))
    {
        for(line=0 ; line<getMapHeight() ; ++line)
        {
            for(col=0 ; col<getMapWidth() ; ++col)
            {
                if(getTile(node->line,node->col)==getTile(line,col))
                {
                    neighbour = createNode();
                    neighbour->col = col;
                    neighbour->line = line;
                    addList(list,neighbour);
                }
            }
        }
    }

    if(getType()==TYPE_HUMAN && getTile(node->line,node->col)==HOME(getGlobalId()))
    {
        for(line=0 ; line<getMapHeight() ; ++line)
        {
            for(col=0 ; col<getMapWidth() ; ++col)
            {
                if(getTile(line,col)==HOME(getGlobalId()))
                {
                    neighbour = createNode();
                    neighbour->col = col;
                    neighbour->line = line;
                    addList(list,neighbour);
                }
            }
        }
    }
}

static float distance(node_t* first, node_t* second)
{
    return (first->line-second->line)*(first->line-second->line) + (first->col-second->col)*(first->col-second->col);
}

static char isInList(list_t* list,node_t* node, float* cost_f)
{
    unsigned int i;
    nodes_t* current = list->nodes;
    for(i=0 ; i<list->count ; ++i)
    {
        if(current->node->col==node->col && current->node->line==node->line)
        {
            *cost_f = current->node->cost_f;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

static char foundBest(node_t* end)
{
    unsigned int taille;        /* correspond à l'écart autour du point cliqué */
    unsigned int diffMax;
    unsigned int i;
    int found;
    int ligne;
    int colonne;

    /* recherche du maximum de la taille*/
    diffMax = end->line - 1;

    if (diffMax > end->col - 1)
        diffMax = end->col - 1;

    if (diffMax > getMapHeight() - end->line - 1)
        diffMax = getMapHeight() - end->line - 1;

    if (diffMax > getMapWidth() - end->col - 1)
        diffMax = getMapWidth() - end->col - 1;

    /* On cherche la zone accessible la plus proche du point cliqué
    recherche taille par taille, et dans cet ordre : ligne haut, ligne bas, ligne gauche (exclure bornes), ligne droite (aussi)*/
    found = 0;

    for (taille = 1; taille <= diffMax && !found; ++taille)
    {
        /*ligne du haut*/
        if (end->line - taille >= 0)
        {
            for (i = 0; i < taille * 2 + 1 && !found; ++i)
            {
                ligne = end->line - taille;
                colonne = end->col - taille + i;

                if(colonne >= 0 && colonne < getMapWidth() && (getTile(ligne,colonne)==EMPTY || getTile(ligne,colonne)==FOOD || getTile(ligne,colonne)==AMMO))
                {
                    end->line = ligne;
                    end->col = colonne;
                    found = 1;
                }
            }
        }

        /*ligne du bas*/
        if (end->line + taille < getMapHeight())
        {
            for (i = 0; i < taille * 2 + 1 && !found; ++i)
            {
                ligne = end->line + taille;
                colonne = end->col - taille + i;

                if(colonne >= 0 && colonne < getMapWidth() && (getTile(ligne,colonne)==EMPTY || getTile(ligne,colonne)==FOOD || getTile(ligne,colonne)==AMMO))
                {
                    end->line = ligne;
                    end->col = colonne;
                    found = 1;
                }
            }
        }

        /*colonne de gauche*/
        if (end->col - taille >= 0)
        {
            for (i = 1; i < taille * 2 && !found; ++i)
            {
                ligne = end->line - taille + i;
                colonne = end->col - taille;

                if(ligne >= 0 && ligne < getMapHeight() && (getTile(ligne,colonne)==EMPTY || getTile(ligne,colonne)==FOOD || getTile(ligne,colonne)==AMMO))
                {
                    end->line = ligne;
                    end->col = colonne;
                    found = 1;
                }
            }
        }

        /*colonne de droite*/
        if (end->col + taille)
        {
            for (i = 1; i < taille * 2 && !found; ++i)
            {
                ligne = end->line - taille + i;
                colonne = end->col + taille;

                if(ligne >= 0 && ligne < getMapHeight() && (getTile(ligne,colonne)==EMPTY || getTile(ligne,colonne)==FOOD || getTile(ligne,colonne)==AMMO))
                {

                    end->line = ligne;
                    end->col = colonne;
                    found = 1;
                }
            }
        }
    }

    return found;
}

list_t* find(node_t* start,node_t* end)
{
    list_t* openList;
    list_t* closeList;
    node_t* current;
    list_t* successor;
    nodes_t* currentSuccessor;
    list_t* ret;
    node_t* temp;
    float list_current_cost;
    unsigned int i;
    unsigned int count = 0;

    ret = createList();
    if(
            (getType()==TYPE_HUMAN && getTile(end->line,end->col)==OBSTACLE)
            || (getType()==TYPE_HUMAN && (getTile(end->line,end->col)>=HOME(0) && getTile(end->line,end->col)<HOME(MAX_PLAYER) && getTile(end->line,end->col)!=HOME(getGlobalId())))
            || (getType()==TYPE_ZOMBIE && getTile(end->line,end->col)!=EMPTY && getTile(end->line,end->col)!=FOOD && getTile(end->line,end->col)!=AMMO)
            )
    {
        if(foundBest(end)==0)
            return ret;
    }

    openList = createList();
    closeList = createList();
    current = createNode();

    start->cost_f = distance(start,end);
    addList(openList,start);
    current = start;


    while(openList->count!=0 && count<MAX_ASTAR_ITERATION)
    {
        ++count;
        current = getBestNode(openList);
        if(current!=NULL)
        {
            if(compareNode(current,end))
                break;
            successor = createList();
            addSuccessor(successor,current);
            currentSuccessor = successor->nodes;
            for(i=0 ; i<successor->count ; ++i)
            {
                currentSuccessor->node->cost_g = current->cost_g+distance(current,currentSuccessor->node);
                currentSuccessor->node->cost_h = distance(end,currentSuccessor->node);
                currentSuccessor->node->cost_f = currentSuccessor->node->cost_g + currentSuccessor->node->cost_h;

                if(isInList(openList,currentSuccessor->node,&list_current_cost) && list_current_cost<currentSuccessor->node->cost_f)
                {
                    deleteNode(currentSuccessor->node);
                    currentSuccessor = currentSuccessor->next;
                    continue;
                }
                else if(isInList(closeList,currentSuccessor->node,&list_current_cost) && list_current_cost<currentSuccessor->node->cost_f)
                {
                    deleteNode(currentSuccessor->node);
                    currentSuccessor = currentSuccessor->next;
                    continue;
                }
                else
                {
                    currentSuccessor->node->parent=current;
                    addList(openList,currentSuccessor->node);
                }
                currentSuccessor = currentSuccessor->next;
            }
            addList(closeList,current);
            deleteList(successor);
        }
        else
        {
            fprintf(stderr,"[find] Erreur lors de la recuperation du meilleur noeud\n");
            return ret;
        }
    }

    if(count<MAX_ASTAR_ITERATION)
    {
        while(current!=NULL)
        {
            temp = createNode();
            temp->line=current->line;
            temp->col=current->col;
            addList(ret,temp);
            current = current->parent;
        }
        if(temp!=NULL)
        {
            removeList(ret,temp);
            deleteNode(temp);
        }
    }


    deleteList(openList);
    deleteList(closeList);

    return ret;
}
