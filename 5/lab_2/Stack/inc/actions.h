#ifndef ACTIONS_H
#define ACTIONS_H

int add_to_stack(struct stack**, char);
int delete_from_stack(struct stack**);
void view_of_stack(struct stack*, int);
void full_free(struct stack**);

#endif