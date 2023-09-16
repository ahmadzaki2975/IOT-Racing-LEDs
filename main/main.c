#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "rom/gpio.h"

#define LED1 13
#define LED2 12
#define LED3 14
#define LED4 27

void BlinkLED1(void *pvParameters)
{
  (void)pvParameters;
  gpio_pad_select_gpio(LED1);
  gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
  while (1)
  {
    gpio_set_level(LED1, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(LED1, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void BlinkLED2(void *pvParameters)
{
  (void)pvParameters;
  gpio_pad_select_gpio(LED2);
  gpio_set_direction(LED2, GPIO_MODE_OUTPUT);
  while (1)
  {
    gpio_set_level(LED2, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
    gpio_set_level(LED2, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
  }
}

void BlinkLED3(void *pvParameters)
{
  (void)pvParameters;
  gpio_pad_select_gpio(LED3);
  gpio_set_direction(LED3, GPIO_MODE_OUTPUT);
  while (1)
  {
    gpio_set_level(LED3, 0);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    gpio_set_level(LED3, 1);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void BlinkLED4(void *pvParameters)
{
  (void)pvParameters;
  gpio_pad_select_gpio(LED4);
  gpio_set_direction(LED4, GPIO_MODE_OUTPUT);
  while (1)
  {
    gpio_set_level(LED4, 0);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    gpio_set_level(LED4, 1);
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
  xTaskCreate(&BlinkLED1, "BlinkLED1", 2048, NULL, 5, NULL);
  xTaskCreate(&BlinkLED2, "BlinkLED2", 2048, NULL, 5, NULL);
  xTaskCreate(&BlinkLED3, "BlinkLED3", 2048, NULL, 5, NULL);
  xTaskCreate(&BlinkLED4, "BlinkLED4", 2048, NULL, 5, NULL);
}