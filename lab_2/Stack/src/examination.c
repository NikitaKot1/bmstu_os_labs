#include "../inc/strunct.h"
#include <stdlib.h>
#include "../inc/errors.h"
#include "../inc/actions.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool prof(char a, char b)
{
    if (a == '(' && b == ')')
        return true;
    if (a == '[' && b == ']')
        return true;
    if (a == '{' && b == '}')
        return true;
    return false;
}

int exam_for_stack(char *str, struct stack *ps)
{
    char scobs_in[] = "({[";
    char scobs_out[] = "]})";
    char symb;
    int rc = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        symb = str[i];
        if (strchr(scobs_in, symb))
        {
            add_to_stack(&ps, symb);
        }
            
        else if (strchr(scobs_out, symb))
        {
            if (!ps)
                return NOT_OK;
            else if (prof(ps->el, symb))
            {
                rc = delete_from_stack(&ps);
                if (rc != OK)
                    return NOT_OK;
            }
            else
            {
                return NOT_OK;
            }
        }            
    }
    if (ps == NULL)
        return OK;
    else
    {
        return NOT_OK;
    }
        
}

int exam_for_mass(char *str, char *mass)
{
    char scobs_in[] = "({[";
    char scobs_out[] = "]})";
    char symb;
    int n = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        symb = str[i];
        if (strchr(scobs_in, symb))
        {
            mass[n] = symb;
            n++;
            if (n > N)
                return TOO_MACH;
        }
        else if (strchr(scobs_out, symb))
        {
            if (n > 0 && prof(mass[n], symb))
            {
                n--;
            }
            else
                return NOT_OK;
        }
    }
    if (n == 0)
        return OK;
    else
        return NOT_OK;
}

void del_from_mem(void *k[N], int n, int in)
{
    for (int i = in; i < n - 1; i++)
        k[i] = k[i + 1];
}

int exam_for_stack_memory(char *str, struct stack *ps, void *k[N], int *n)
{
    char scobs_in[] = "({[";
    char scobs_out[] = "]})";
    char symb;
    int rc = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        symb = str[i];
        if (strchr(scobs_in, symb))
        {
            add_to_stack(&ps, symb);
            for (int i = 0; i < *n; i++)
            {
                if (k[i] == ps)
                    del_from_mem(k, *n, i);
                (*n)--;
            }
        }
            
        else if (strchr(scobs_out, symb))
        {
            if (!ps)
                return NOT_OK;
            else if (prof(ps->el, symb))
            {
                k[*n] = ps;
                (*n)++;
                rc = delete_from_stack(&ps);
                if (rc != OK)
                    return NOT_OK;
            }
            else
            {
                return NOT_OK;
            }
        }            
    }
    if (ps == NULL)
        return OK;
    else
    {
        return NOT_OK;
    }
        
}