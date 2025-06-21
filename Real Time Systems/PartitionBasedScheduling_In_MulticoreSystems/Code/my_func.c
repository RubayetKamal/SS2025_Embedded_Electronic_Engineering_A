#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h> 
#include "my_func.h"


int numberOfCores;
int numberOfTasks;
float maximumUtilization;



Task tasks[MAX_TASKS];
Core cores[MAX_CORES];

float communicationVolume[MAX_TASKS][MAX_TASKS];  // SM_{i,j}
float costMatrix[MAX_CORES][MAX_CORES];  // X_{Ck,Cl}

void SaveAllocationToCSV() {
    FILE *file = fopen("allocation.csv", "w");
    if (!file) {
        printf("Failed to open allocation.csv for writing.\n");
        return;
    }

    fprintf(file, "TaskID,CoreID,StartTime,Duration\n");

    for (int i = 0; i < numberOfCores; i++) {
        float currentTime = 0.0;
        for (int j = 0; j < cores[i].num_tasks; j++) {
            int task_id = cores[i].assigned_tasks[j];
            Task t = tasks[task_id];

            fprintf(file, "%d,%d,%.2f,%.2f\n", t.id, cores[i].id, currentTime, t.WCET);
            currentTime += t.WCET;  // accumulate time for the next task on this core
        }
    }

    fclose(file);
    printf("allocation.csv written successfully.\n");
}

int CompareEDF(const void *a, const void *b) {
    int t1 = *(int*)a;
    int t2 = *(int*)b;
    return (tasks[t1].period > tasks[t2].period) - (tasks[t1].period < tasks[t2].period);
}

void SortTasksByEDF() {
    for (int i = 0; i < numberOfCores; i++) {
        qsort(cores[i].assigned_tasks, cores[i].num_tasks, sizeof(int), CompareEDF);
    }
}

void InitializeTasks() {
    tasks[0] = (Task){0, 1, 5};  // U = 0.2
    tasks[1] = (Task){1, 2, 10}; // U = 0.2
    tasks[2] = (Task){2, 1, 4};  // U = 0.25
    tasks[3] = (Task){3, 2, 8};  // U = 0.25
    tasks[4] = (Task){4, 3, 4};  // U = 0.2
    tasks[5] = (Task){5, 1, 10};  
    tasks[6] = (Task){6, 5, 6};
}
void Initialize_CommMatrix(){
    float temporaryCommMatrix[7][7] = { 
        {0, 5, 3, 9, 0, 0, 0},
        {5, 0, 4, 7, 0, 0, 0},
        {3, 4, 0, 0, 0, 0, 0},
        {9, 7, 0, 0, 0, 8, 0},
        {0, 0, 0, 0, 0, 0, 7},
        {0, 0, 0, 8, 0, 0, 0},
        {0, 0, 0, 0, 7, 0, 0},
    };
    memcpy(communicationVolume, temporaryCommMatrix, sizeof(temporaryCommMatrix));
}


void Initialize_Cores() {
    for (int i = 0; i < numberOfCores; i++) {
        cores[i].id = i;
        cores[i].utilization = 0;
        cores[i].num_tasks = 0;
    }

}

void Calculate1DCostMatrix(){
    for (int i = 0; i < numberOfCores; i++) {
        for (int j = 0; j < numberOfCores; j++) {
            costMatrix[i][j] = abs(i - j); 
        }
    }
}


float Calculate_Utilization(Task t) {
    return t.WCET / t.period;
}

void Task_Allocation(int task_id, int core_id) {
    cores[core_id].assigned_tasks[cores[core_id].num_tasks++] = task_id;
    cores[core_id].utilization += Calculate_Utilization(tasks[task_id]);
}


int FindingOptimalCore() {
    float minimum_utilizationFactor = FLT_MAX;
    int selectedCore = -1; //holds the care with least uitlization
    for (int i = 0; i < numberOfCores; i++) {
        if (cores[i].utilization < minimum_utilizationFactor) {
            minimum_utilizationFactor = cores[i].utilization; 
            selectedCore = i;
        }
    }
    return selectedCore;
}

bool CheckingIfTwoTasksAreCommunicating(int t1, int t2) {
    if(communicationVolume[t1][t2] > 0 || communicationVolume[t2][t1] > 0){
        return true;
    }else{
        return false;
    } 
}

float Calculating_TotalCommunicationCost() {
    float totalCost = 0.0;

    for (int firstCore = 0; firstCore < numberOfCores; firstCore++) {
        for (int secondCore = 0; secondCore < numberOfCores; secondCore++) {

            for (int i = 0; i < cores[firstCore].num_tasks; i++) {
                for (int j = 0; j < cores[secondCore].num_tasks; j++) {

                    int firstTask = cores[firstCore].assigned_tasks[i];
                    int secondTask = cores[secondCore].assigned_tasks[j];

                    totalCost += costMatrix[firstCore][secondCore] * communicationVolume[firstTask][secondTask];
                }
            }
        }
    }

    return totalCost;
}

void Clustering() {


    for (int i = 0; i < numberOfTasks; i++) {
        tasks[i].utilization = Calculate_Utilization(tasks[i]);

        // Try to cluster with already assigned communicating tasks
        int leastLoadedCore = -1;

        for (int c = 0; c < numberOfCores; c++) {
            if (cores[c].utilization + tasks[i].utilization > maximumUtilization)
                continue;

            for (int t = 0; t < cores[c].num_tasks; t++) {
                int assigned_task = cores[c].assigned_tasks[t];
                if (CheckingIfTwoTasksAreCommunicating(i, assigned_task)) {
                    leastLoadedCore = c;
                    break;
                }
            }
            if (leastLoadedCore != -1) break;
        }
        // If no related cluster, choose least loaded core
        if (leastLoadedCore == -1)
            leastLoadedCore = FindingOptimalCore();

       Task_Allocation(i, leastLoadedCore);
    }
}

void PrintingTaskAllocation() {
    for (int i = 0; i < numberOfCores; i++) {
        printf("Core %d [Util: %.2f]: ", i, cores[i].utilization);
        for (int j = 0; j < cores[i].num_tasks; j++) {
            printf("T%d ", cores[i].assigned_tasks[j]);
        }
        printf("\n");
    }
}
