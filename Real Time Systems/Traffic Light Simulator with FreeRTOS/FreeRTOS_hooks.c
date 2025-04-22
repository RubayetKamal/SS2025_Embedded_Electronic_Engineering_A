#include "FreeRTOS_hooks.h"

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
