#include "../inc/strunct.h"
#include <stdlib.h>
#include "../inc/errors.h"
#include <stdio.h>


int add_to_stack(struct stack **ps, char new_el)
{
    struct stack *new =  malloc(sizeof(struct stack));
    if (!new)
        return MEM_ERR;
    new->el = new_el;
    new->next = *ps;
    *ps = new;
    return OK;
}

int delete_from_stack(struct stack **ps)
{
    if (!ps)
        return FAIL_ST;
    struct stack *new = (*ps)->next;
    free(*ps);
    *ps = new;
    return OK;
}

void view_of_stack(struct stack *ps)
{
    struct stack *view = ps;
    while (view != NULL)
        printf("%c ", view->el);
    printf("\n");
}

void full_free(struct stack **ps)
{
    struct stack *a = *ps;
    while (*ps != NULL)
    {
        *ps = a->next;
        free(a);
        a = *ps;
    }
}