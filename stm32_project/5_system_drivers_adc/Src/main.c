#include <stdio.h>
#include "stm32f7xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"
#include "bsp.h"
#include "adc.h"

/*Module:
 * FPU,
 * UART,
 * GPIO (BSP - Board support package),
 * TIMEBASE
 * ADC
 * */
bool btn_state;
uint32_t sensor_value;

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

	while(1)
	{
		//led_on();
		//btn_state = get_btn_state();
		sensor_value = adc_read();
	}
}
