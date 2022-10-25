#include <inttypes.h>
#include <sys/time.h>
#include "apparatus.h"
#include "stddef.h"

double time_system_list()
{
    int64_t elapsed_time_k = 0;
    struct timeval tv_start, tv_stop;
    for (int i = 0; i < 100; i++)
    {
        double all_time = 0,  downtime_t2 = 0,  downtime_t1 = 0;
        int col_of_actuation = 0;

        gettimeofday(&tv_start, NULL);
        queuind_system_list(&all_time, &downtime_t1, &downtime_t2, &col_of_actuation);
        gettimeofday(&tv_stop, NULL);
        elapsed_time_k += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    return elapsed_time_k / (double)100;
}

double time_system_arr()
{
    int64_t elapsed_time_k = 0;
    struct timeval tv_start, tv_stop;
    for (int i = 0; i < 100; i++)
    {
        double all_time = 0,  downtime_t2 = 0,  downtime_t1 = 0;
        int col_of_actuation = 0;

        gettimeofday(&tv_start, NULL);
        queuind_system_arr(&all_time, &downtime_t1, &downtime_t2, &col_of_actuation);
        gettimeofday(&tv_stop, NULL);
        elapsed_time_k += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    return elapsed_time_k / (double)100;
}