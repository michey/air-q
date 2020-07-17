/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your
   option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "string.h"

#include "src/bme280.h"
#include "src/bme280_drv.h"
#include "src/i2c.h"
#include "src/led.h"
#include "src/mhz19.h"
#include "src/mqtt.h"
#include "src/sds011.h"
#include "src/serial.h"
#include "src/wifi.h"

xQueueHandle log_queue = NULL;
static const char *_MAIN_TAG = "wifi station";

void app_main(void) {

  init_led();
  led_on();

  log_queue = xQueueCreate(10, sizeof(uint32_t));

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  init_serial(UART_NUM_MHZ, 9600, RX_BUF_SIZE, MHZ_TXD_PIN, MHZ_RXD_PIN);
  init_serial(UART_NUM_SDS, 9600, RX_BUF_SIZE, SDS_TXD_PIN, SDS_RXD_PIN);

  ESP_LOGI(_MAIN_TAG, "ESP_WIFI_MODE_STA");
  wifi_init_sta();
  init_mqtt();

  xTaskCreatePinnedToCore(poll_log_queue, "poll_log_queue", 1024 * 2,
                          (void *)log_queue, configMAX_PRIORITIES - 8, NULL, 1);

  xTaskCreate(sds_rx_task, "sds_rx_task", 1024 * 2, (void *)log_queue,
              configMAX_PRIORITIES - 1, NULL);

  xTaskCreate(mhz_rx_task, "mhz_rx_task", 1024 * 2, (void *)log_queue,
              configMAX_PRIORITIES - 1, NULL);
  xTaskCreate(mhz_tx_task, "mhz_tx_task", 1024 * 2, NULL,
              configMAX_PRIORITIES - 3, NULL);

  xTaskCreate(bme_rx_task, "bme_rx_task", 1024 * 2, (void *)log_queue,
              configMAX_PRIORITIES - 2, NULL);
}
