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
