
#ifndef _LED
#define _LED

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#define GPIO_LED 2
#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_LED))

void led_on();
void led_off();
void led_toggle();
void led_blink(int ms);

void init_led();
#endif