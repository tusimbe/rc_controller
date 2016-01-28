#include "usarts.h"


/////////////////////ȫ�ֱ�������///////////////////////////////////////
uint8_t 	Sport_TxBuffer[S_PORT_TX_BUFF_LEN];
uint8_t 	Sport_RxBuffer[S_PORT_RX_BUFF_LEN];

uint8_t		dbg_TxBuffer[DBG_TX_BUF_LEN];

uint8_t		sPortWriteBusyFlag;					//s-portд��æ��־ 
uint8_t		dbgWriteBusyFlag;					//���Կ�д��æ��־


/***************************************************************************************************
 * @fn      UARTS_Registe
 *
 * @brief   ������Դע��
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void UARTS_Registe(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;
  	USART_InitTypeDef   USART_InitStructure;
  	DMA_InitTypeDef     DMA_InitStructure;
	uint32_t            temp;

	//USART_S_PORT_TxPin
	RCC_AHB1PeriphResetCmd(USART_S_PORT_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = USART_S_PORT_TxPin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(USART_S_PORT_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(USART_S_PORT_GPIO, USART_S_PORT_Tx_Pinsource, USART_S_PORT_GPIO_AF);

	//USART_S_PORT_RxPin
	GPIO_InitStructure.GPIO_Pin = USART_S_PORT_RxPin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(USART_S_PORT_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(USART_S_PORT_GPIO, USART_S_PORT_Rx_Pinsource, USART_S_PORT_GPIO_AF);

	//USARTdbg_TxPin
	RCC_AHB1PeriphResetCmd(USARTdbg_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = USARTdbg_TxPin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(USARTdbg_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(USARTdbg_GPIO, USARTdbg_Tx_Pinsource, USARTdbg_GPIO_AF);

	//USARTdbg_RxPin
	GPIO_InitStructure.GPIO_Pin = USARTdbg_RxPin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(USARTdbg_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(USARTdbg_GPIO, USARTdbg_Rx_Pinsource, USARTdbg_GPIO_AF);


	//---------------------USART_S_PORT���ڹ�������---------------------
	//�򿪴��ڶ�Ӧ������ʱ��  
	RCC_APB1PeriphClockCmd(USART_S_PORT_CLK, ENABLE); 
	//���ڷ�DMA����  
	//����DMAʱ��
	RCC_APB1PeriphResetCmd(USART_S_PORT_DMA_CLK, ENABLE);

	//DMA1ͨ������
	//�ر�ͨ��
	DMA_Cmd(USART_S_PORT_Tx_DMA_STREAM,DISABLE);
	DMA_DeInit(USART_S_PORT_Tx_DMA_STREAM);
	//�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART_S_PORT->DR);
	//�ڴ��ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Sport_TxBuffer;
	//dma���䷽����
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_BufferSize = S_PORT_TX_BUFF_LEN;
	//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//���������ֳ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//�ڴ������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//����DMA�����ȼ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(USART_S_PORT_Tx_DMA_STREAM,&DMA_InitStructure);
	//���ж�
	DMA_ClearITPendingBit(USART_S_PORT_Tx_DMA_STREAM, USART_S_PORT_Tx_DMA_FLAG);
	//ʹ���ж�
	DMA_ITConfig(USART_S_PORT_Tx_DMA_STREAM,DMA_IT_TC,ENABLE);

	//������DMA����  
	//�ر�ͨ��
	DMA_Cmd(USART_S_PORT_Rx_DMA_STREAM,DISABLE);
	DMA_DeInit(USART_S_PORT_Rx_DMA_STREAM);
	//�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART_S_PORT->DR);
	//�ڴ��ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Sport_RxBuffer;
	//dma���䷽����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_BufferSize = S_PORT_RX_BUFF_LEN;
	//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//���������ֳ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//�ڴ������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//����DMA�����ȼ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	//����DMA��2��memory�еı����������
	DMA_Init(USART_S_PORT_Rx_DMA_STREAM,&DMA_InitStructure);
	DMA_Cmd(USART_S_PORT_Rx_DMA_STREAM,ENABLE);  


	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//��ʼ������ 
	USART_Init(USART_S_PORT,&USART_InitStructure);  
	  
	//�ж�����
	USART_ITConfig(USART_S_PORT,USART_IT_TC,DISABLE);  
	USART_ITConfig(USART_S_PORT,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART_S_PORT,USART_IT_IDLE,ENABLE);

	//����UART�ж�  
	NVIC_InitStructure.NVIC_IRQChannel = USART_S_PORT_IRQn;                           //ͨ������Ϊ����1�ж�  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_S_PORT_Rx_NVIC_PRIORITY;   //�ж�ռ�ȵȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //�ж���Ӧ���ȼ�0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�  
	NVIC_Init(&NVIC_InitStructure);   

	//DMA�����ж�  
	NVIC_InitStructure.NVIC_IRQChannel = USART_S_PORT_Tx_DMA_IRQn;                        			//ͨ������Ϊ����1�ж�  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_S_PORT_Tx_DMA_NVIC_PRIORITY;       //�ж�ռ�ȵȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //�ж���Ӧ���ȼ�0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�  
	NVIC_Init(&NVIC_InitStructure);  

	//���ж�
	temp = USART_S_PORT->SR;
	temp = USART_S_PORT->DR; //��USART_IT_IDLE��־
	  
	//����DMA��ʽ����
	USART_DMACmd(USART_S_PORT,USART_DMAReq_Tx,ENABLE);
	//  //����DMA��ʽ����
	USART_DMACmd(USART_S_PORT,USART_DMAReq_Rx,ENABLE);
	//��������  
	USART_Cmd(USART_S_PORT, ENABLE); 

	//---------------------USARTdbg���ڹ�������---------------------
	//�򿪴��ڶ�Ӧ������ʱ��  
	RCC_APB1PeriphClockCmd(USARTdbg_CLK, ENABLE); 
	//���ڷ�DMA����  
	//�ر�ͨ��
	DMA_Cmd(USARTdbg_Tx_DMA_STREAM,DISABLE);
	DMA_DeInit(USARTdbg_Tx_DMA_STREAM);
	//�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USARTdbg->DR);
	//�ڴ��ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)dbg_TxBuffer;
	//dma���䷽����
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_BufferSize = DBG_TX_BUF_LEN;
	//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//���������ֳ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//�ڴ������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//����DMA�����ȼ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(USARTdbg_Tx_DMA_STREAM,&DMA_InitStructure);
	//���ж�
	DMA_ClearITPendingBit(USARTdbg_Tx_DMA_STREAM, USARTdbg_Tx_DMA_FLAG);
	//ʹ���ж�
	DMA_ITConfig(USARTdbg_Tx_DMA_STREAM,DMA_IT_TC,ENABLE);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//��ʼ������ 
	USART_Init(USARTdbg,&USART_InitStructure); 

	//DMA�����ж�  
	NVIC_InitStructure.NVIC_IRQChannel = USARTdbg_Tx_DMA_IRQn;                        			//ͨ������Ϊ����1�ж�  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USARTdbg_Tx_DMA_NVIC_PRIORITY;       //�ж�ռ�ȵȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //�ж���Ӧ���ȼ�0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�  
	NVIC_Init(&NVIC_InitStructure);  
	  
	//����DMA��ʽ����
	USART_DMACmd(USARTdbg,USART_DMAReq_Tx,ENABLE);
	//��������  
	USART_Cmd(USARTdbg, ENABLE); 
}


/***************************************************************************************************
 * @fn      UARTS_init
 *
 * @brief   ������Դ��ʼ��
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void UARTS_init(void)
{
	UARTS_Registe();	
}


/***************************************************************************************************
 * @fn      USART_S_PORT_send
 *
 * @brief   s-port���ڷ���
 * @param   str -- ��������
 *			len -- ���ݳ���
 * @return  null
 ***************************************************************************************************/  
