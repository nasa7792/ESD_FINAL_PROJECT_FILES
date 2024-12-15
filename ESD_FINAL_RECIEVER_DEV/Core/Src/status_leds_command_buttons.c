/*
 * status_leds_command_buttons.c
 *
 *  Created on: Dec 8, 2024
 *      Author: Asus
 */
#include<stm32f411xe.h>
#include"utilities.h"
#include"status_leds_command_buttons.h"




void setup_enable_status_leds(){
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER8, ESF_GPIO_MODER_OUTPUT); // led for max
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER9, ESF_GPIO_MODER_OUTPUT); // led for gps
	GPIOA->BSRR |=GPS_ON_INDICATOR;
	GPIOA->BSRR |=MAX30102_ON_INDICATOR;
}


void  command_button_config(){
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC clock
	    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock

	    // Configure PC6 and PC7 as inputs with pull-up resistors
	    GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7); // Input mode for PC6 and PC7
	    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7); // Clear pull-up/down bits
	    GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0); // Enable pull-up for PC6 and PC7

	    // Configure PC6 and PC7 as sources for EXTI6 and EXTI7
	    SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI6 | SYSCFG_EXTICR2_EXTI7); // Clear bits
	    SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PC | SYSCFG_EXTICR2_EXTI7_PC); // Map PC6 and PC7

	    // Configure EXTI lines for falling edge trigger and unmask them
	    EXTI->IMR |= (EXTI_IMR_MR6 | EXTI_IMR_MR7); // Unmask EXTI6 and EXTI7
	    EXTI->FTSR |= (EXTI_FTSR_TR6 | EXTI_FTSR_TR7); // Falling edge trigger for EXTI6 and EXTI7

	    // Enable EXTI9_5 interrupt in NVIC for PC6 and PC7
	    NVIC_EnableIRQ(EXTI9_5_IRQn);
	    NVIC_SetPriority(EXTI9_5_IRQn, 2); // Set priority
}
