#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "strunct.h"
#include "actions.h"

extern double p;
extern double t1_min;
extern double t1_max;
extern double t2_min;
extern double t2_max;

double expected_result()
{
    double t2_all = (t2_max + t2_min) / 2 * 1000;
    double t2_come = (t1_max + t1_min) / 2 / (1 - p) * 1000;
    if (t2_come > t2_all)
        return t2_come;
    double expect = t2_all;
    return expect;
}

bool service_apparatus_choise()
{
    double choise = (double)rand()/(double)RAND_MAX;
    return choise < p;
}

double sevise_apparatus_1()
{
    double t1_time = (double)rand()/(double)RAND_MAX * (t1_max - t1_min) + t1_min;
    return t1_time;
}

double sevise_apparatus_2()
{
    double t2_time = (double)rand()/(double)RAND_MAX * (t2_max - t2_min) + t2_min;
    return t2_time;
}


int col_of_list(queue_list *list)
{
    int n = 0;
    while (list)
    {
        list = list->next;
        n++;
    }
    return n;
}


void queuind_system_list(double *all_time, double *downtime_t1, double *downtime_t2, int *col_of_actuation)
{
    queue_list *start_t1 = NULL, *start_t2 = NULL, *end_t2 = NULL;
    start_t1 = create_node();

    queue_list *end_t1 = start_t1;
    for (int i = 0; i < 99; i++)
    {
        start_t1->next = create_node();
        start_t1 = start_t1->next;
    }

    double t1_time_wait = -1, t2_time_wait = -1;
    double t1_time_obr = 0, t2_time_obr = 0;
    bool downtime_t2_flag = true, downtime_t1_flag = true;
    int col_exit = 0;
    while (col_exit < 1000)
    {
        downtime_t2_flag = true;
        downtime_t1_flag = true;
        t1_time_wait = -1;
        t2_time_wait = -1;
        if (t2_time_obr < 1e-6)
        {
            if (end_t2)
            {
                downtime_t2_flag = false;
                if (end_t2 == start_t2)
                {
                    end_t2 = delete_node(end_t2);
                    start_t2 = NULL;
                }
                else
                    end_t2 = delete_node(end_t2);
                
                t2_time_wait = sevise_apparatus_2();
                t2_time_obr += t2_time_wait;
                col_exit++;

                if (start_t1)
                {
                    start_t1->next = create_node();
                    start_t1 = start_t1->next;
                }
                else
                {
                    start_t1 = create_node();
                    if (!end_t1)
                        end_t1 = start_t1;
                }
            }
        }
        else
        {
            downtime_t2_flag = false;
        }
                
        if (t1_time_obr < 1e-6)
        {
            if (end_t1)
            {
                downtime_t1_flag = false;
                if (end_t1 == start_t1)
                {
                    end_t1 = delete_node(end_t1);
                    start_t1 = NULL;
                }
                else
                    end_t1 = delete_node(end_t1);
                
                t1_time_wait = sevise_apparatus_1();
                t1_time_obr += t1_time_wait;
                (*col_of_actuation)++;

                if (service_apparatus_choise())
                {
                    if (start_t1)
                    {
                        start_t1->next = create_node();
                        start_t1 = start_t1->next;
                    }
                    else
                    {
                        start_t1 = create_node();
                        if (!end_t1)
                            end_t1 = start_t1;
                    }
                }
                else
                {
                    if (!start_t2)
                    {
                        start_t2 = create_node();
                        end_t2 = start_t2;
                    }
                    else
                    {
                        start_t2->next = create_node();
                        start_t2 = start_t2->next;
                    }
                }
            }
        }
        else
            downtime_t1_flag = false;

        if (t1_time_obr < t2_time_obr)
        {
            t2_time_obr -= t1_time_obr;
            *all_time += t1_time_obr;
            t1_time_obr = 0;
        }
        else
        {
            t1_time_obr -= t2_time_obr;
            *all_time += t2_time_obr;
            t2_time_obr = 0;
        }
        if (downtime_t2_flag)
        {
            *downtime_t2 += t1_time_obr;
            *all_time += t1_time_obr;
            t1_time_obr = 0;
        }
        if (downtime_t1_flag)
        {
            *downtime_t1 += t2_time_obr;
            *all_time += t2_time_obr;
            t2_time_obr = 0;
        }
    }
    full_free(&end_t2);
    full_free(&end_t1);
}