uint8_t USART_S_PORT_send(uint8_t *Str, uint8_t len)
{
	if(len > S_PORT_TX_BUFF_LEN)
		return 0;

	if(sPortWriteBusyFlag)
		return 0;

	sPortWriteBusyFlag = 1;	

	memcpy(Sport_TxBuffer, Str, len);
	DMA_SetCurrDataCounter(USART_S_PORT_Tx_DMA_STREAM,len);
	DMA_Cmd(USART_S_PORT_Tx_DMA_STREAM,ENABLE);

	return 1;
}


/***************************************************************************************************
 * @fn      USART_S_PORT_Tx_DMA_IRQHandler_Callback
 *
 * @brief   s-port����DMA�����жϻص�
 * @param   null
 * @return  null
 ***************************************************************************************************/  
void USART_S_PORT_Tx_DMA_IRQHandler_Callback(void)
{
	if(DMA_GetITStatus(USART_S_PORT_Tx_DMA_STREAM, USART_S_PORT_Tx_DMA_FLAG) != RESET)
	{
		DMA_ClearITPendingBit(USART_S_PORT_Tx_DMA_STREAM, USART_S_PORT_Tx_DMA_FLAG);
		DMA_Cmd(USART_S_PORT_Tx_DMA_STREAM,DISABLE);
		sPortWriteBusyFlag = 0;		
	}
}

