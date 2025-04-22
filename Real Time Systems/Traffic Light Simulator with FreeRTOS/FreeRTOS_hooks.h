#ifndef FREERTOS_HOOKS_H
#define FREERTOS_HOOKS_H

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
void vApplicationTickHook(void);
void vApplicationDaemonTaskStartupHook(void);
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);
void vAssertCalled(const char * const pcFile, unsigned long ulLine);
void vApplicationMallocFailedHook(void);
uint32_t uiTraceTimerGetFrequency(void);
uint32_t uiTraceTimerGetValue(void);
void vTraceTimerReset(void);

#endif // FREERTOS_HOOKS_H