void queuind_system_arr(double *all_time, double *downtime_t1, double *downtime_t2, int *col_of_actuation)
{
    queue_arr t1;
    t1.q1 = malloc(100 * sizeof(char));
    t1.qm = t1.q1 + 99;
    t1.pin = t1.q1;
    t1.pout = t1.q1 + 99;
    t1.m = 100;

    queue_arr t2;
    t2.q1 = malloc(100 * sizeof(char));
    t2.qm = t2.q1 + 1;
    t2.pin = t2.q1;
    t2.pout = t2.q1;
    t2.m = 0;

    double t1_time_wait = -1, t2_time_wait = -1;
    double t1_time_obr = 0, t2_time_obr = 0;
    bool downtime_t2_flag = true, downtime_t1_flag = true;
    int col_exit = 0;
    while (col_exit < 1000)
    {
        downtime_t2_flag = true;
        downtime_t1_flag = true;
        t1_time_wait = -1;
        t2_time_wait = -1;
        if (t2_time_obr < 1e-6)
        {
            if (t2.m > 0)
            {
                downtime_t2_flag = false;

                if (t2.pout == t2.qm)
                    t2.pout = t2.q1;
                else
                    t2.pout += 1;
                t2.m--;

                t2_time_wait = sevise_apparatus_2();
                t2_time_obr += t2_time_wait;
                col_exit++;

                if (t1.pin == t1.qm)
                    t1.pin = t1.q1;
                else
                    t1.pin += 1;
                t1.m++;
            }
        }
        else
        {
            downtime_t2_flag = false;
        }

        if (t1_time_obr < 1e-6)
        {
            if (t1.m > 0)
            {
                downtime_t1_flag = false;

                if (t1.pout == t1.qm)
                    t1.pout = t1.q1;
                else
                    t1.pout += 1;
                t1.m--;
                
                t1_time_wait = sevise_apparatus_1();
                t1_time_obr += t1_time_wait;
                (*col_of_actuation)++;

                if (service_apparatus_choise())
                {
                    if (t1.pin == t1.qm)
                        t1.pin = t1.q1;
                    else
                        t1.pin += 1;
                    t1.m++;
                }
                else
                {
                    if (t2.pin == t2.qm)
                        t2.pin = t2.q1;
                    else
                        t2.pin += 1;
                    t2.m++;
                }
            }
        }
        else
            downtime_t1_flag = false;
        

        if (t1_time_obr < t2_time_obr)
        {
            t2_time_obr -= t1_time_obr;
            *all_time += t1_time_obr;
            t1_time_obr = 0;
        }
        else
        {
            t1_time_obr -= t2_time_obr;
            *all_time += t2_time_obr;
            t2_time_obr = 0;
        }
        if (downtime_t2_flag)
        {
            *downtime_t2 += t1_time_obr;
            *all_time += t1_time_obr;
            t1_time_obr = 0;
        }
        if (downtime_t1_flag)
        {
            *downtime_t1 += t2_time_obr;
            *all_time += t2_time_obr;
            t2_time_obr = 0;
        }
    }
    free(t1.q1);
    free(t2.q1);
}

void queuind_system_list_with_memory(double *all_time, double *downtime_t1, double *downtime_t2, int *col_of_actuation)
{
    queue_list *start_t1 = NULL, *start_t2 = NULL, *end_t2 = NULL;
    start_t1 = create_node();

    queue_list *end_t1 = start_t1;
    for (int i = 0; i < 99; i++)
    {
        start_t1->next = create_node();
        start_t1 = start_t1->next;
    }

    FILE* f = fopen("memory_logs.txt", "w");

    int n_of_list_1 = 0;
    int n_of_list_2 = 0;
    int n_obsh_1 = 0, n_obsh_2 = 0;

    double t1_time_wait = -1, t2_time_wait = -1;
    double t1_time_obr = 0, t2_time_obr = 0;
    bool downtime_t2_flag = true, downtime_t1_flag = true;
    bool downtime_not_t2 = true;
    int col_exit = 0;
    while (col_exit < 1000)
    {
        downtime_t2_flag = true;
        downtime_t1_flag = true;
        downtime_not_t2 = true;
        t1_time_wait = -1;
        t2_time_wait = -1;
        if (t2_time_obr < 1e-6)
        {
            if (end_t2)
            {
                downtime_t2_flag = false;
                fprintf(f, "queue2 delete: %p\n", end_t2);
                if (end_t2 == start_t2)
                {
                    end_t2 = delete_node(end_t2);
                    start_t2 = NULL;
                }
                else
                    end_t2 = delete_node(end_t2);
                
                t2_time_wait = sevise_apparatus_2();
                t2_time_obr += t2_time_wait;
                col_exit++;

                if (start_t1)
                {
                    start_t1->next = create_node();
                    start_t1 = start_t1->next;
                }
                else
                {
                    start_t1 = create_node();
                    if (!end_t1)
                        end_t1 = start_t1;
                }
            }
        }
        else
        {
            downtime_t2_flag = false;
            downtime_not_t2 = false;
        }
                
        if (t1_time_obr < 1e-6)
        {
            if (end_t1)
            {
                downtime_t1_flag = false;
                if (end_t1 == start_t1)
                {
                    end_t1 = delete_node(end_t1);
                    start_t1 = NULL;
                }
                else
                    end_t1 = delete_node(end_t1);
                
                t1_time_wait = sevise_apparatus_1();
                t1_time_obr += t1_time_wait;
                (*col_of_actuation)++;

                if (service_apparatus_choise())
                {
                    if (start_t1)
                    {
                        start_t1->next = create_node();
                        start_t1 = start_t1->next;
                    }
                    else
                    {
                        start_t1 = create_node();
                        if (!end_t1)
                            end_t1 = start_t1;
                    }
                }
                else
                {
                    if (!start_t2)
                    {
                        start_t2 = create_node();
                        end_t2 = start_t2;
                    }
                    else
                    {
                        start_t2->next = create_node();
                        start_t2 = start_t2->next;
                    }
                }
            }
        }
        else
            downtime_t1_flag = false;

        if (t1_time_obr < t2_time_obr)
        {
            t2_time_obr -= t1_time_obr;
            *all_time += t1_time_obr;
            t1_time_obr = 0;
        }
        else
        {
            t1_time_obr -= t2_time_obr;
            *all_time += t2_time_obr;
            t2_time_obr = 0;
        }
        if (downtime_t2_flag)
        {
            *downtime_t2 += t1_time_obr;
            *all_time += t1_time_obr;
            t1_time_obr = 0;
        }
        if (!downtime_t2_flag && downtime_not_t2)
        {
            n_of_list_1 = col_of_list(end_t1);
            n_of_list_2 = col_of_list(end_t2);
            n_obsh_1 += n_of_list_1;
            n_obsh_2 += n_of_list_2;
        }
        if (downtime_t1_flag)
        {
            *downtime_t1 += t2_time_obr;
            *all_time += t2_time_obr;
            t2_time_obr = 0;
        }
    }
    full_free(&end_t2);
    full_free(&end_t1);
    fclose(f);
}


