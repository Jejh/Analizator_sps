
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "arm_math.h"
#include "pdm_filter.h"
#include "moje.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
static __IO uint32_t TimingDelay;
/* Exported macro ------------------------------------------------------------*/
#define ARM_MATH_CM4



#define DECIMATION_FACTOR       64
#define OUT_FREQ                40000
#define PDM_Input_Buffer_SIZE   ((OUT_FREQ/1000)*DECIMATION_FACTOR/8)
#define PCM_Output_Buffer_SIZE  (OUT_FREQ/1000)

/* Exported functions ------------------------------------------------------- */
void _RCC(void);
void _GPIO(void);
void _NVIC(void);
void _USART1(uint32_t baud);
void _LED(void);
void _I2S(void);
void _PDM(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
