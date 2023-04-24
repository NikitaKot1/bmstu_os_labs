#include "strunct.h"
#include <stdlib.h>
#include <stdio.h>

void full_free(queue_list **list)
{
    while (*list)
    {
        queue_list *last = *list;
        *list = (*list)->next;
        free(last);
    }
    *list = NULL;
}

queue_list *create_node()
{
    queue_list *node = malloc(sizeof(queue_list));
    if (node)
        node->next = NULL;
    return node;
}

queue_list *delete_node(queue_list *list)
{
    queue_list *last = list;
    list = list->next;
    free(last);
    return list;
}