over#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <pthread.h>
#include <unistd.h>


#define MAX_TASKS 100
#define MAX_CORES 16


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


Task tasks[MAX_TASKS];
Core cores[MAX_CORES];


int numberOfTasks, numberOfCores;
float communicationVolume[MAX_TASKS][MAX_TASKS];


int compareEDF(const void *a, const void *b) {
    Task *taskA = (Task *)a;
    Task *taskB = (Task *)b;
    float deadlineA = taskA->period;
    float deadlineB = taskB->period;
    return (deadlineA > deadlineB) - (deadlineA < deadlineB);
}


void clusterTasksEDF(Task tasks[], Core cores[], int numberOfTasks, int numberOfCores, float communicationVolume[MAX_TASKS][MAX_TASKS]) {
    qsort(tasks, numberOfTasks, sizeof(Task), compareEDF);


    for (int i = 0; i < numberOfTasks; i++) {
        Task t = tasks[i];
        float minCost = FLT_MAX;
        int selectedCore = -1;


        for (int j = 0; j < numberOfCores; j++) {
            Core *c = &cores[j];
            if (c->utilization + t.utilization <= 1.0f) {
                float commCost = 0.0f;
                for (int k = 0; k < c->num_tasks; k++) {
                    int otherTaskId = c->assigned_tasks[k];
                    commCost += communicationVolume[t.id][otherTaskId];
                }
                if (commCost < minCost) {
                    minCost = commCost;
                    selectedCore = j;
                }
            }
        }


        if (selectedCore != -1) {
            Core *selected = &cores[selectedCore];
            selected->assigned_tasks[selected->num_tasks++] = t.id;
            selected->utilization += t.utilization;
        } else {
            printf("Task %d could not be assigned to any core!\n", t.id);
        }
    }
}


void printAndSaveResults(Core cores[], Task tasks[], int numberOfCores, const char* filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }


    fprintf(file, "CoreID,TaskID,WCET,Period,Utilization\n");


    for (int i = 0; i < numberOfCores; i++) {
        Core c = cores[i];
        printf("Core %d:\n", c.id);
        for (int j = 0; j < c.num_tasks; j++) {
            Task t = tasks[c.assigned_tasks[j]];
            printf("  Task %d - WCET: %.2f, Period: %.2f, Util: %.2f\n", t.id, t.WCET, t.period, t.utilization);
            fprintf(file, "%d,%d,%.2f,%.2f,%.2f\n", c.id, t.id, t.WCET, t.period, t.utilization);
        }
    }


    fclose(file);
}


void* simulate_core(void* arg) {
    int core_id = *(int*)arg;
    Core *c = &cores[core_id];


    printf("\n[Simulation] Core %d running %d tasks:\n", core_id, c->num_tasks);
    for (int i = 0; i < c->num_tasks; i++) {
        Task t = tasks[c->assigned_tasks[i]];
        printf("  Core %d executing Task %d (WCET = %.2fs)...\n", core_id, t.id, t.WCET);
        usleep((int)(t.WCET * 1e6));  // Simulate WCET in microseconds
        printf("  Core %d finished Task %d.\n", core_id, t.id);
    }
    return NULL;
}


int main() {
    printf("Enter number of tasks: ");
    scanf("%d", &numberOfTasks);
    printf("Enter number of cores: ");
    scanf("%d", &numberOfCores);


    if (numberOfTasks > MAX_TASKS || numberOfCores > MAX_CORES) {
        printf("Exceeded max tasks or cores.\n");
        return 1;
    }


    for (int i = 0; i < numberOfTasks; i++) {
        tasks[i].id = i;
        printf("\nTask %d:\n", i);
        printf("  WCET: ");
        scanf("%f", &tasks[i].WCET);
        printf("  Period: ");
        scanf("%f", &tasks[i].period);
        tasks[i].utilization = tasks[i].WCET / tasks[i].period;
    }


    printf("\nEnter communication volume matrix (%d x %d):\n", numberOfTasks, numberOfTasks);
    for (int i = 0; i < numberOfTasks; i++) {
        for (int j = 0; j < numberOfTasks; j++) {
            scanf("%f", &communicationVolume[i][j]);
        }
    }


    for (int i = 0; i < numberOfCores; i++) {
        cores[i].id = i;
        cores[i].utilization = 0.0f;
        cores[i].num_tasks = 0;
    }


    clusterTasksEDF(tasks, cores, numberOfTasks, numberOfCores, communicationVolume);
    printAndSaveResults(cores, tasks, numberOfCores, "clustering_results.csv");


    // Simulate execution
    pthread_t threads[MAX_CORES];
    int core_ids[MAX_CORES];


    for (int i = 0; i < numberOfCores; i++) {
        core_ids[i] = i;
        pthread_create(&threads[i], NULL, simulate_core, &core_ids[i]);
    }


    for (int i = 0; i < numberOfCores; i++) {
        pthread_join(threads[i], NULL);
    }


    printf("\nSimulation completed.\n");
    return 0;
}