void queuind_system_arr_with_memory(double *all_time, double *downtime_t1, double *downtime_t2, int *col_of_actuation)
{
    queue_arr t1;
    t1.q1 = malloc(100 * sizeof(char));
    t1.qm = t1.q1 + 99;
    t1.pin = t1.q1;
    t1.pout = t1.q1 + 99;
    t1.m = 100;

    queue_arr t2;
    t2.q1 = malloc(100 * sizeof(char));
    t2.qm = t2.q1 + 1;
    t2.pin = t2.q1;
    t2.pout = t2.q1;
    t2.m = 0;

    int n_of_list_1 = 0;
    int n_of_list_2 = 0;
    int n_obsh_1 = 0, n_obsh_2 = 0;

    double t1_time_wait = -1, t2_time_wait = -1;
    double t1_time_obr = 0, t2_time_obr = 0;
    bool downtime_t2_flag = true, downtime_t1_flag = true;
    bool downtime_not_t2 = true;
    int col_exit = 0;
    while (col_exit < 1000)
    {
        downtime_t2_flag = true;
        downtime_t1_flag = true;
        downtime_not_t2 = true;
        t1_time_wait = -1;
        t2_time_wait = -1;
        if (t2_time_obr < 1e-6)
        {
            if (t2.m > 0)
            {
                downtime_t2_flag = false;

                if (t2.pout == t2.qm)
                    t2.pout = t2.q1;
                else
                    t2.pout += 1;
                t2.m--;

                t2_time_wait = sevise_apparatus_2();
                t2_time_obr += t2_time_wait;
                col_exit++;

                if (t1.pin == t1.qm)
                    t1.pin = t1.q1;
                else
                    t1.pin += 1;
                t1.m++;
            }
        }
        else
        {
            downtime_t2_flag = false;
            downtime_not_t2 = false;
        }

        if (t1_time_obr < 1e-6)
        {
            if (t1.m > 0)
            {
                downtime_t1_flag = false;

                if (t1.pout == t1.qm)
                    t1.pout = t1.q1;
                else
                    t1.pout += 1;
                t1.m--;
                
                t1_time_wait = sevise_apparatus_1();
                t1_time_obr += t1_time_wait;
                (*col_of_actuation)++;

                if (service_apparatus_choise())
                {
                    if (t1.pin == t1.qm)
                        t1.pin = t1.q1;
                    else
                        t1.pin += 1;
                    t1.m++;
                }
                else
                {
                    if (t2.pin == t2.qm)
                        t2.pin = t2.q1;
                    else
                        t2.pin += 1;
                    t2.m++;
                }
            }
        }
        else
            downtime_t1_flag = false;
        

        if (t1_time_obr < t2_time_obr)
        {
            t2_time_obr -= t1_time_obr;
            *all_time += t1_time_obr;
            t1_time_obr = 0;
        }
        else
        {
            t1_time_obr -= t2_time_obr;
            *all_time += t2_time_obr;
            t2_time_obr = 0;
        }
        if (downtime_t2_flag)
        {
            *downtime_t2 += t1_time_obr;
            *all_time += t1_time_obr;
            t1_time_obr = 0;
        }
        if (!downtime_t2_flag && downtime_not_t2)
        {
            n_of_list_1 = t1.m;
            n_of_list_2 = t2.m;
            n_obsh_1 += n_of_list_1;
            n_obsh_2 += n_of_list_2;
        }
        if (downtime_t1_flag)
        {
            *downtime_t1 += t2_time_obr;
            *all_time += t2_time_obr;
            t2_time_obr = 0;
        }
    }
    free(t1.q1);
    free(t2.q1);
}