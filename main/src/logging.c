#include "logging.h"

static const char *_GENERAL_LOG_TAG = "logging";

static const char *_CO = "co2";
static const char *_PRESSURE = "pressure";
static const char *_DUST = "dust";

cJSON *prepare_temperature(double pressure, double temperature,
                           double humidity) {
  cJSON *root;
  root = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "type", cJSON_CreateString(_PRESSURE));
  cJSON_AddNumberToObject(root, "pressure", pressure);
  cJSON_AddNumberToObject(root, "temperature", temperature);
  cJSON_AddNumberToObject(root, "humidity", humidity);
  return root;
}

cJSON *prepare_dust(double thin_dust, double thick_dust) {
  cJSON *root;
  root = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "type", cJSON_CreateString(_DUST));
  cJSON_AddNumberToObject(root, "pm25", thin_dust);
  cJSON_AddNumberToObject(root, "pm10", thick_dust);
  return root;
}

cJSON *prepare_co2(double measure) {
  cJSON *root;
  root = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "type", cJSON_CreateString(_CO));
  cJSON_AddNumberToObject(root, "co2", measure);
  return root;
}

void poll_log_queue(void *qPointer) {
  xQueueHandle log_queue = qPointer;
  cJSON *fp = NULL;
  while (1) {
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    if (xQueueReceive(log_queue, &fp, 0)) {
      char *rendered = cJSON_PrintUnformatted(fp);
      ESP_LOGI(_GENERAL_LOG_TAG, "%s", rendered);
      send_event(rendered);
      free(fp);
      free(rendered);
      led_blink(500);
    }
    taskYIELD();
  }
}