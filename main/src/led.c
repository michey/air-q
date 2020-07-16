#include "led.h"

int state = 0;
void led_on() {
  gpio_set_level(GPIO_LED, 1);
  state = 1;
}

void led_off() {
  gpio_set_level(GPIO_LED, 0);
  state = 0;
}

void led_toggle() {
  if (state == 1) {
    led_off();
  } else {
    led_on();
  }
}

void led_blink(int ms) {
  led_toggle();
  vTaskDelay(ms / portTICK_PERIOD_MS);
  led_toggle();
}

void init_led() {

  gpio_config_t io_conf;
  // disable interrupt
  io_conf.intr_type = GPIO_INTR_DISABLE;
  // set as output mode
  io_conf.mode = GPIO_MODE_OUTPUT;
  // bit mask of the pins that you want to set,e.g.GPIO18/19
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  // disable pull-down mode
  io_conf.pull_down_en = 0;
  // disable pull-up mode
  io_conf.pull_up_en = 0;
  // configure GPIO with the given settings
  gpio_config(&io_conf);
}