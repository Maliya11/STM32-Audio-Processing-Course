#include "audio_processor.h"


extern I2S_HandleTypeDef hi2s1;
extern I2S_HandleTypeDef hi2s2;


//Buffer sizes
#define BUFFER_SIZE        128
#define FIFO_SIZE          256
#define TEMP_BUFFER_SIZE   16


//Buffers
static uint16_t tx_buffer[BUFFER_SIZE];
static uint16_t rx_buffer[BUFFER_SIZE];
static uint16_t pcm_temp_buffer[TEMP_BUFFER_SIZE];
static uint16_t fifo_buffer[FIFO_SIZE];

static uint8_t fifo_write_ptr = 0;
static uint8_t fifo_read_ptr = 0;
static uint8_t fifo_read_enabled = 0;


static volatile rx_state_t rx_state = RX_STATE_IDLE;
static volatile tx_state_t tx_state = TX_STATE_IDLE;

void i2s_tx_rx_dma_start(void)
{
	MX_PDM2PCM_Init();
	HAL_I2S_Transmit_DMA(&hi2s1,tx_buffer,BUFFER_SIZE/2);
	HAL_I2S_Receive_DMA(&hi2s2,rx_buffer,BUFFER_SIZE/2);
}


void fifo_write(uint16_t data)
{
	fifo_buffer[fifo_write_ptr++] = data;
}

uint16_t fifo_read(void)
{
	return fifo_buffer[fifo_read_ptr++];
}

void process_tx_data(void)
{
	switch(tx_state)
	{
	case TX_STATE_IDLE:
		//No operation required
		break;

	case TX_STATE_HALF_COMPLETE:
		//Process first half
		if(fifo_read_enabled == 1)
		{
			for(int i=0; i < 64; i += 4)
			{
				uint16_t data = fifo_read();
				tx_buffer[i] = data;
				tx_buffer[i+2] = data;
			}
		}
		tx_state = TX_STATE_IDLE;

		break;

	case TX_STATE_FULL_COMPLETE:
		//Process second half
		if(fifo_read_enabled == 1)
		{
			for(int i=64; i < 128; i += 4)
			{
				uint16_t data = fifo_read();
				tx_buffer[i] = data;
				tx_buffer[i+2] = data;
			}
		}
		tx_state = TX_STATE_IDLE;

		break;

	default:
		break;
	}
}

void process_rx_data(void)
{
	switch(rx_state)
	{
	case RX_STATE_IDLE:
		//No operation required
		break;

	case RX_STATE_HALF_COMPLETE:
		//Process first half of PDM data
		PDM_Filter(&rx_buffer[0],pcm_temp_buffer,&PDM1_filter_handler);

		//Write PCM data to fifo
		for(int i=0;i<16;i++)
		{
			fifo_write(pcm_temp_buffer[i]);
		}

		if(fifo_write_ptr - fifo_read_ptr > 128)
		{
			fifo_read_enabled = 1;
		}

		rx_state = RX_STATE_IDLE;

		break;

	case RX_STATE_FULL_COMPLETE:
		//Process second half of PDM data
		PDM_Filter(&rx_buffer[64],pcm_temp_buffer,&PDM1_filter_handler);
		//Write PCM data to fifo
		for(int i=16;i<32;i++)
		{
			fifo_write(pcm_temp_buffer[i]);
		}

//		if(fifo_write_ptr - fifo_read_ptr > 128)
//		{
//			fifo_read_enabled = 1;
//		}

		rx_state = RX_STATE_IDLE;

		break;

	default:
		break;
	}
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	tx_state = TX_STATE_HALF_COMPLETE;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	tx_state = TX_STATE_FULL_COMPLETE;
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	rx_state = RX_STATE_HALF_COMPLETE;
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	rx_state = RX_STATE_FULL_COMPLETE;
}
