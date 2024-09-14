#include "i2c.h"

#define GPIOBEN    (1U<<1)
#define I2C1EN     (1U<<21)

/* PB8  SCL
 * PB9 SDA
 * */


void i2c2_init(void)
{
	/*Enable clock access*/
	RCC->AHB1ENR |= GPIOBEN;

	/*Set PB8 and PB9 mode to alternate function*/
	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<17);

	GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |= (1U<<19);

	/*Set PB8 and PB9 output type to open drain*/
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);

	/*Enable Pull-up resistors for PB8 and PB9*/
	GPIOB->PUPDR |= (1U<<16);
	GPIOB->PUPDR &= ~(1U<<17);

	GPIOB->PUPDR |= (1U<<18);
	GPIOB->PUPDR &= ~(1U<<19);

	/*Set PB8 and PB9 alternate function type(AF4)*/
	GPIOB->AFR[1] &= ~(1U<<0);
	GPIOB->AFR[1] &= ~(1U<<1);
	GPIOB->AFR[1] |= (1U<<2);
	GPIOB->AFR[1] &= ~(1U<<3);

	GPIOB->AFR[1] &= ~(1U<<4);
	GPIOB->AFR[1] &= ~(1U<<5);
	GPIOB->AFR[1] |= (1U<<6);
	GPIOB->AFR[1] &= ~(1U<<7);

	/*Enable clock access to I2C1*/
	RCC->APB1ENR |= I2C1EN;

	/*Enter reset mode*/
	/*Come out of reset mode*/
	/*Not found in the reference manual*/

	/*Set the peripheral clock*/
	/*Set I2C to standard mode, 100kHz clock*/
	/*Set rise time*/
	/*Enable I2C module*/
}

