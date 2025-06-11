#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stdio.h>

// Queue handle
QueueHandle_t xQueue;

// Task1: Sends data to the queue
void vSenderTask(void *pvParameters)
{
    int valueToSend = 100;
    while (1)
    {
        // Send data to the queue (wait up to 100ms if full)
        if (xQueueSend(xQueue, &valueToSend, pdMS_TO_TICKS(100)) == pdPASS)
        {
            printf("Sender: Sent value %d to queue.\n", valueToSend);
        }
        else
        {
            printf("Sender: Failed to send value.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));  // Wait 1 second
    }
}

// Task2: Receives data from the queue
void vReceiverTask(void *pvParameters)
{
    int receivedValue;
    while (1)
    {
        // Wait for data for up to 500ms
        if (xQueueReceive(xQueue, &receivedValue, pdMS_TO_TICKS(500)) == pdPASS)
        {
            printf("Receiver: Got value %d from queue.\n", receivedValue);
        }
        else
        {
            printf("Receiver: Timeout waiting for data.\n");
        }
    }
}

int main(void)
{
    // Create a queue that holds 5 integers
    xQueue = xQueueCreate(5, sizeof(int));

    if (xQueue != NULL)
    {
        // Create sender and receiver tasks
        xTaskCreate(vSenderTask, "Sender", 1000, NULL, 1, NULL);
        xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 1, NULL);

        // Start scheduler
        vTaskStartScheduler();
    }
    else
    {
        printf("Failed to create queue.\n");
    }

    return 0;
}
