#ifndef EDF_FUNCTIONS_H
#define EDF_FUNCTIONS_H

#include "FreeRTOS.h"
#include "task.h"

#define MAX_TASKS 5

typedef enum {
    arrival_time,
    period,
    wcet,
    deadline,
    abs_arrival,
    abs_deadline,
    execution_copy
} TaskAttributesIndex;

typedef struct {
    int TaskAttributes[7];
    int alive;
    int instance;
    TaskHandle_t taskHandle;
} task;

void Copy_Execution_Time(task *tasks, int index);
void Update_Absolute_arrivalTime(task *tasks, int index, int instance);
void Update_Absolute_deadlineTime(task *tasks, int index);
int InterruptForArrivalOfNewTask(task *tasks, int timer, int n);
int findTaskIDwithLowestDeadline(task *tasks, int n);

#endif
