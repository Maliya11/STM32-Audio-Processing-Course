#ifndef TIMEBASE_H_
#define TIMEBASE_H_

#include <stdint.h>

void timebase_init(void);
uint32_t get_tick(void);
void delay(uint32_t delay);

#define MAX_DELAY    0xFFFFFFFF

#endif /* TIMEBASE_H_ */
