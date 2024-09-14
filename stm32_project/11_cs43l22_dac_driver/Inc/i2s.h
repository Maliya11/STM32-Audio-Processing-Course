#ifndef I2S_H_
#define I2S_H_

#include "stm32f7xx.h"

void i2s1_gpio_init(void);
void i2s1_init(void);
void i2s1_init_dma(void);
void i2s1_send_data(uint16_t *data, uint16_t len);
void i2s1_send_data_dma(uint16_t *data, uint16_t len);


#endif /* I2S_H_ */
