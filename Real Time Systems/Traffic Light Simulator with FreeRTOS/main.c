#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdlib.h>
#include <sys/time.h>
#include "FreeRTOSConfig.h"


#define RED_DELAY_MS     5000
#define GREEN_DELAY_MS   4000
#define YELLOW_DELAY_MS  1000

SemaphoreHandle_t redSemaphore;
SemaphoreHandle_t greenSemaphore;
SemaphoreHandle_t yellowSemaphore;

/* Called if configUSE_IDLE_HOOK is set to 1 */
void vApplicationIdleHook(void) {
    // Optional: Add code for when CPU is idle
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    printf("Stack overflow in task %s!\n", pcTaskName);
    abort(); // Optional: Halt execution for debugging
}

/* Called if configUSE_TICK_HOOK is set to 1 */
void vApplicationTickHook(void) {
    // Optional: Tick hook for system-level actions
}

/* Called at timer task startup */
void vApplicationDaemonTaskStartupHook(void) {
    // Optional: Code to run once the timer task starts
}

/* If static memory is used for idle task */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];
    
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* If static memory is used for timer task */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize) {
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];
    
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/* Called when configASSERT fails */
void vAssertCalled(const char * const pcFile, unsigned long ulLine) {
    printf("Assertion failed in %s at line %lu\n", pcFile, ulLine);
    abort();  // Optional: halt program
}

uint32_t uiTraceTimerGetFrequency(void) {
    return 1000000; // 1 MHz dummy timer frequency
}

uint32_t uiTraceTimerGetValue(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint32_t)(tv.tv_sec * 1000000 + tv.tv_usec); // microseconds
}

void vApplicationMallocFailedHook(void) {
    /* Called if a call to pvPortMalloc() fails. */
    printf("Malloc failed! Out of memory.\n");
    for (;;); // Optional: hang here to catch the issue
}

void vTraceTimerReset(void) {
    // Nothing needed for POSIX timer reset
}

void turnOnLight(const char* color) {
    printf("\n=== %s Light ON ===\n", color);
}

void turnOffLight(const char* color) {
    printf("--- %s Light OFF ---\n", color);
}

void vTaskRed(void *pvParameters) {
    for(;;) {
        xSemaphoreTake(redSemaphore, portMAX_DELAY);
        turnOnLight("RED");
        vTaskDelay(pdMS_TO_TICKS(RED_DELAY_MS));
        turnOffLight("RED");
        xSemaphoreGive(greenSemaphore);
    }
}

void vTaskGreen(void *pvParameters) {
    for(;;) {
        xSemaphoreTake(greenSemaphore, portMAX_DELAY);
        turnOnLight("GREEN");
        vTaskDelay(pdMS_TO_TICKS(GREEN_DELAY_MS));
        turnOffLight("GREEN");
        xSemaphoreGive(yellowSemaphore);
    }
}

void vTaskYellow(void *pvParameters) {
    for(;;) {
        xSemaphoreTake(yellowSemaphore, portMAX_DELAY);
        turnOnLight("YELLOW");
        vTaskDelay(pdMS_TO_TICKS(YELLOW_DELAY_MS));
        turnOffLight("YELLOW");
        xSemaphoreGive(redSemaphore);
    }
}

int main(void) {
    // Create binary semaphores
    redSemaphore = xSemaphoreCreateBinary();
    greenSemaphore = xSemaphoreCreateBinary();
    yellowSemaphore = xSemaphoreCreateBinary();

    if (redSemaphore == NULL || greenSemaphore == NULL || yellowSemaphore == NULL) {
        printf("Semaphore creation failed!\n");
        abort();
    }

    // Start with red light
    xSemaphoreGive(redSemaphore);

    // Create tasks
    xTaskCreate(vTaskRed, "RED_LED", 1024, NULL, 2, NULL);    // Higher priority
    xTaskCreate(vTaskGreen, "GREEN_LED", 1024, NULL, 1, NULL);
    xTaskCreate(vTaskYellow, "YELLOW_LED", 1024, NULL, 1, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    return 0;
}
