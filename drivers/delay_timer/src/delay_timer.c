#include "delay_timer.h"





uint32_t		msDlyCnter;



/***************************************************************************************************
 * @fn      msConter
 *
 * @brief   ms计数，在SysTick_Handler中调用
 * @param   NULL
 * @return  null
 ***************************************************************************************************/ 
void msConter(void)
{
	msDlyCnter++;	
}


/***************************************************************************************************
 * @fn      Delay_ms
 *
 * @brief   延时n ms
 * @param   n -- 要延时的毫秒值
 * @return  null
 ***************************************************************************************************/ 
void Delay_ms(uint32_t n)
{
	uint32_t cntr;

	cntr = msDlyCnter;
	
	while(msDlyCnter < (cntr+n));
}


//////////////end of file/////////////////////////////////////////////////////
