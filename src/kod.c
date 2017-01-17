#include "main.h"
GPIO_InitTypeDef GPIO_Str;
USART_InitTypeDef USART_Str;
NVIC_InitTypeDef NVIC_Str;
I2S_InitTypeDef I2S_Str;
PDMFilter_InitStruct Filter;

void _RCC(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB |	RCC_AHB1Periph_GPIOC, ENABLE); //GPIO dla UART oraz MP45DT02
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // zegar dla USART1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //zegar dla SPI2 do ktorego podpiety jest mikrofon
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE); // zegar CRC dla struktury filtru PDM
	RCC_PLLI2SCmd(ENABLE); // wlaczenie PLL dla interfejsu I2S
}

/**
* @brief Function that configures GPIO
* @param -
* @retval -
*/
void _GPIO(void)
{	
	// Configure USART1 Tx->PB6 | Rx->PB7
  GPIO_Str.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Str.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_Str.GPIO_OType = GPIO_OType_PP;
  GPIO_Str.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Str.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_Str);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);  // Connect PB6 to USART1 Tx
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);  // Connect PB7  to USART1 Rx
	
	// Configure MP45DT02's CLK / I2S2_CLK (PB10) line
	GPIO_Str.GPIO_Pin   = GPIO_Pin_10;
  GPIO_Str.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_Str.GPIO_OType = GPIO_OType_PP;
  GPIO_Str.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Str.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOB, &GPIO_Str);

  // Configure MP45DT02's DOUT / I2S2_DATA (PC3) line
  GPIO_Str.GPIO_Pin   = GPIO_Pin_3;
  GPIO_Str.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_Str.GPIO_OType = GPIO_OType_PP;
  GPIO_Str.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Str.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOC, &GPIO_Str);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);  // Connect pin 10 of port B to the SPI2 peripheral
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);   // Connect pin 3 of port C to the SPI2 peripheral
}

/**
* @brief Function that configures USART1
* @param baud: determines data send speed (kbytes/sec)
* @retval -
*/
void _USART1(uint32_t baud)
{		
	USART_Str.USART_BaudRate = 9600;
	USART_Str.USART_WordLength = USART_WordLength_8b;
	USART_Str.USART_StopBits = USART_StopBits_1;
	USART_Str.USART_Parity = USART_Parity_No;
	USART_Str.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Str.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_Str);
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
}

/**
* @brief Function that configures NVIC
* @param -
* @retval -
*/
void _NVIC(void) 
{	
	// Configure the interrupt priority grouping
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	// Config USART1 channel
	/*
	NVIC_Str.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Str.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Str.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Str.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_Str);	
	NVIC_EnableIRQ(USART1_IRQn);
	*/
	
	// Przerwanie SPI2
	NVIC_Str.NVIC_IRQChannel =SPI2_IRQn;
	NVIC_Str.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Str.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Str.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_Str);
	NVIC_EnableIRQ(SPI2_IRQn);
}

void _I2S(void)
{ 	
	SPI_I2S_DeInit(SPI2);
	I2S_Str.I2S_AudioFreq = 40000*2;
	I2S_Str.I2S_Standard = I2S_Standard_LSB;
	I2S_Str.I2S_DataFormat = I2S_DataFormat_16b;
	I2S_Str.I2S_CPOL = I2S_CPOL_High;
	I2S_Str.I2S_Mode = I2S_Mode_MasterRx;
	I2S_Str.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
	// Inicjalizacja struktury I2S_Str
	I2S_Init(SPI2, &I2S_Str);
	
	// Wlaczenie przerwan od zapelnienia bufora odbiorczego I2S2
	SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
	
	I2S_Cmd(SPI2, ENABLE);;
}


void _PDM(void)
{	
	
	// Initialize PDM filter
  Filter.Fs = 2*OUT_FREQ;
  Filter.HP_HZ = 0;
  Filter.LP_HZ = 20000;
  Filter.In_MicChannels = 1;
  Filter.Out_MicChannels = 1;
  PDM_Filter_Init(&Filter);
}
