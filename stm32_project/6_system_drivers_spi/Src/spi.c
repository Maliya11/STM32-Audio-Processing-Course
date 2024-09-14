#include "spi.h"

#define GPIOAEN   (1U<<0)
#define SPI1EN    (1U<<12)

void spi1_gpio_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA5, PA6, PA7 mode to alternate function mode*/
	/*PA5*/
	GPIOA->MODER &= ~(1U<<10);
	GPIOA->MODER |= (1U<<11);

	/*PA6*/
	GPIOA->MODER &= ~(1U<<12);
	GPIOA->MODER |= (1U<<13);

	/*PA7*/
	GPIOA->MODER &= ~(1U<<14);
	GPIOA->MODER |= (1U<<15);

	/*Set PA5, PA6, PA7 alternate function type to SPI1*/
	/*PA5*/
	GPIOA->AFR[0] |= (1U<<20);
	GPIOA->AFR[0] &= ~(1U<<21);
	GPIOA->AFR[0] |= (1U<<22);
	GPIOA->AFR[0] &= ~(1U<<23);

	/*PA6*/
	GPIOA->AFR[0] |= (1U<<24);
	GPIOA->AFR[0] &= ~(1U<<25);
	GPIOA->AFR[0] |= (1U<<26);
	GPIOA->AFR[0] &= ~(1U<<27);

	/*PA7*/
	GPIOA->AFR[0] |= (1U<<28);
	GPIOA->AFR[0] &= ~(1U<<29);
	GPIOA->AFR[0] |= (1U<<30);
	GPIOA->AFR[0] &= ~(1U<<21);

	/*Enable clock access for the SPI1*/
	RCC->APB2ENR |= SPI1EN;
}

StatusTypeDef spi_init(SPI_HandleTypeDef *hspi)
{
	/*Check SPI handle allocation*/
	if(hspi == NULL)
	{
		return DEV_ERROR;
	}

	/*Disable CRC calculation*/
	hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

	if(hspi->State == SPI_STATE_RESET)
	{
		spi1_gpio_init();
	}

	hspi->State = SPI_STATE_BUSY;

	/*Disable the SPI peripheral before configuring*/
	CLEAR_BIT(hspi->Instance->CR1, SPI_CR1_SPE);

	/*Configuring the CR1 register*/
	WRITE_REG(hspi->Instance->CR1, ((hspi->Init.Mode & (SPI_CR1_MSTR | SPI_CR1_SSI)) |
			(hspi->Init.Direction & (SPI_CR1_RXONLY | SPI_CR1_BIDIMODE)) |
			(hspi->Init.DataSize & SPI_CR1_CRCL) |
			(hspi->Init.CLKPolarity & SPI_CR1_CPOL) |
			(hspi->Init.CLKPhase & SPI_CR1_CPHA) |
			(hspi->Init.NSS & SPI_CR1_SSM) |
			(hspi->Init.BaudRatePrescalar & SPI_CR1_BR_Msk) |
			(hspi->Init.FirstBit & SPI_CR1_LSBFIRST) |
			(hspi->Init.CRCCalculation & SPI_CR1_CRCEN)));

	/*Configure the CR2 register*/
	WRITE_REG(hspi->Instance->CR2, (((hspi->Init.NSS >> 16U) & SPI_CR2_SSOE)|(hspi->Init.TIMode & SPI_CR2_FRF)));

	hspi->ErrorCode = SPI_ERROR_NONE;
	hspi->State = SPI_STATE_READY;
	return DEV_OK;
}

