#include "stm32f7xx.h"
#include "fpu.h"

/*Module:
 * FPU,
 * UART,
 * GPIO,
 * TIMEBASE
 * */

#define GPIOAEN  (1U<<0)
#define PIN5     (1U<<5)
#define LED_PIN   PIN5

int main()
{
	/*Enable FPU*/
	fpu_enable();

	while(1)
	{

	}
}
