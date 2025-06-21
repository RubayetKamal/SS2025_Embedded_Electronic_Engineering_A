#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "my_func.h"
#include <stdbool.h>



int main() {
    numberOfCores = 3;
    numberOfTasks = 7;
    maximumUtilization = 1.0;
    
    InitializeTasks();
    Initialize_CommMatrix();
    Initialize_Cores();
    Calculate1DCostMatrix();

    Clustering();
    SortTasksByEDF();


    PrintingTaskAllocation();

    float totalCommunicationCost = Calculating_TotalCommunicationCost();
    printf("Total Communication Cost: %.2f\n", totalCommunicationCost);
    SaveAllocationToCSV();

    return 0;
}
