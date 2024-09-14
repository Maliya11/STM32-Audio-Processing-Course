#include "stm32f7xx.h"



void fpu_enable(void)
{
	/*Enable floating point unit*/
	/*Enable CP10,CP11 full access*/
	SCB->CPACR |= (1U<<20);
	SCB->CPACR |= (1U<<21);
	SCB->CPACR |= (1U<<22);
	SCB->CPACR |= (1U<<23);
}
