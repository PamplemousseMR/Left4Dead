#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
    float cost_f,cost_g,cost_h;
    int line,col;
    struct node* parent;
}node_t;

typedef struct nodes
{
    node_t* node;
    struct nodes* next;
    struct nodes* previous;
}nodes_t;

typedef struct
{
    unsigned int count;
    nodes_t* nodes;
}list_t;

node_t* createNode();
void deleteNode(node_t*);

list_t* createList();
void deleteList(list_t*);
void deleteListWithNode(list_t*);
void addList(list_t*,node_t*);
void removeList(list_t*,node_t*);

#endif
