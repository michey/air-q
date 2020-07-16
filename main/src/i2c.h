
#ifndef _I2C_R
#define _I2C_R

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_system.h"
#include "string.h"

esp_err_t init_i2c();

int8_t user_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data,
                      uint8_t cnt);

int8_t user_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data,
                     uint8_t cnt);
#endif