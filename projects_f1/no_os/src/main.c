

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "board.h"
#include "assist/ByteQueue.h"

DECLARE_BYTE_QUEUE(pc,256);
DECLARE_BYTE_QUEUE(wireless,256);

int main(void)
{
	int8u byte_to_pc,byte_to_wireless;
	board_init();
  while (1)
	{
		if(byte_queue_receive(&byte_queue_wireless,&byte_to_pc))//无线接受，传到pc口
		{
			board_485_tx();
			uart_send_byte_sync(USART1,byte_to_pc);
			board_485_rx();
		}
		if(byte_queue_receive(&byte_queue_pc,&byte_to_wireless))//pc接受，传到无线口
		{
			uart_send_byte_sync(USART3,byte_to_wireless);
		}
  }
}


void system_tick(int ticks)
{
	led1_tick(ticks);
	led2_tick(ticks);
}

//射频模块通信端口
void USART3_IRQHandler(void)
{
	int8u serial_out;
	if(USART3->SR & (USART_FLAG_RXNE|USART_FLAG_ORE))
	{
		serial_out=USART3->DR;
		byte_queue_put(&byte_queue_wireless,serial_out);
		board_led_on(LED2,20);
	}
}

//pc通信端口
void USART1_IRQHandler(void)
{
	int8u serial_out;
	if(USART1->SR & (USART_FLAG_RXNE|USART_FLAG_ORE))
	{
		serial_out=USART1->DR;
		byte_queue_put(&byte_queue_pc,serial_out);
		board_led_on(LED1,20);
	}
}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
