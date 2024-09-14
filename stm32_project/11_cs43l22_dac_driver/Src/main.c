#include <stdio.h>
#include "stm32f7xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "bsp.h"
#include "adc.h"
#include "i2s.h"
#include "clock.h"

uint16_t tx_data[5] = {0x1111,0x2222,0x3333,0x4444,0x5555};
volatile uint8_t i2s_cmplt_flg = 0;

int main()
{
	clock_init_168Mhz();

	/*Initialize debug UART*/
	debug_uart_init();

	/*Initialize time base*/
	timebase_init();
	i2s1_gpio_init();
	i2s1_init_dma();

	while(1)
	{
		i2s1_send_data_dma(tx_data, 5);

		while(i2s_cmplt_flg==0){}

		i2s_cmplt_flg = 0;

		delay(10);
	}
}

static void spi1_dma_tx_cmplt_callback(void)
{
	i2s_cmplt_flg = 1;
}

void SPI1_IRQHandler(void)
{
	if((SPI1->SR & SPI_SR_TXE)&&((SPI1->SR & SPI_SR_BSY)==0))
	{
		spi1_dma_tx_cmplt_callback();

		/*Clear the TXEIE*/
		SPI1->CR2 &= ~SPI_CR2_TXEIE;
	}
}

void DMA2_Stream3_IRQHandler(void)
{
	/*Check interrupt source of DMA2 stream*/
	if((DMA2->LISR & DMA_LISR_TCIF3)==DMA_LISR_TCIF3)
	{
		/*Enable SPI Tx buffer empty interrupt*/
		SPI1->CR2|=SPI_CR2_TXEIE;

		/*Clear pending flag*/
		DMA2->LIFCR=DMA_LIFCR_CTCIF3;
	}
}
