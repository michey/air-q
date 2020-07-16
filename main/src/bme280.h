
#ifndef _BME280
#define _BME280

#include "bme280_drv.h"
#include "cJSON.h"
#include "delay.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"
#include "logging.h"
#include "string.h"

void bme_rx_task(void *qPointer);

#endif