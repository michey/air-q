
#include "bme280.h"

static const char *_BME_LOG_TAG = "bme280";

struct bme280_dev bme280;

void user_delay_ms(uint32_t ms) { vTaskDelay(ms / portTICK_PERIOD_MS); }

void bme_init() {
  int8_t rslt = BME280_OK;
  uint8_t settings_sel;

  bme280.dev_id = BME280_I2C_ADDR_PRIM;
  bme280.intf = BME280_I2C_INTF;
  bme280.read = (void *)user_i2c_read;
  bme280.write = (void *)user_i2c_write;
  bme280.delay_ms = (void *)user_delay_ms;
  ESP_LOGD(_BME_LOG_TAG, "calling bme280_init");
  rslt = bme280_init(&bme280);
  ESP_LOGD(_BME_LOG_TAG, "bme280 init result %d", rslt);

  bme280.settings.osr_h = BME280_OVERSAMPLING_1X;
  bme280.settings.osr_p = BME280_OVERSAMPLING_4X;
  bme280.settings.osr_t = BME280_OVERSAMPLING_4X;
  bme280.settings.filter = BME280_FILTER_COEFF_OFF;
  settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL |
                 BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
  rslt = bme280_set_sensor_settings(settings_sel, &bme280);
  ESP_LOGD(_BME_LOG_TAG, "bme280 settings config result %d", rslt);
}

void bme_rx_task(void *qPointer) {
  xQueueHandle log_queue = qPointer;

  esp_log_level_set(_BME_LOG_TAG, ESP_LOG_INFO);
  struct bme280_data comp_data;

  int8_t rslt;
  init_i2c();
  bme_init();
  while (1) {
    vTaskDelay(LOCAL_DELAY);

    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &bme280);
    bme280.delay_ms(40);
    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &bme280);
    if (rslt != BME280_OK) {
      ESP_LOGI(_BME_LOG_TAG, "bme280_get_sensor_data() returned %d", rslt);
    }

    double pressure = (comp_data.pressure * 0.00750062);

    cJSON *json_result = prepare_temperature(pressure, comp_data.temperature,
                                             comp_data.humidity);

    xQueueSend(log_queue, &json_result, 10);
  }
}
