
#include "serial.h"

int sendData(const char *log_tag, uart_port_t uart_num, const char *data,
             const int len) {
  const int txBytes = uart_write_bytes(uart_num, data, len);
  // ESP_LOGI(log_tag, "Wrote %d bytes to %d", txBytes, uart_num);
  // ESP_LOG_BUFFER_HEXDUMP(log_tag, data, len, ESP_LOG_INFO);
  return txBytes;
}

void init_serial(uart_port_t uart_num, uint32_t baud_rate, int rx_buffer_size,
                 int tx_io_num, int rx_io_num) {
  const uart_config_t uart_config = {.baud_rate = baud_rate,
                                     .data_bits = UART_DATA_8_BITS,
                                     .parity = UART_PARITY_DISABLE,
                                     .stop_bits = UART_STOP_BITS_1,
                                     .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
                                     .source_clk = UART_SCLK_APB};

  uart_param_config(uart_num, &uart_config);
  uart_set_pin(uart_num, tx_io_num, rx_io_num, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE);
  // We won't use a buffer for sending data.
  uart_driver_install(uart_num, rx_buffer_size * 2, 0, 0, NULL, 0);
}