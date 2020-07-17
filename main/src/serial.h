
#ifndef _SERIAL_R
#define _SERIAL_R

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "string.h"

static const int RX_BUF_SIZE = 512;

int sendData(const char *log_tag, uart_port_t uart_num, const char *data,
             const int len);
void init_serial(uart_port_t uart_num, uint32_t baud_rate, int rx_buffer_size,
                 int tx_io_num, int rx_io_num);
#endif