#ifndef __MOJE_H
#define __MOJE_H
#include "main.h"
#include "stm32f4xx.h"

/*------- Exported constans -----------------*/
#define LED_Green		GPIO_Pin_12
#define LED_Orange	GPIO_Pin_13
#define LED_Red			GPIO_Pin_14
#define LED_Blue		GPIO_Pin_15

/*------- Exported functions -----------------*/
void Delay(uint32_t nCount);
void _LED(void);
void USART_putf(USART_TypeDef *USARTx, uint8_t *, uint64_t size);
void USART_puts(USART_TypeDef *USARTx, volatile char *str);
#endif
