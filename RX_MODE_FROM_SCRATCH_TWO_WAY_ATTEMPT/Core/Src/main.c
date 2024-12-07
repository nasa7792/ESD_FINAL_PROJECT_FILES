
#include "main.h"
#include "usart.h"
#include <stdio.h>
#include"utilities.h"
#include"delay.h"
#include"SPI.h"
#include"NRF_DRIVER.h"
#include<string.h>
#include"utilities.h"
#include "st7789_lcd_functions.h"

void test_configure_btn_interupt(){
    // Configure PA1 as input
    // Configure PA1 as input
    GPIOA->MODER &= ~GPIO_MODER_MODER1; // Clear mode bits for PA1 (set as input)
    // Configure PA1 as input with pull-up
    GPIOA->MODER &= ~GPIO_MODER_MODER1; // Input mode
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1; // Clear pull-up/down bits
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0; // Enable pull-up

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Configure PA1 as the source for EXTI1
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;

    // Configure EXTI1 for falling edge trigger and unmask it
    EXTI->IMR |= EXTI_IMR_MR1;     // Unmask EXTI1
    EXTI->FTSR |= EXTI_FTSR_TR1;   // Falling edge trigger

    // Enable EXTI1 interrupt in NVIC
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_SetPriority(EXTI1_IRQn, 2); // Set priority

}



volatile char ack_payload[10];

void SystemClock_Config(void);


void print(uint8_t data[], uint32_t len)
	{
	int i=0;
	while(len--){
		putchar(data[i]);
		i++;
	}
	printf("\n \r");
	}
// EXTI1 interrupt handler
void EXTI1_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR1) { // Check if EXTI1 triggered
        printf("Hi, I am pressed!\r\n");
        strcpy(ack_payload, "disable");
        // Clear the interrupt pending bit
        EXTI->PR = EXTI_PR_PR1;
    }
}

void  lcd_initial_characters(){
uint8_t rotate=0;
rotate=0x60;
ST7789_WriteCommand(ST7789_MADCTL);
ST7789_WriteSmallData(rotate);
ST7789_DrawImage(20,20,40,40,image_data_Image);
ST7789_DrawImage(20,70,40,40,spo2_image);
ST7789_DrawImage(20,120,40,40,EARTH_IMAGE);
}


void convert_to_str(uint8_t RxData[],int recv_width,char str[]){
int i=0;
while(recv_width--){
	str[i]=RxData[i];
	i++;
}
str[i]='\0';
}


int main(void)
{

SystemClock_Config();
ST7789_Init();
delay(3000);
uint8_t RxAddress[] = {0xB3,0xB4,0xB5,0xB6,0x05};
uint8_t RxData[32] ;
uint8_t channel=10;
usart_init();
strcpy(ack_payload, "ack_def");
NRF_INIT();
NRF_PRX_CONFIG(RxAddress,channel);
test_configure_btn_interupt();
printf("\n \rsetting up as PRX dynamic payload attemp \n \r");
for(int i=0;i<=0x1D;i++){
	printf("\n \r register %x is value %x \n \r",i,NRF_READ_REGISTER(i));
}

lcd_initial_characters();

while(1){
	if(is_data_on_pipe(0)==1){
		printf("\n \r recieved data \n \r");
		uint8_t recv_width=NRF_RECV_DATA(RxData);
		print(RxData,recv_width);
		char str[32];
		convert_to_str(RxData,recv_width,str);
		char *delimeter="-";
		char* tok=strtok(str,delimeter);
		char clear_section[10];
		   memset(clear_section, ' ', sizeof(clear_section));
		char heart_rate_data[10]={0};
		char spo2_data[10]={0};
		char gps_data_lat[10]={0};
		char gps_data_long[10]={0};
		int count_tok=0;
		while(tok!=NULL){
			printf("tok %s \n \r",tok);
			count_tok++;
			if(count_tok==1 && tok[0]=='H'){
				memset(heart_rate_data,0,10);
				strcpy(heart_rate_data,tok+1);
			}
			else if(count_tok==2 && tok[0]=='O'){
				memset(spo2_data,0,10);
				strcpy(spo2_data,tok+1);
			}
			else if(count_tok==3 && tok[0]=='G'){
				char *delim=",";
				char* tok2=strtok(tok+1,delim);
				strcpy(gps_data_lat,tok2);
				tok2=strtok(NULL,delim);
				strcpy(gps_data_long,tok2);
			}
			tok=strtok(NULL,delimeter);

		}
	//first clear previous text
	ST7789_WriteString(100, 20, clear_section, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
	ST7789_WriteString(100, 80, clear_section, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
	ST7789_WriteString(100, 120, clear_section, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
	ST7789_WriteString(100, 140, clear_section, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD

	ST7789_WriteString(100, 20, heart_rate_data, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
	ST7789_WriteString(100, 80, spo2_data, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
	ST7789_WriteString(100, 120, gps_data_lat, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
	ST7789_WriteString(100, 140, gps_data_long, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
	}

}

}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
