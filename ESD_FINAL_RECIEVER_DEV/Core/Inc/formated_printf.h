/*
 * formated_printf.h
 *
 *  Created on: Dec 6, 2024
 *      Author: Asus
 */

#ifndef INC_FORMATED_PRINTF_H_
#define INC_FORMATED_PRINTF_H_

#include <stdio.h>
#include <stdint.h>

void print_success(char *str);
void print_info(char *str);
void print_error(char *str);
void print(uint8_t data[], uint32_t len);


#endif /* INC_FORMATED_PRINTF_H_ */