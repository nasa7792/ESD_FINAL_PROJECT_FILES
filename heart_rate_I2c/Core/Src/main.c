
#include "main.h"
#include "usart.h"
#include <stdio.h>
#include"utilities.h"
#include"delay.h"
#include"SPI.h"
#include"NRF_DRIVER.h"
#include"max_heart_sensor_driver.h"
#include <stdbool.h>
#include"spo2_algorithm.h"
#include"heartRate.h"
//#define __AVR_ATmega168__ 1

void SystemClock_Config(void);
int main(void)
{

SystemClock_Config();
usart_init();
printf("trying to init max30102 \n \r");
MAX30102_init();
printf("after  to init max30102 \n \r");

uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
int32_t bufferLength;
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid
while(1){

	//printf("ir %lu \n \r",getIR());
	//printf("red %lu \n \r",getRed());

	bufferLength=100;
	  for (uint8_t i = 0 ; i < bufferLength ; i++)
	  {
	    while (available() == false) //do we have new data?
	    	check_new_data(); //Check the sensor for new data

	    redBuffer[i] = getRed();
	    irBuffer[i] =  getIR();
	    nextSample(); //We're finished with this sample so move to next sample

//	    printf("\n \r RED =%lu",redBuffer[i]);
//	    printf(" IR =%lu",irBuffer[i]);
	  }
	  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
	  while (1)
	  {
	    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
	    for (uint8_t i = 25; i < 100; i++)
	    {
	      redBuffer[i - 25] = redBuffer[i];
	      irBuffer[i - 25] = irBuffer[i];
	    }

	    //take 25 sets of samples before calculating the heart rate.
	    for (uint8_t i = 75; i < 100; i++)
	    {
	      while (available() == false) //do we have new data?
	    	  check_new_data(); //Check the sensor for new data



	      redBuffer[i] = getRed();
	      irBuffer[i] = getIR();
	     nextSample(); //We're finished with this sample so move to next sample

	      //send samples and calculation result to terminal program through UART
	   //  printf("\n \r RED =%lu",redBuffer[i]);
	 	// printf(" IR =%lu",irBuffer[i]);

	     printf("\n \r HEARATE =%ld",heartRate);
	     printf(" is valid HEARATE =%d",validHeartRate);
	 	 printf(" spo2 =%ld",spo2);
	     printf(" is valid spo2 =%d",validSPO2);
	    }

	    //After gathering 25 new samples recalculate HR and SP02
	    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
	    delay(1000);
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
