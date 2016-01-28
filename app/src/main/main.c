#include "cmsis_os.h"
#include "keys_n_ADCs.h"
#include "delay_timer.h"
#include "haptic.h"
#include "eeprom.h"
#include "hal_lcd.h"
#include "font.h"
#include "audio.h"
#include "misc.h"
#include "ADCs.h"
#include "communicate.h"
#include "display.h"
#include "usarts.h"
#include "global.h"




osThreadId 		Task_commHandle;
osThreadId 		Task_ADCsHandle;
osThreadId 		Task_dispHandle;

osMessageQId 	xQueue_ToComm;
osMessageQId 	xQueue_ToADCs;
osMessageQId 	xQueue_ToDisp;

osSemaphoreId	xSemaphore_ForComm;
osSemaphoreId	xSemaphore_ForADCs;





///////////////////函数声明/////////////////////////////////
void system_config(void);
void _init (void);


void main(void)
{
	system_config();	

	eepromInit();
	Audio_init();
	HAPTIC_init();
	keys_n_ADCs_init();
	LCD_Init();
	UARTS_init();

	/* Create the thread(s) */
    /* definition and creation of commTask */
    osThreadDef(commTask, Task_comm, osPriorityNormal, 0, 1024);
    Task_commHandle = osThreadCreate(osThread(commTask), NULL);

	osThreadDef(adcsTask, Task_ADCs, osPriorityNormal, 0, 1024);
    Task_ADCsHandle = osThreadCreate(osThread(adcsTask), NULL);

	osThreadDef(dispTask, Task_disp, osPriorityNormal, 0, 1024);
    Task_dispHandle = osThreadCreate(osThread(dispTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_QUEUES */
	osMessageQDef(toComm, 5, MSG_QUEUE_t);
	xQueue_ToComm = osMessageCreate(osMessageQ(toComm), NULL);
	
	osMessageQDef(toADCs, 5, MSG_QUEUE_t);
	xQueue_ToADCs = osMessageCreate(osMessageQ(toADCs), NULL);
	
	osMessageQDef(toDisp, 5, MSG_QUEUE_t);
	xQueue_ToDisp = osMessageCreate(osMessageQ(toDisp), NULL);
    /* USER CODE END RTOS_QUEUES */

	/*Semaphores*/
	osSemaphoreDef(forComm);
	osSemaphoreEmptyCreate(osSemaphore(forComm));

	osSemaphoreDef(forADCs);
	osSemaphoreEmptyCreate(osSemaphore(forADCs));

    //printf("tasks hava created, os starting ...\r\n");
    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    //printf("scheduler has been started ...\r\n");

    /* USER CODE BEGIN 3 */
    /* Infinite loop */
    while (1)
    {
        
    }
    /* USER CODE END 3 */

}


/***************************************************************************************************
* @fn      system_config
*
* @brief   系统资源初始化
* @param   NULL
* @return  null
***************************************************************************************************/ 
void system_config(void)
{
	SystemInit();
	/* Enable Prefetch Buffer */
	FLASH_PrefetchBufferCmd(ENABLE);

	/* Flash 2 wait state */
	FLASH_SetLatency(FLASH_Latency_2);

	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK ); 
}


void _init (void)
{
  
}










//////////////end of file/////////////////////////////////////////////////////
