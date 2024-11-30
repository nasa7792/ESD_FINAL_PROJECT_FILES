#ifndef DELAY_H_
#define DELAY_H_

#include "stdint.h"
void delay_init();
uint64_t millis();
void delay(uint32_t time);
int isTimeout(uint32_t start,uint32_t timeout_val);
#endif /* DELAY_H_ */
