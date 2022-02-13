#include <stdio.h>
#include <stdlib.h>

typedef struct list_node_
{
    int x, y;
    struct list_node_ *next;
} list_node;

typedef struct
{
    list_node *root;
    int size;
} linked_list;

void init_list(linked_list *list);
void reset_list(linked_list *list, int *PREV_X, int *PREV_Y);
void add_node(linked_list *list, int x, int y);
void free_list(linked_list *list);