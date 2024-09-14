#include <stdint.h>
#include "uart.h"

#define GPIOAEN      (1U<<0)
#define UART2EN      (1U<<17)


#define DBG_UART_BAUDRATE   115200
#define SYS_FREQ            168000000  //16MHz
#define APB1_CLK            (SYS_FREQ/2)
#define CR1_TE              (1U<<3)
#define CR1_UE              (1U<<1)
#define ISR_TXE             (1U<<7)

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart_write(int ch);

int __io_putchar(int ch)
{
	uart_write(ch);
	return ch;
}

void debug_uart_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set the mode of PA2 to alternate function mode*/
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/*Set alternate function type to AF7(USART2)*/
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	/*Enable clock access to USART2*/
	RCC->APB1ENR |= UART2EN;

	/*Configure USART baud rate*/
	uart_set_baudrate(APB1_CLK, DBG_UART_BAUDRATE);

	/*Configure transfer direction*/
	USART2->CR1 = CR1_TE;

	/*Enable USART module*/
	USART2->CR1 |= CR1_UE;
}

static void uart_write(int ch)
{
	/*Make sure transmit data register is empty*/
	while(!(USART2->ISR & ISR_TXE)){}

	/*Write to transmit data register*/
	USART2->TDR = (ch & 0xFF);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U))/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2->BRR = compute_uart_bd(periph_clk, baudrate);
}
