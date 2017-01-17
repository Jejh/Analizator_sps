/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "arm_math.h"

/* Variables -----------------------------------------------------------------*/
uint32_t speed;
float32_t buffer_input[1024];
float32_t buffer_input_copy[256];
//float32_t buffer_output[1024];
//float32_t buffer_output_mag[1024];
//float32_t buffer_output_mag_copy[256];
uint8_t PDM_Input_Buffer[PDM_Input_Buffer_SIZE];
uint16_t PCM_Output_Buffer[PCM_Output_Buffer_SIZE];
uint8_t Data_Status = 0;
uint16_t z, i;
volatile unsigned char str;

union
{
	float32_t buffer_output[1024];
	uint8_t output_bytes[4096];
}bout;

union
{
	float32_t buffer_output_mag[512];
	uint8_t output_bytes_mag[2048];
}bout_mag;

uint32_t size_out = sizeof(bout)/8;
uint32_t size_moout = sizeof(bout_mag)/8;

/* Pointers ------------------------------------------------------------------*/
arm_rfft_instance_f32 S;
arm_cfft_radix4_instance_f32 S_CFFT;
uint16_t output;
/* ---------------------------------------------------------------------------*/
int main(void)
{
	_RCC();
	_GPIO();
	_LED();
	_USART1(9600);
	_NVIC();	
	_PDM();
	GPIO_SetBits(GPIOD, LED_Green);
	_I2S();
	
		
	arm_rfft_init_f32(&S, &S_CFFT, 512, 0, 1);
	
	//I2S_Cmd(SPI2, ENABLE);
	
	
	//----------------- Infinite loop
  while (1) 
	{
		
		// Jesli nowa paczka danych PCM jest gotowa
		if(Data_Status)
		{
			GPIO_SetBits(GPIOD, LED_Orange);
			//Kopiuj paczki danych do bufora
			for(i =0; i<(OUT_FREQ/1000); i++)
			{
				buffer_input[i+(OUT_FREQ/1000)*z] = (float32_t) PCM_Output_Buffer[i];
			}
			
			++z;
			
			
			if(z>512)
			{
				z=0;				
				/* ------- */
				//Wyznaczanie transformaty
				arm_rfft_f32(&S, buffer_input, bout.buffer_output);
				//Obliczanie modulow
				arm_cmplx_mag_f32(bout.buffer_output, bout_mag.buffer_output_mag, 512);
				
				I2S_Cmd(SPI2, DISABLE);
				GPIO_ToggleBits(GPIOD, LED_Orange);
			  Data_Status = 0;
				
				USART_putf(USART1, bout.output_bytes, size_out);
				GPIO_ToggleBits(GPIOD, LED_Red);
				Delay(100);
			}
		
		}

	}

}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */


#endif

