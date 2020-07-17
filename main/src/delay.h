
#ifndef _DELAY
#define _DELAY

#include "bme280_drv.h"
#include "cJSON.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"
#include "logging.h"
#include "string.h"

#define LOCAL_DELAY (60000 / portTICK_PERIOD_MS)
#endif