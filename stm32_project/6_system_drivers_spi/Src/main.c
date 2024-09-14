#include <stdio.h>
#include "stm32f7xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "bsp.h"
#include "adc.h"
#include "spi.h"

/*Module:
 * FPU,
 * UART,
 * GPIO (BSP - Board support package),
 * TIMEBASE
 * ADC
 * */
bool btn_state;
uint32_t sensor_value;

SPI_HandleTypeDef hspi1;

static void test_spi_init(void);

uint8_t tx_buffer[1] = {0xDE};
uint8_t rx_buffer[1];
int main()
{
	/*Enable FPU*/
	fpu_enable();

	/*Initialize debug UART*/
	debug_uart_init();

	/*Initialize time base*/
	timebase_init();

	/*Initialize LED*/
	led_init();

	/*Initialize push button*/
	button_init();

	/*Initialize ADC*/
	pa1_adc_init();

	/*Start conversion of ADC*/
	start_conversion();

	/*Initialize SPI*/
	test_spi_init();

	spi_transmit(&hspi1, tx_buffer, 1, 10);

	spi_receive(&hspi1, rx_buffer, 1 , 10);

	while(1)
	{

	}
}


static void test_spi_init(void)
{
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescalar = SPI_BAUDRATEPRESCALAR_4;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

	/*Initialize SPI*/
	spi_init(&hspi1);
}
