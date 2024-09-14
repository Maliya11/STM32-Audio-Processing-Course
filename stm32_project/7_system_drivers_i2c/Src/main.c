#include <stdio.h>
#include "stm32f7xx.h"
#include "fpu.h"
#include "uart.h"
#include "timebase.h"

/*Module:
 * FPU,
 * UART,
 * GPIO (BSP - Board support package),
 * TIMEBASE
 * ADC
 * */

#define GPIOAEN  (1U<<0)
#define PIN5     (1U<<5)
#define LED_PIN   PIN5

int main()
{
	/*Enable FPU*/
	fpu_enable();

	/*Initialize debug UART*/
	debug_uart_init();

	/*Initialize time base*/
	timebase_init();

	while(1)
	{
		printf("Hello\n\r");
		delay(1000);
	}
}
