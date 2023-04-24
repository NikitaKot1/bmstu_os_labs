#include "../inc/strunct.h"
#include "../inc/actions.h"
#include "../inc/examination.h"

#include "stddef.h"
#include <inttypes.h>
#include <sys/time.h>


double time_of_exam_stack(struct stack *ps, char *str)
{
    int64_t elapsed_time_k = 0;
    struct timeval tv_start, tv_stop;
    for (int i = 0; i < 10000; i++)
    {
        gettimeofday(&tv_start, NULL);
        exam_for_stack(str, ps);
        gettimeofday(&tv_stop, NULL);
        elapsed_time_k += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        full_free(&ps);
        ps = NULL;
    }
    return elapsed_time_k / (double)10000;
}

double time_of_exam_mass(char *st, char *str)
{
    int64_t elapsed_time_k = 0;
    struct timeval tv_start, tv_stop;
    for (int i = 0; i < 10000; i++)
    {
        gettimeofday(&tv_start, NULL);
        exam_for_mass(str, st);
        gettimeofday(&tv_stop, NULL);
        elapsed_time_k += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    return elapsed_time_k / (double)10000;
}