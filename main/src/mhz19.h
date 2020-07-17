
#ifndef _MHZ19
#define _MHZ19

#include "cJSON.h"
#include "delay.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "logging.h"
#include "serial.h"
#include "string.h"

#define UART_NUM_MHZ UART_NUM_1
#define MHZ_TXD_PIN (GPIO_NUM_32)
#define MHZ_RXD_PIN (GPIO_NUM_33)

void mhz_tx_task();
void mhz_rx_task(void *qPointer);

#endif