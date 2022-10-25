#ifndef APARATUS_H
#define APARATUS_H

void queuind_system_list(double *all_time, double *downtime_t1, double *downtime_t2, int *col_of_actuation);
double expected_result();
void queuind_system_arr(double *all_time, double *downtime_t1, double *downtime_t2, int *col_of_actuation);
void queuind_system_list_with_memory(double *all_time, double *downtime_t1, double *downtime_t2, int *col_of_actuation);
void queuind_system_arr_with_memory(double *all_time, double *downtime_t1, double *downtime_t2, int *col_of_actuation);

#endif