StatusTypeDef spi_transmit(SPI_HandleTypeDef *hspi, uint8_t *p_data, uint16_t size, uint32_t timeout)
{
	uint32_t tickstart;
	StatusTypeDef error_code = DEV_OK;
	uint16_t tx_xfer_cnt;

	tx_xfer_cnt = size;
	tickstart = get_tick();

	if(hspi->State != SPI_STATE_READY)
	{
		error_code = DEV_BUSY;
		hspi->State = SPI_STATE_READY;
		return error_code;
	}

	if((p_data == NULL) || (size == 0))
	{
		error_code = DEV_ERROR;
		hspi->State = SPI_STATE_READY;
		return error_code;
	}

	/*Set the transaction information*/
	hspi->State = SPI_STATE_BUST_TX;
	hspi->ErrorCode = SPI_ERROR_NONE;
	hspi->pTxBuffPtr = (uint8_t *)p_data;
	hspi->TxXferSize = size;
	hspi->TxXferCount = size;

	hspi->pRxBuffPtr = (uint8_t *)NULL;
	hspi->RxXferSize = 0;
	hspi->RxXferCount = 0;

	/*Configure communication direction*/
	if(hspi->Init.Direction == SPI_DIRECTION_1LINE)
	{
		CLEAR_BIT(hspi->Instance->CR1,SPI_CR1_SPE);

		/*Set 1 line TX*/
		SET_BIT(hspi->Instance->CR1, SPI_CR1_BIDIOE);
	}

	if((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
	{
		SET_BIT(hspi->Instance->CR1, SPI_CR1_SPE);
	}

	/*Transmit data in 16 bit mode*/
	if(hspi->Init.DataSize == SPI_DATASIZE_16BIT)
	{
		if((hspi->Init.Mode == SPI_MODE_SLAVE) || (tx_xfer_cnt == 1))
		{
			hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);
			hspi->pTxBuffPtr += sizeof(uint16_t);
			hspi->TxXferCount--;
		}

		while(hspi->TxXferCount > 0)
		{
			/*Check if TXE flag to be set and send data*/
			if(hspi->Instance->SR & (SPI_FLAG_TXE))
			{
				hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);
				hspi->pTxBuffPtr += sizeof(uint16_t);
				hspi->TxXferCount--;
			}
			else
			{
				if((((get_tick()-tickstart) >= timeout)&&(timeout != MAX_DELAY)) || timeout == 0)
				{
					error_code = DEV_TIMEOUT;
					hspi->State = SPI_STATE_READY;
					return error_code;
				}
			}
		}
	}

	/*Transmit data in 8 bit mode*/
	else
	{
		if((hspi->Init.Mode == SPI_MODE_SLAVE) || (tx_xfer_cnt == 1))
		{
			*((__IO uint8_t *)&hspi->Instance->DR) = (* hspi->pTxBuffPtr);
			hspi->pTxBuffPtr += sizeof(uint8_t);
			hspi->TxXferCount--;
		}

		while(hspi->TxXferCount > 0)
		{
			/*Check if TXE flag to be set and send data*/
			if(hspi->Instance->SR & (SPI_FLAG_TXE))
			{
				*((__IO uint8_t *)&hspi->Instance->DR) = (* hspi->pTxBuffPtr);
				hspi->pTxBuffPtr += sizeof(uint8_t);
				hspi->TxXferCount--;
			}
			else
			{
				if((((get_tick()-tickstart) >= timeout)&&(timeout != MAX_DELAY)) || timeout == 0)
				{
					error_code = DEV_TIMEOUT;
					hspi->State = SPI_STATE_READY;
					return error_code;
				}
			}
		}
	}

	if(hspi->ErrorCode != SPI_ERROR_NONE)
	{
		error_code = DEV_ERROR;
	}

	hspi->State = SPI_STATE_READY;
	return error_code;
}


StatusTypeDef spi_receive(SPI_HandleTypeDef *hspi, uint8_t *p_data, uint16_t size, uint32_t timeout)
{
	uint32_t tickstart;
	StatusTypeDef error_code = DEV_OK;
	//uint16_t rx_xfer_cnt;

	tickstart = get_tick();
	//rx_xfer_cnt = size;

	if(hspi->State != SPI_STATE_READY)
	{
		error_code = DEV_BUSY;
		hspi->State = SPI_STATE_READY;
		return error_code;
	}

	if((p_data == NULL) || (size == 0))
	{
		error_code = DEV_ERROR;
		hspi->State = SPI_STATE_READY;
		return error_code;
	}

	/*Set the transaction information*/
	hspi->State = SPI_STATE_BUST_RX;
	hspi->ErrorCode = SPI_ERROR_NONE;
	hspi->pRxBuffPtr = (uint8_t *)p_data;
	hspi->RxXferCount = size;
	hspi->RxXferSize = size;

	hspi->pTxBuffPtr = (uint8_t *)NULL;
	hspi->TxXferCount = 0;
	hspi->TxXferSize = 0;

	if((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
	{
		SET_BIT(hspi->Instance->CR1, SPI_CR1_SPE);
	}

	/*Receive data in 8 bit mode*/
	if(hspi->Init.DataSize == SPI_DATASIZE_8BIT)
	{
		while(hspi->RxXferCount > 0)
		{
			if(hspi->Instance->SR & SPI_FLAG_RXNE)
			{
				(*(uint8_t *)hspi->pRxBuffPtr) = *(__IO uint8_t *)&hspi->Instance->DR;
				hspi->pRxBuffPtr += sizeof(uint8_t);
				hspi->RxXferCount--;
			}
			else
			{
				if((((get_tick()-tickstart) >= timeout)&&(timeout != MAX_DELAY)) || (timeout == 0))
				{
					error_code = DEV_TIMEOUT;
					hspi->State = SPI_STATE_READY;
					return error_code;
				}
			}
		}
	}
	else
	{
		while(hspi->RxXferCount > 0)
		{
			if(hspi->Instance->SR & SPI_FLAG_RXNE)
			{
				(*(uint16_t *)hspi->pRxBuffPtr) = *(__IO uint16_t *)&hspi->Instance->DR;
				hspi->pRxBuffPtr += sizeof(uint16_t);
				hspi->RxXferCount--;
			}
			else
			{
				if((((get_tick()-tickstart) >= timeout)&&(timeout != MAX_DELAY)) || (timeout == 0))
				{
					error_code = DEV_TIMEOUT;
					hspi->State = SPI_STATE_READY;
					return error_code;
				}
			}
		}
	}

	if(hspi->ErrorCode != SPI_ERROR_NONE)
	{
		error_code = DEV_ERROR;
	}

	hspi->State = SPI_STATE_READY;
	return error_code;
}
