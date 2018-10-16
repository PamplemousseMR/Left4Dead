#include "list.h"

/* =============== Noeud =============== */

node_t* createNode()
{
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if(node==NULL)
    {
        fprintf(stderr,"[createNode] Erreur lors du malloc\n");
        exit(EXIT_FAILURE);
    }

    node->col = 0;
    node->line = 0;
    node->cost_f = 0.0;
    node->cost_g = 0.0;
    node->cost_h = 0.0;
    node->parent = NULL;

    return node;
}

void deleteNode(node_t* node)
{
    if(node!=NULL)
    {
        free(node);
        node = NULL;
    }
}

/* =============== List =============== */

list_t* createList()
{
    list_t* list = (list_t*)malloc(sizeof(list_t));
    if(list==NULL)
    {
        fprintf(stderr,"[createList] Erreur lors du malloc\n");
        exit(EXIT_FAILURE);
    }

    list->count = 0;
    list->nodes = NULL;

    return list;
}

void deleteList(list_t* list)
{
    unsigned int i;
    nodes_t* temp;
    nodes_t* current;
    if(list!=NULL)
    {
        current= list->nodes;
        for(i=0 ; i<list->count ; ++i)
        {
            temp = current;
            current = current->next;
            free(temp);
            temp = NULL;
        }
    }
    free(list);
}

void deleteListWithNode(list_t* list)
{
    unsigned int i;
    node_t* node;
    nodes_t* temp;
    nodes_t* current;
    if(list!=NULL)
    {
        current= list->nodes;
        for(i=0 ; i<list->count ; ++i)
        {
            temp = current;
            node = current->node;
            current = current->next;
            deleteNode(node);
            free(temp);
            temp = NULL;
        }
    }
    free(list);
}

void addList(list_t* list,node_t* node)
{
    nodes_t* nodes;
    if(list!=NULL && node!=NULL)
    {
        ++list->count;
        nodes = (nodes_t*)malloc(sizeof(nodes_t));
        if(nodes==NULL)
        {
            fprintf(stderr,"[createList] Erreur lors du malloc\n");
            exit(EXIT_FAILURE);
        }
        nodes->next = list->nodes;
        nodes->previous = NULL;
        if(list->nodes!=NULL)
            list->nodes->previous = nodes;
        nodes->node = node;
        list->nodes = nodes;
    }
}

void removeList(list_t* list,node_t* node)
{
    nodes_t* current;
    unsigned int i;
    char found;
    if(list!=NULL && node!=NULL)
    {
        found = 0;
        current = list->nodes;
        for(i=0 ; i<list->count ; ++i)
        {
            if(current->node==node)
            {
                found = 1;
                break;
            }
            current=current->next;
        }
        if(found)
        {
            if(current->next!=NULL)
                current->next->previous = current->previous;
            if(current->previous!=NULL)
                current->previous->next = current->next;
            else
                list->nodes = current->next;
            free(current);
            current = NULL;
            --list->count;
        }
    }
}