/***************************************************************************************************
 * @fn      USART_S_PORT_IRQHandler_Callback
 *
 * @brief   s-port�����жϻص�
 * @param   null
 * @return  null
 ***************************************************************************************************/ 
extern USART_S_PORT_IRQHandler_CallbackHook(uint8_t *msg, uint8_t len);
void USART_S_PORT_IRQHandler_Callback(void)
{
	uint32_t temp = 0;
	uint16_t len = 0;

	if(USART_GetITStatus(USART_S_PORT, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(USART_S_PORT_Rx_DMA_STREAM,DISABLE);

		temp = USART_S_PORT->SR;
		temp = USART_S_PORT->DR; //��USART_IT_IDLE��־  
		temp = temp;

		//�����ֽ���
		len = S_PORT_RX_BUFF_LEN - DMA_GetCurrDataCounter(USART_S_PORT_Rx_DMA_STREAM);

		USART_S_PORT_IRQHandler_CallbackHook(Sport_RxBuffer, len);

		//���ô������ݳ���
		DMA_SetCurrDataCounter(USART_S_PORT_Rx_DMA_STREAM,S_PORT_RX_BUFF_LEN);
		//��DMA
		DMA_Cmd(USART_S_PORT_Rx_DMA_STREAM,ENABLE);
	} 
}


/***************************************************************************************************
 * @fn      USARTdbg_send
 *
 * @brief   ���Դ��ڷ���
 * @param   str -- ��������
 *			len -- ���ݳ���
 * @return  null
 ***************************************************************************************************/  
uint8_t USARTdbg_send(uint8_t *Str, uint16_t len)
{
	if(len > DBG_TX_BUF_LEN)
		return 0;
	
	if(dbgWriteBusyFlag)
		return 0;

	dbgWriteBusyFlag = 1;

	memcpy(dbg_TxBuffer, Str, len);
	DMA_SetCurrDataCounter(USARTdbg_Tx_DMA_STREAM,len);
	DMA_Cmd(USARTdbg_Tx_DMA_STREAM,ENABLE);

	return 1;
}


/***************************************************************************************************
 * @fn      USARTdbg_Tx_DMA_IRQHandler_Callback
 *
 * @brief   ���Դ���DMA�����жϻص�
 * @param   null
 * @return  null
 ***************************************************************************************************/  
void USARTdbg_Tx_DMA_IRQHandler_Callback(void)
{
	if(DMA_GetITStatus(USARTdbg_Tx_DMA_STREAM, USARTdbg_Tx_DMA_FLAG) != RESET)
	{
		DMA_ClearITPendingBit(USARTdbg_Tx_DMA_STREAM, USARTdbg_Tx_DMA_FLAG);
		DMA_Cmd(USARTdbg_Tx_DMA_STREAM,DISABLE);
		dbgWriteBusyFlag = 0;		
	}
}



//////////////end of file/////////////////////////////////////////////////////