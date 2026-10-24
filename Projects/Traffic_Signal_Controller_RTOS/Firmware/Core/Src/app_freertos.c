/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : FreeRTOS applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"
#include "SevenSeg.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t SignalTaskHandle;
osThreadId_t CountdownTaskHandle;

osMessageQueueId_t CountdownQueue;

const osThreadAttr_t SignalTask_attributes = {
  .name = "SignalTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t CountdownTask_attributes = {
    .name = "CountdownTask",
    .priority = osPriorityNormal,
    .stack_size = 256 * 4
};
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void SignalTask(void *argument);
void CountdownTask(void *argument);
/* USER CODE END FunctionPrototypes */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	SevenSeg_Init();
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	CountdownQueue = osMessageQueueNew(3,sizeof(uint8_t),NULL);
  /* USER CODE END RTOS_QUEUES */
  /* creation of SignalTask */
	SignalTaskHandle = osThreadNew(SignalTask, NULL, &SignalTask_attributes);
	CountdownTaskHandle = osThreadNew(CountdownTask, NULL, &CountdownTask_attributes);
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_SignalTask */
/**
* @brief Function implementing the SignalTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SignalTask */

void SignalTask(void *argument)
{
	uint8_t RedPhase=20;
	uint8_t YellowPhase=05;
	uint8_t GreenPhase=35;


  /* Infinite loop */
	for(;;)
	{

	  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_10 |GPIO_PIN_6 | GPIO_PIN_11, GPIO_PIN_SET); //GREEN ON
	  	  osMessageQueuePut(CountdownQueue,&GreenPhase,0,0);
	  	  osDelay(35000);

	  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_10 , GPIO_PIN_RESET); //GREEN OFF

	  	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1 | GPIO_PIN_9 , GPIO_PIN_SET); //YELLOW ON
	  	  osMessageQueuePut(CountdownQueue,&YellowPhase,0,0);
	  	  osDelay(5000);

	  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1 | GPIO_PIN_9 |GPIO_PIN_6 | GPIO_PIN_11, GPIO_PIN_RESET);//YELLOW OFF

	  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 |GPIO_PIN_7 | GPIO_PIN_8 |GPIO_PIN_12, GPIO_PIN_SET); //RED ON
	      osMessageQueuePut(CountdownQueue,&RedPhase,0,0);
	      osDelay(20000);

	      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 |GPIO_PIN_7 | GPIO_PIN_8 |GPIO_PIN_12, GPIO_PIN_RESET); //RED OFF


	}

}


void CountdownTask(void *argument)
{
    uint8_t Phase;
    uint32_t startTick;
    uint32_t ticksPerSecond;

    ticksPerSecond = osKernelGetTickFreq();

    for (;;)
    {
        if (osMessageQueueGet(CountdownQueue, &Phase, NULL, osWaitForever) != osOK)
        {
            continue;
        }

        for (uint8_t count = Phase; count > 0U; count--)
        {
            SevenSeg_SetNumber(count);
            startTick = osKernelGetTickCount();

            while ((uint32_t)(osKernelGetTickCount() - startTick)
                   < ticksPerSecond)
            {
                SevenSeg_MultiplexTask();
                osDelay(1);
            }
        }

        SevenSeg_Blank();
    }
}


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

