
#ifndef _LOGGING
#define _LOGGING

#include "bme280_drv.h"
#include "cJSON.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "led.h"
#include "mqtt.h"
#include "string.h"

cJSON *prepare_temperature(double pressure, double temperature,
                           double humidity);

cJSON *prepare_dust(double thin_dust, double thick_dust);

cJSON *prepare_co2(double measure);

void poll_log_queue(void *qPointer);

#endif