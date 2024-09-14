#include "clock.h"
#include "fpu.h"

#define PLL_M    4
#define PLL_N    168
#define PLL_P    2
#define PLL_Q    7

#define I2SPLL_N    172
#define I2SPLL_R    2

/* APB1 Pre-scalar : 2
 * APB2 Pre-scalar : 1
 * */

void clock_init_168Mhz(void)
{
	__IO uint32_t startup_counter = 0;
	__IO uint32_t hse_status = 0;

	/*Enable HSE Oscillator*/
	RCC->CR |= RCC_CR_HSEON;

	/*Wait until HSE is ready*/
	do{
		hse_status = RCC->CR & RCC_CR_HSERDY;
		startup_counter++;
	}while((hse_status==0) && (startup_counter != 3000));

	/*Check it HSE is ready*/
	if((RCC->CR & RCC_CR_HSERDY)!= RESET)
	{
		hse_status = 1;
	}
	else
	{
		hse_status = 0;
	}

	/*Configure the system clock*/
	if(hse_status)
	{
		/*Enable power control block*/
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;

		/*Power control scale to low*/
		PWR->CR1 &= ~ PWR_CR1_VOS;

		/*Set HCLK pre-scalar*/
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

		/*Set APB1(PCLK1) pre-scalar*/
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

		/*Set APB2 (PCLK2) pre-scalar*/
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

		/*Configure the PLL*/
		RCC->PLLCFGR = PLL_M|(PLL_N << 6)|(PLL_P >> 1) << 16|RCC_PLLCFGR_PLLSRC_HSE|(PLL_Q << 24);

		/*Enable the PLL*/
		RCC->CR |= RCC_CR_PLLON;

		/*Wait until PLL is ready*/
		while((RCC->CR & RCC_CR_PLLRDY)==0){}

		/*Configure cache and wait state*/
		//FLASH->CR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

		/*Set PLL as the main clock source*/
		RCC->CFGR &= ~RCC_CFGR_SW;
		RCC->CFGR |= RCC_CFGR_SW_PLL;

		/*Wait until PLL is selected*/
		while((RCC->CFGR & RCC_CFGR_SWS)!= RCC_CFGR_SWS_PLL){}
	}
	else
	{
		/*Handle error*/
	}

	/*Enable FPU (Improve the performance of computing the PLL configuration)*/
	fpu_enable();
	/*Configure I2S clock*/
	RCC->PLLI2SCFGR = (I2SPLL_N << RCC_PLLI2SCFGR_PLLI2SN_Pos)|(I2SPLL_R << RCC_PLLI2SCFGR_PLLI2SR_Pos);

	/*Enable I2S PLL*/
	RCC->CR |= RCC_CR_PLLI2SON;

	/*Wait until I2SPLL is ready*/
	while(!(RCC->CR & RCC_CR_PLLI2SRDY)){}
}
