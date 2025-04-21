#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

FILE *logfile;

#define MAX_TASKS 5
#define CURRENT 1
#define IDLE_TASK_ID -1

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

SemaphoreHandle_t log_semaphore;
task arrayOfTasksForUser[MAX_TASKS];
int numberOfTasksForUser = 4;

void vApplicationMallocFailedHook(void) {}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {}


void Copy_Execution_Time(task *tasks, int index) {
    tasks[index].TaskAttributes[execution_copy] = tasks[index].TaskAttributes[wcet];
}

void Update_Absolute_arrivalTime(task *tasks, int index, int instance) {
    tasks[index].TaskAttributes[abs_arrival] =
        tasks[index].TaskAttributes[arrival_time] + instance * tasks[index].TaskAttributes[period];
}

void Update_Absolute_deadlineTime(task *tasks, int index) {
    tasks[index].TaskAttributes[abs_deadline] =
        tasks[index].TaskAttributes[abs_arrival] + tasks[index].TaskAttributes[deadline];
}

int InterruptForArrivalOfNewTask(task *tasks, int timer, int n) {
    int triggered = 0;
    for (int i = 0; i < n; i++) {
        if (tasks[i].TaskAttributes[abs_arrival] == timer) {
            tasks[i].alive = 1;
            Copy_Execution_Time(tasks, i);
            Update_Absolute_arrivalTime(tasks, i, tasks[i].instance);
            Update_Absolute_deadlineTime(tasks, i);
            triggered = 1;
        }
    }
    return triggered;
}

int findTaskIDwithLowestDeadline(task *tasks, int n) {
    int minDeadline = __INT_MAX__;
    int selected = IDLE_TASK_ID;
    for (int i = 0; i < n; i++) {
        if (tasks[i].alive && tasks[i].TaskAttributes[abs_deadline] < minDeadline &&
            tasks[i].TaskAttributes[execution_copy] > 0) {
            minDeadline = tasks[i].TaskAttributes[abs_deadline];
            selected = i;
        }
    }
    return selected;
}

void EDF_Task(void *params) {
    int timer = 0;
    while (1) {
        InterruptForArrivalOfNewTask(arrayOfTasksForUser, timer, numberOfTasksForUser);

        int activeID = findTaskIDwithLowestDeadline(arrayOfTasksForUser, numberOfTasksForUser);
        if (activeID != IDLE_TASK_ID) {
            task *activeTask = &arrayOfTasksForUser[activeID];
            if (activeTask->TaskAttributes[execution_copy] > 0) {
                activeTask->TaskAttributes[execution_copy]--;
            
                xSemaphoreTake(log_semaphore, portMAX_DELAY);
                printf("%d  Task %d\n", timer, activeID + 1);
                fprintf(logfile, "%d Task%d\n", timer, activeID + 1);  // ✅ This line writes to file
                fflush(logfile);  // Ensure it gets written immediately
                xSemaphoreGive(log_semaphore);
            }

            if (activeTask->TaskAttributes[execution_copy] == 0) {
                activeTask->alive = 0;
                activeTask->instance++;
                Update_Absolute_arrivalTime(arrayOfTasksForUser, activeID, activeTask->instance);
                Update_Absolute_deadlineTime(arrayOfTasksForUser, activeID);
            }
        } else {
            printf("%d  Idle\n", timer);
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // simulate 1 time unit = 100 ms
        timer++;
    }
}

int main() {
    arrayOfTasksForUser[0].TaskAttributes[period] = 6;
    arrayOfTasksForUser[0].TaskAttributes[wcet] = 2;
    arrayOfTasksForUser[0].TaskAttributes[deadline] = 6;
    arrayOfTasksForUser[0].TaskAttributes[arrival_time] = 0;
    arrayOfTasksForUser[0].instance = 0;

    arrayOfTasksForUser[1].TaskAttributes[period] = 3;
    arrayOfTasksForUser[1].TaskAttributes[wcet] = 1;
    arrayOfTasksForUser[1].TaskAttributes[deadline] = 3;
    arrayOfTasksForUser[1].TaskAttributes[arrival_time] = 0;
    arrayOfTasksForUser[1].instance = 0;

    arrayOfTasksForUser[2].TaskAttributes[period] = 7;
    arrayOfTasksForUser[2].TaskAttributes[wcet] = 1;
    arrayOfTasksForUser[2].TaskAttributes[deadline] = 7;
    arrayOfTasksForUser[2].TaskAttributes[arrival_time] = 0;
    arrayOfTasksForUser[2].instance = 0;

    arrayOfTasksForUser[3].TaskAttributes[period] = 9;
    arrayOfTasksForUser[3].TaskAttributes[wcet] = 2;
    arrayOfTasksForUser[3].TaskAttributes[deadline] = 9;
    arrayOfTasksForUser[3].TaskAttributes[arrival_time] = 0;
    arrayOfTasksForUser[3].instance = 0;

    for (int i = 0; i < numberOfTasksForUser; i++) {
        Copy_Execution_Time(arrayOfTasksForUser, i);
        Update_Absolute_arrivalTime(arrayOfTasksForUser, i, 0);
        Update_Absolute_deadlineTime(arrayOfTasksForUser, i);
    }

    log_semaphore = xSemaphoreCreateMutex();

    xTaskCreate(EDF_Task, "EDF_Task", configMINIMAL_STACK_SIZE + 1024, NULL, tskIDLE_PRIORITY + 1, NULL);

    logfile = fopen("edf_log.txt", "w");
    if (!logfile) {
    perror("Failed to open log file");
    return 1;
    }

    vTaskStartScheduler();
    fclose(logfile);
    return 0;
}
