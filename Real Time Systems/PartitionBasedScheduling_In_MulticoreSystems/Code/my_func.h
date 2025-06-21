#ifndef MY_FUNC_H
#define MY_FUNC_H

#define MAX_TASKS 10
#define MAX_CORES 4

#include <stdbool.h>

typedef struct {
    int id;
    float WCET;
    float period;
    float utilization;
} Task;

typedef struct {
    int id;
    float utilization;
    int assigned_tasks[MAX_TASKS];
    int num_tasks;
} Core;

extern int numberOfCores;
extern int numberOfTasks;
extern float maximumUtilization;


void InitializeTasks();
void Initialize_CommMatrix();
void Initialize_Cores();

void Calculate1DCostMatrix();
float Calculate_Utilization(Task t);
float Calculating_TotalCommunicationCost();

void Task_Allocation(int task_id, int core_id);
void PrintingTaskAllocation();

int FindingOptimalCore();
bool CheckingIfTwoTasksAreCommunicating(int t1, int t2);
void Clustering();
int CompareEDF(const void *a, const void *b);
void SortTasksByEDF();

void ExportAllocationToFile();
void SaveAllocationToCSV();




#endif
