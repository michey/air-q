
#include "mhz19.h"

static const char *_MHZ_LOG_TAG = "MHZ19";

static char ask_cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

void mhz_tx_task() {
  esp_log_level_set(_MHZ_LOG_TAG, ESP_LOG_INFO);
  while (1) {
    vTaskDelay(LOCAL_DELAY);
    sendData(_MHZ_LOG_TAG, UART_NUM_MHZ, ask_cmd, 9);
  }
}

uint8_t count_crc(uint8_t *data) {
  int8_t crc = 0;
  for (int8_t i = 1; i < 8; i++) {
    crc += data[i];
  }

  crc = 255 - crc;
  crc++;
  return crc;
}

void mhz_rx_task(void *qPointer) {
  xQueueHandle log_queue = qPointer;

  esp_log_level_set(_MHZ_LOG_TAG, ESP_LOG_INFO);
  uint8_t *data = (uint8_t *)malloc(RX_BUF_SIZE + 1);
  while (1) {
    const int rxBytes =
        uart_read_bytes(UART_NUM_MHZ, data, RX_BUF_SIZE, 10 / portTICK_RATE_MS);

    if (rxBytes > 0) {

      data[rxBytes] = 0;
      uint8_t crc = count_crc(data);
      if (data[0] == 0xFF && data[1] == 0x86 && data[8] == crc) {
        uint16_t responseHigh = (uint16_t)data[2];
        uint16_t responseLow = (uint16_t)data[3];
        uint16_t ppm = (256 * responseHigh) + responseLow;

        // ESP_LOGI(_MHZ_LOG_TAG, "Read %d ppm", ppm);

        cJSON *json_result = prepare_co2(ppm);
        xQueueSend(log_queue, &json_result, 10);
      } else {
        ESP_LOGI(_MHZ_LOG_TAG, "CRC error");
      }
    }
  }
  free(data);
}
