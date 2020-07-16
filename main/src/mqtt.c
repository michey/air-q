#include "mqtt.h"

static const char *_MQTT_TAG = "MQTT_EXAMPLE";
static const char *CONFIG_BROKER_URL = "mqtt://stats.home.fidonode.me:1883";
static const char *TOPIC = "/topic/sensors";

esp_mqtt_client_handle_t client = NULL;
uint8_t base_mac_addr[6] = {0};

char *full_topic_name = NULL;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {
  client = event->client;
  // your_context_t *context = event->context;
  switch (event->event_id) {

  case MQTT_EVENT_CONNECTED:
    ESP_LOGI(_MQTT_TAG, "MQTT_EVENT_CONNECTED");
    break;
  //   msg_id =
  //       esp_mqtt_client_publish(client, "/topic/sensors", "data_3", 0, 1, 0);
  //   ESP_LOGI(_MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);

  //   msg_id = esp_mqtt_client_subscribe(client, "/topic/sensors", 0);
  //   ESP_LOGI(_MQTT_TAG, "sent subscribe successful, msg_id=%d", msg_id);

  //   msg_id =
  //       esp_mqtt_client_publish(client, "/topic/sensors", "data_3", 0, 1, 0);
  //   ESP_LOGI(_MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);

  //   msg_id =
  //       esp_mqtt_client_publish(client, "/topic/sensors", "data_2", 0, 1, 0);
  //   ESP_LOGI(_MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);

  //   msg_id =
  //       esp_mqtt_client_publish(client, "/topic/sensors", "data_1", 0, 1, 0);
  //   ESP_LOGI(_MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);

  //   msg_id = esp_mqtt_client_subscribe(client, "/topic/sensors", 1);
  //   ESP_LOGI(_MQTT_TAG, "sent subscribe successful, msg_id=%d", msg_id);

  //   break;
  // case MQTT_EVENT_DISCONNECTED:
  //   ESP_LOGI(_MQTT_TAG, "MQTT_EVENT_DISCONNECTED");
  //   break;

  // case MQTT_EVENT_SUBSCRIBED:
  //   ESP_LOGI(_MQTT_TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
  //   msg_id = esp_mqtt_client_publish(client, "/topic/sensors", "data", 0, 0,
  //   0); ESP_LOGI(_MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);
  //   break;
  // case MQTT_EVENT_UNSUBSCRIBED:
  //   ESP_LOGI(_MQTT_TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
  //   break;
  // case MQTT_EVENT_PUBLISHED:
  //   ESP_LOGI(_MQTT_TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
  //   break;
  // case MQTT_EVENT_DATA:
  //   ESP_LOGI(_MQTT_TAG, "MQTT_EVENT_DATA");
  //   printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
  //   printf("DATA=%.*s\r\n", event->data_len, event->data);
  //   break;
  case MQTT_EVENT_ERROR:
    ESP_LOGI(_MQTT_TAG, "MQTT_EVENT_ERROR");
    break;
  default:
    ESP_LOGI(_MQTT_TAG, "Other event id:%d", event->event_id);
    break;
  }
  return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data) {
  ESP_LOGD(_MQTT_TAG, "Event dispatched from event loop base=%s, event_id=%d",
           base, event_id);
  mqtt_event_handler_cb(event_data);
}

static void mqtt_app_start(void) {
  esp_mqtt_client_config_t mqtt_cfg = {
      .uri = CONFIG_BROKER_URL,
  };

  esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler,
                                 client);
  esp_mqtt_client_start(client);
}

void init_mqtt() {

  int ret = esp_base_mac_addr_get(base_mac_addr);
  if (ret != ESP_OK) {
    ESP_LOGE(_MQTT_TAG, "Failed to get base MAC address from EFUSE BLK3. (%s)",
             esp_err_to_name(ret));
  } else {
    ESP_LOGI(_MQTT_TAG, "Base MAC Address read from EFUSE BLK3 %s",
             base_mac_addr);
  }

  full_topic_name = malloc(sizeof(char) * (strlen(TOPIC) + 12 + 2));
  sprintf(full_topic_name, "%s/%2x%2x%2x%2x%2x%2x", TOPIC, base_mac_addr[0],
          base_mac_addr[1], base_mac_addr[2], base_mac_addr[3],
          base_mac_addr[4], base_mac_addr[5]);

  ESP_LOGI(_MQTT_TAG, "full_topic_name '%s'", full_topic_name);

  esp_log_level_set("*", ESP_LOG_INFO);
  esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
  esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
  esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
  esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
  esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
  esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

  // ESP_ERROR_CHECK(esp_event_loop_create_default());
  mqtt_app_start();
}

void send_event(char *rendered) {
  if (client != NULL) {
    int msg_id =
        esp_mqtt_client_publish(client, full_topic_name, rendered, 0, 1, 0);
    ESP_LOGI(_MQTT_TAG, "sent publish successful, msg_id=%d", msg_id);
  }
}