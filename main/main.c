#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "rom/gpio.h"
#include "driver/uart.h"
#include "string.h"
#include "ctype.h"

// Queue handle
QueueHandle_t uartQueue;
QueueHandle_t ledQueue;

// Definisi PIN UART
#define TX_PIN 1
#define RX_PIN 3

// Definisi PIN LED
#define LED1 13
#define LED2 12
#define LED3 14
#define LED4 27

void shiftString(char *str)
{
  int len = strlen(str);
  for (int i = 0; i < len; i++)
  {
    str[i] = str[i + 1];
  }
  str[len] = '\0';
}

void emptyString(char *str)
{
  int len = strlen(str);
  for (int i = 0; i < len; i++)
  {
    str[i] = '\0';
  }
}

// Blink untuk LED 1
void BlinkLED1(void *pvParameters)
{
  (void)pvParameters;
  gpio_pad_select_gpio(LED1);
  gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
  uint16_t period = 100;
  while (1)
  {
    char queueData[64];
    if (xQueueReceive(ledQueue, &queueData, pdMS_TO_TICKS(1000)) == pdPASS)
    {
      if (queueData[0] == '1')
      {
        shiftString(queueData);
        period = atoi(queueData);
        printf("Period of LED 1 changed to: %i\n", period);
      }
    }
    gpio_set_level(LED1, 0);
    vTaskDelay(period / portTICK_PERIOD_MS);
    gpio_set_level(LED1, 1);
    vTaskDelay(period / portTICK_PERIOD_MS);
  }
}

// Blink untuk LED 2
void BlinkLED2(void *pvParameters)
{
  (void)pvParameters;
  gpio_pad_select_gpio(LED2);
  gpio_set_direction(LED2, GPIO_MODE_OUTPUT);
  uint16_t period = 200;
  while (1)
  {
    char queueData[64];
    if (xQueueReceive(ledQueue, &queueData, pdMS_TO_TICKS(1000)) == pdPASS)
    {
      if (queueData[0] == '2')
      {
        shiftString(queueData);
        period = atoi(queueData);
        printf("Period of LED 2 changed to: %i\n", period);
      }
    }
    gpio_set_level(LED2, 0);
    vTaskDelay(period / portTICK_PERIOD_MS);
    gpio_set_level(LED2, 1);
    vTaskDelay(period / portTICK_PERIOD_MS);
  }
}

// Blink untuk LED 3
void BlinkLED3(void *pvParameters)
{
  (void)pvParameters;
  gpio_pad_select_gpio(LED3);
  gpio_set_direction(LED3, GPIO_MODE_OUTPUT);
  uint16_t period = 300;
  while (1)
  {
    char queueData[64];
    if (xQueueReceive(ledQueue, &queueData, pdMS_TO_TICKS(1000)) == pdPASS)
    {
      if (queueData[0] == '3')
      {
        shiftString(queueData);
        period = atoi(queueData);
        printf("Period of LED 3 changed to: %i\n", period);
      }
    }
    gpio_set_level(LED3, 0);
    vTaskDelay(period / portTICK_PERIOD_MS);
    gpio_set_level(LED3, 1);
    vTaskDelay(period / portTICK_PERIOD_MS);
  }
}

// Blink untuk LED 4
void BlinkLED4(void *pvParameters)
{
  (void)pvParameters;
  gpio_pad_select_gpio(LED4);
  gpio_set_direction(LED4, GPIO_MODE_OUTPUT);
  uint16_t period = 400;
  while (1)
  {
    char queueData[64];
    if (xQueueReceive(ledQueue, &queueData, pdMS_TO_TICKS(1000)) == pdPASS)
    {
      if (queueData[0] == '4')
      {
        shiftString(queueData);
        period = atoi(queueData);
        printf("Period of LED 4 changed to: %i\n", period);
      }
    }
    gpio_set_level(LED4, 0);
    vTaskDelay(period / portTICK_PERIOD_MS);
    gpio_set_level(LED4, 1);
    vTaskDelay(period / portTICK_PERIOD_MS);
  }
}

char received_message[64];
int message_index = 0;
int selected_led = 0;
// Read data dari UART
void readUART()
{
  while (1)
  {
    char data;
    uart_read_bytes(UART_NUM_0, &data, 1, portMAX_DELAY);
    printf("%c\n", data);
    if (message_index == 0)
    {
      selected_led = data;
      if (selected_led == '1')
      {
        printf("Changing period of LED 1\n");
      }
      if (selected_led == '2')
      {
        printf("Changing period of LED 2\n");
      }
      if (selected_led == '3')
      {
        printf("Changing period of LED 3\n");
      }
      if (selected_led == '4')
      {
        printf("Changing period of LED 4\n");
      }
    }
    if (data == ' ' && message_index > 0)
    {
      xQueueSend(ledQueue, received_message, portMAX_DELAY);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      selected_led = 0;
      printf("Sent queue data: %s\n", received_message);
      emptyString(received_message);
      message_index = 0;
      continue;
    }
    received_message[message_index] = data;
    message_index++;
  }
}

void app_main(void)
{
  // Konfigurasi UART
  const uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
  uart_param_config(UART_NUM_0, &uart_config);
  uart_set_pin(UART_NUM_0, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);

  // Membuat Queue untuk UART dan LED
  uartQueue = xQueueCreate(10, sizeof(char[64]));
  ledQueue = xQueueCreate(10, sizeof(char[64]));

  // Membuat Task untuk Blink LED 1 hingga 4
  xTaskCreate(&BlinkLED1, "BlinkLED1", 2048, NULL, 5, NULL);
  xTaskCreate(&BlinkLED2, "BlinkLED2", 2048, NULL, 5, NULL);
  xTaskCreate(&BlinkLED3, "BlinkLED3", 2048, NULL, 5, NULL);
  xTaskCreate(&BlinkLED4, "BlinkLED4", 2048, NULL, 5, NULL);

  // Membuat Task untuk membaca data dari UART
  xTaskCreate(&readUART, "readUART", 2048, NULL, 5, NULL);
}