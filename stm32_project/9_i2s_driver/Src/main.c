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

int main()
{
	clock_init_168Mhz();

	/*Enable FPU*/
	fpu_enable();

	/*Initialize debug UART*/
	debug_uart_init();

	/*Initialize time base*/
	timebase_init();

	i2s1_gpio_init();
	i2s1_init();

	while(1)
	{
		i2s1_send_data(tx_data, 5);
		delay(10);
	}
}
