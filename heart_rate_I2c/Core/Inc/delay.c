#include "delay.h"
#include "stm32f4xx.h"

#define	CTRL_ENABLE					(1U<<0)
#define CTRL_CLKSRC					(1U<<2)
#define CTRL_COUNTFLAG				(1U<<16)
#define CTRL_TICKINT				(1U<<1)
#define SYSTICK_LOAD 12000 - 1

volatile uint64_t mil;

void delay_init(){

	SysTick->LOAD = SYSTICK_LOAD; //20 ms each tick
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

}



uint64_t millis()
	{
	__disable_irq();
	uint64_t ml=mil;
	__enable_irq();
	return ml;
	}



void delay(uint32_t time)
	{

	uint64_t start=millis();
	while((millis() - start) < time);

	}

void SysTick_Handler(void)
{
	mil++;
}
