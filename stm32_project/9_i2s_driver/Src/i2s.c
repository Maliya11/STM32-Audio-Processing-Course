#include "i2s.h"

/*
 * PC4 : I2S1_MCK
 * PA4 : I2S1_WS
 * PA5 : I2S1_CK
 * PA7 : I2S1_SD
 * */

void i2s1_gpio_init(void)
{
	/*Enable clock access to GPIOA and GPIOC*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	/*Set PA4,PA5,PA7,PC4 to alternate function mode*/
	/*PA4*/
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |= (1U<<7);

	/*PA5*/
	GPIOA->MODER &= ~(1U<<8);
	GPIOA->MODER |= (1U<<9);

	/*PA7*/
	GPIOA->MODER &= ~(1U<<12);
	GPIOA->MODER |= (1U<<13);

	/*PC4*/
	GPIOC->MODER &= ~(1U<<6);
	GPIOC->MODER |= (1U<<7);

	/*Set alternate function type to SPI1.I2S1*/
	/*PA4*/
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] &= ~(1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	/*PA5*/
	GPIOA->AFR[0] |= (1U<<16);
	GPIOA->AFR[0] &= ~(1U<<17);
	GPIOA->AFR[0] |= (1U<<18);
	GPIOA->AFR[0] &= ~(1U<<19);

	/*PA7*/
	GPIOA->AFR[0] |= (1U<<24);
	GPIOA->AFR[0] &= ~(1U<<25);
	GPIOA->AFR[0] |= (1U<<26);
	GPIOA->AFR[0] &= ~(1U<<27);

	/*PC4*/
	GPIOC->AFR[0] |= (1U<<12);
	GPIOC->AFR[0] &= ~(1U<<13);
	GPIOC->AFR[0] |= (1U<<14);
	GPIOC->AFR[0] &= ~(1U<<15);
}

void i2s1_init(void)
{
	/*Enable clock access to SPI1/I2S1*/
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	/*Set SPI to I2S mode*/
	SPI1->I2SCFGR |= SPI_I2SCFGR_I2SMOD;

	/*Set the I2S mode to MASTER TRANSMIT*/
	SPI1->I2SCFGR |= SPI_I2SCFGR_I2SCFG_1;

	/*Set I2S linear pre-scalar*/
	SPI1->I2SPR |= (1U<<0);
	SPI1->I2SPR |= (1U<<1);

	/*Set odd factor for the pre-scalar*/
	SPI1->I2SPR |= (1U<<8);

	/*Enable Master Clock output*/
	SPI1->I2SPR |= SPI_I2SPR_MCKOE;

	/*Enable I2S*/
	SPI1->I2SCFGR |= SPI_I2SCFGR_I2SE;
}

void i2s1_send_data(uint16_t *data, uint16_t len)
{
	uint32_t i = 0;

	while(i<len)
	{
		/*Wait until TXE is set*/
		while(!(SPI1->SR & (SPI_SR_TXE))){}

		/*Write the data to the data register*/
		SPI1->DR = data[i];
		i++;
	}
	/*Wait until TXE is set*/
	while(!(SPI1->SR & (SPI_SR_TXE))){}

	/*Wait for the bust flag to reset*/
	while(!(SPI1->SR & (SPI_SR_BSY))){}

	/*Clear OVR flag (reference manual)*/
	(void) SPI1->DR;
	(void) SPI1->SR;
}

