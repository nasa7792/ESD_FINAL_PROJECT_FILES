
#include "main.h"
#include "usart.h"
#include <stdio.h>
#include"utilities.h"
#include"delay.h"
#include"SPI.h"
#include"NRF_DRIVER.h"
#include"max_heart_sensor_driver.h"

void SystemClock_Config(void);
int main(void)
{

SystemClock_Config();
usart_init();
printf("trying to init max30102 \n \r");
MAX30102_init();
printf("after  to init max30102 \n \r");
uint8_t get_id;
while (1) {
    // Start temperature measurement
    MAX30102_Start_Temperature_Measurement();
    uint32_t start=millis();
    uint8_t temp_ready_flag =0;
    while (!(temp_ready_flag & 0x02)) {

    	if(isTimeout(start,200)){
            printf("Temperature measurement timeout!\n\r");
               break; // Exit the loop on timeout
    	}
    	MAX30102_READ_REGISTER(INTERRUPT_STATUS_2, &temp_ready_flag, 1);
    	delay(2);
    }
    if (temp_ready_flag & 0x02) {
        float temperature = MAX30102_Read_Temperature();
        printf("\n\rTemperature: %f C\n\r", temperature);
    }

}
}
/*
delay(6000);
uint8_t TxAddress[] = {0xEE,0xDD,0xCC,0xBB,0xAA};
uint8_t TxData[32]="where did my data go ?";
uint8_t channel=10;


NRF_INIT();
NRF_PTX_CONFIG(TxAddress,channel);

for(int i=0;i<0x1D;i++){
	printf("\n \r register %x is value %x \n \r",i,NRF_READ_REGISTER(i));
}

while(1){
delay(600);
if(NRF_TX_DATA(TxData)==1){
printf("\n \r transmit success \n \r");
}
else{
	printf("\n \r Something went wrong ! \n \r");
}
}
*/

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
