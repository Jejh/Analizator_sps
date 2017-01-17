#include "main.h"
#include "moje.h"

/**
* @brief Function that makes delay
* @param nCount - miliseconds
* @retval -
*/
void Delay(__IO uint32_t nCount)
{ 
		nCount = nCount;
   while(nCount--);
}



void USART_puts(USART_TypeDef *USARTx, volatile char *str)
{
	while(*str)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); // check if there is no TC flag for USART1
		USART_SendData(USARTx, *str);
		*str++;
	}
	
}

/**
* @brief Funkcja do przesylania zmiennej typu FLOAT32 przez USART
* @param USARTx - pointer to USART
* @param data - pointer to one of four bits of float32
* @retval -
**/
void USART_putf(USART_TypeDef *USARTx, uint8_t *data, uint64_t size)
{
	for(uint32_t i=0; i<size; i++)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); // check if there is Tx register is empty
		USART_SendData(USARTx, data[i]);
		Delay(100);
		//GPIO_ToggleBits(GPIOD, LED_Red);
	}
}


/**
* @brief Function that configures user LEDS
* @param -
* @retval -
*/
void _LED(void)
{
	GPIO_InitTypeDef GPIO_Str;
  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_Str.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_Str.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Str.GPIO_OType = GPIO_OType_PP;
  GPIO_Str.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Str.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_Str);
  
}
