/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File name : delay.c
 *  --------------------------------------------------------------------------------
 * This file contains function definitions regarding systick configuration and creating 
 * delay functions
   ---------------------------------------------------------------------------------*/
#include "delay.h"
#include "stm32f4xx.h"

#define	CTRL_ENABLE					(1U<<0)
#define CTRL_CLKSRC					(1U<<2)
#define CTRL_COUNTFLAG				(1U<<16)
#define CTRL_TICKINT				(1U<<1)
#define SYSTICK_LOAD 12000 - 1

volatile uint64_t mil; //global tick


/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */
void delay_init(){

	SysTick->LOAD = SYSTICK_LOAD; 
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
	while((millis() - start) < time); //create software delay

	}

void SysTick_Handler(void)
{
	mil++; //on isr increment mil counter
}
