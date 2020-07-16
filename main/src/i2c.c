#include "i2c.h"

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define I2C_MASTER_SCL_IO                                                      \
  (GPIO_NUM_22) /*!< gpio number for I2C master clock                          \
                 */
#define I2C_MASTER_SDA_IO                                                      \
  (GPIO_NUM_21) /*!< gpio number for I2C master data                           \
                 */
#define I2C_MASTER_NUM                                                         \
  I2C_NUM_0                         /*!< I2C port number for master dev        \
                                     */
#define I2C_MASTER_FREQ_HZ 20000    /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */

esp_err_t init_i2c() {
  int i2c_master_port = I2C_MASTER_NUM;
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = I2C_MASTER_SDA_IO;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = I2C_MASTER_SCL_IO;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
  i2c_param_config(i2c_master_port, &conf);
  return i2c_driver_install(i2c_master_port, conf.mode,
                            I2C_MASTER_RX_BUF_DISABLE,
                            I2C_MASTER_TX_BUF_DISABLE, 0);
}

#define SUCCESS 0
#define FAIL -1
#define I2C_MASTER_ACK 0
#define I2C_MASTER_NACK 1

int8_t user_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data,
                      uint8_t cnt) {
  int32_t iError = 0;

  esp_err_t espRc;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);

  i2c_master_write_byte(cmd, reg_addr, true);
  i2c_master_write(cmd, reg_data, cnt, true);
  i2c_master_stop(cmd);

  espRc = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 200 / portTICK_PERIOD_MS);
  //  printf("in user_i2c_read, i2c_master_cmd_begin returns %d\r\n", espRc);
  if (espRc == ESP_OK) {
    iError = SUCCESS;
  } else {
    iError = FAIL;
  }
  i2c_cmd_link_delete(cmd);

  return (int8_t)iError;
}

int8_t user_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data,
                     uint8_t cnt) {
  int32_t iError = 0;
  esp_err_t espRc;

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, reg_addr, true);

  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, true);

  if (cnt > 1) {
    i2c_master_read(cmd, reg_data, cnt - 1, I2C_MASTER_ACK);
  }
  i2c_master_read_byte(cmd, reg_data + cnt - 1, I2C_MASTER_NACK);
  i2c_master_stop(cmd);

  espRc = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 200 / portTICK_PERIOD_MS);
  if (espRc == ESP_OK) {
    iError = SUCCESS;
  } else {
    iError = FAIL;
  }

  i2c_cmd_link_delete(cmd);

  return (int8_t)iError;
}
