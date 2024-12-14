/*
 * formated_printf.c
 *
 *  Created on: Dec 6, 2024
 *      Author: Nalin Saxena
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
