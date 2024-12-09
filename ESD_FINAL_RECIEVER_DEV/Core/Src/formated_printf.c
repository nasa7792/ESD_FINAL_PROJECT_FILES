/*
 * formated_printf.c
 *
 *  Created on: Dec 6, 2024
 *      Author: Asus
 */


#include"formated_printf.h"


void print_success(char * str){
    printf("\033[32m%s\033[0m\n \r", str);
}


void print_info(char * str){
    printf("\033[33m%s\033[0m\n\r", str);
}

void print_error(char * str){
    printf("\033[31m%s\033[0m\n \r", str);
}

void print(uint8_t data[], uint32_t len)
{
	int i=0;
	while(len--){
		putchar(data[i]);
		i++;
	}
	printf("\n\r");
}
