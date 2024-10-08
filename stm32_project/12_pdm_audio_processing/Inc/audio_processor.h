#ifndef AUDIO_PROCESSOR_H_
#define AUDIO_PROCESSOR_H_

#include "pdm2pcm.h"
#include "i2s.h"


/*Enums for managing TX and RX states*/
typedef enum{
	RX_STATE_IDLE = 0,
	RX_STATE_HALF_COMPLETE,
	RX_STATE_FULL_COMPLETE,
}rx_state_t;

typedef enum{
	TX_STATE_IDLE = 0,
	TX_STATE_HALF_COMPLETE,
	TX_STATE_FULL_COMPLETE,
}tx_state_t;

void process_tx_data(void);
void process_rx_data(void);
void i2s_tx_rx_dma_start(void);


#endif /* AUDIO_PROCESSOR_H_ */
