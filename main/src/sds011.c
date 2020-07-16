
#include "sds011.h"

static const char *_SDS_LOG_TAG = "SDS011";

static char set_period[19] = {0xAA, 0xB4, 0x08, 0x01, 0x01, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0xFF, 0xFF, 0x08, 0xAB};

void sds_init() {
  esp_log_level_set(_SDS_LOG_TAG, ESP_LOG_INFO);
  // vTaskDelay(15000 / portTICK_PERIOD_MS);
  // sendData(_SDS_LOG_TAG, UART_NUM_SDS, set_id, 19);
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  sendData(_SDS_LOG_TAG, UART_NUM_SDS, set_period, 19);
}

void sds_rx_task(void *qPointer) {
  xQueueHandle log_queue = qPointer;

  esp_log_level_set(_SDS_LOG_TAG, ESP_LOG_INFO);
  sds_init();
  uint8_t *data = (uint8_t *)malloc(RX_BUF_SIZE + 1);
  while (1) {
    const int rxBytes =
        uart_read_bytes(UART_NUM_SDS, data, RX_BUF_SIZE, 10 / portTICK_RATE_MS);

    if (rxBytes > 0) {
      data[rxBytes] = 0;

      // ESP_LOG_BUFFER_HEXDUMP(_SDS_LOG_TAG, data, rxBytes, ESP_LOG_INFO);
      // ESP_LOGI(_SDS_LOG_TAG, "read %d bytes from %d", rxBytes, UART_NUM_SDS);
      uint8_t cmd = data[1];
      switch (cmd) {
      case 0xC5:
        // ESP_LOGI(_SDS_LOG_TAG, "c5 response");
        // ESP_LOG_BUFFER_HEXDUMP(_SDS_LOG_TAG, data, rxBytes, ESP_LOG_INFO);
        break;

      case 0xC0: {
        uint8_t thinDustHigh = (uint8_t)data[3];
        uint8_t thinDustLow = (uint8_t)data[2];
        double thinDust = (double)(thinDustLow | (thinDustHigh << 8)) / 10;
        uint8_t thickDustHigh = (uint8_t)data[5];
        uint8_t thickDustLow = (uint8_t)data[4];
        double thickDust = (double)(thickDustLow | (thickDustHigh << 8)) / 10;
        // ESP_LOGI(_SDS_LOG_TAG, "PM2.5 %.1f, PM10 %.1f", thinDust, thickDust);

        cJSON *json_result = prepare_dust(thinDust, thickDust);
        xQueueSend(log_queue, &json_result, 10);
        break;
      }

      default:
        ESP_LOGI(_SDS_LOG_TAG, "undandled response");
        ESP_LOG_BUFFER_HEXDUMP(_SDS_LOG_TAG, data, rxBytes, ESP_LOG_INFO);
        break;
      }
    }
  }
  free(data);
}