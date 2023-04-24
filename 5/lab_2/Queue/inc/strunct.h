#ifndef STRUCT_H
#define STRUCT_H

typedef struct queue_list queue_list;

struct queue_list{
    struct queue_list *next;
};

typedef struct{
    char *q1;
    char *qm;
    char *pin;
    char *pout;
    int m;
}queue_arr;

#endif