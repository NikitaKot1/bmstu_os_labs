#include "errorss.h"
#include "strunct.h"
#include <time.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "apparatus.h"
#include "times.h"

double p = 0.7;
double t1_min = 0;
double t1_max = 6;
double t2_min = 1;
double t2_max = 8;

int main()
{
    printf("\nВероятность P: ");
    scanf("%lf", &p);
    double expected = expected_result();
    double all_time = 0,  downtime_t2 = 0,  downtime_t1 = 0;
    int col_of_actuation = 0;
    all_time = 0;
    downtime_t2 = 0;
    downtime_t1 = 0;
    col_of_actuation = 0;
    queuind_system_list_with_memory(&all_time, &downtime_t1, &downtime_t2, &col_of_actuation);
    expected = expected_result();
    printf("Ожидаемый результат: %lf е.в.\n\n", expected);
    printf("Время простоя ОА1: %lf е.в.\n", downtime_t1);
    printf("Время простоя ОА2: %lf е.в.\n", downtime_t2);
    printf("Отклонение: %lf%%\n\n", fabs(expected - all_time) / expected * 100);
    return 0;
}