#include "timebase.h"
#include "stm32f7xx.h"

#define CTRL_ENABLE      (1U<<0)
#define CTRL_TICKINT     (1U<<1)
#define CTRL_CLCKSRC     (1U<<2)
#define CTRL_COUNTFLAG   (1U<<16)

#define ONE_SEC_LOAD      168000

#define MAX_DELAY         0xFFFFFFFF
#define TICK_FREQ         1

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;

/*Delay in mili-seconds*/
void delay(uint32_t delay)
{
	uint32_t tickstart = get_tick();
	uint32_t wait = delay;

	if(wait < MAX_DELAY)
	{
		wait += (uint32_t)TICK_FREQ;
	}

	while((get_tick() - tickstart) < wait){}
}

uint32_t get_tick(void)
{
	__disable_irq();
	g_curr_tick_p = g_curr_tick;
	__enable_irq();

	return g_curr_tick_p;
}

void tick_increment(void)
{
	g_curr_tick += TICK_FREQ;
}

void timebase_init(void)
{
	/*Disable global interrupt*/
	__disable_irq();

	/*Load the timer with number of clock cycles per second*/
	SysTick->LOAD = ONE_SEC_LOAD - 1;

	/*Clear Systick current value register*/
	SysTick->VAL = 0;

	/*Select internal clock source*/
	SysTick->CTRL = CTRL_CLCKSRC;

	/*Enable interrupt*/
	SysTick->CTRL |= CTRL_TICKINT;

	/*Enable global interrupt*/
	__enable_irq();
}

void SysTick_Handler(void)
{
	tick_increment();
}

