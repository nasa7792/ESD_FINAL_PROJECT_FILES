/*
 * usart.h
 *
 *  Created on: Oct 30, 2024
 *      Author: Nalin Saxena
 *
 *  file contains function definitions for usart
 *  usart get and put and usart init
 */

#include "usart.h"

void usart_init() {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Enable USART2 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable GPIOA clock

    // Set PA2 and PA3 as alternate function
    GPIOA->MODER |= (ALTERNATE_MODE << GPIO_MODER_MODER2_Pos);
    GPIOA->MODER |= (ALTERNATE_MODE << GPIO_MODER_MODER3_Pos);

    // Set PA2 to USART_TX and PA3 to USART_RX
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos); // TX alternate function
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL3_Pos); // RX alternate function

    // Enable USART, transmitter, and receiver
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

    // Set baud rate to 9600 (make sure BAUD_9600 is defined based on your clock setup)
    USART2->BRR = BAUD_9600;

    // Test character output to verify USART setup
}

void usart_out(char ch) {
    while (!(USART2->SR & USART_SR_TXE)) {
        // Wait until transmit buffer is empty
    }
    USART2->DR = ch;
}

void uart_string_out(char *string) {
    while (*string) {
        usart_out(*string++);
    }
}

char usart_get() {
    while (!(USART2->SR & USART_SR_RXNE)) {
        // Wait until receive buffer is not empty
    }
    return USART2->DR;
}

// Override weak symbols for printf and getchar compatibility
int __io_putchar(int ch) {
    usart_out((char)ch); // Send character using USART
    return ch;           // Return the character for compatibility
}

int __io_getchar(void) {
    return usart_get();  // Receive character using USART
}
