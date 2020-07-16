
#ifndef _SDS011
#define _SDS011

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "serial.h"
#include "string.h"
#include "cJSON.h"
#include "logging.h"

#define UART_NUM_SDS UART_NUM_2
#define SDS_TXD_PIN (GPIO_NUM_17)
#define SDS_RXD_PIN (GPIO_NUM_16)

void sds_rx_task(void *qPointer);

#endif