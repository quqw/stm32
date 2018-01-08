

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "board/board.h"
#include "usb_func.h"
#include "drivers/keyboard/keyboard.h"
#include "apps/motor-control.h"
#include "form/mainform.h"
#include "form/editform.h"
extern struct uart uart_1;


int main(void)
{ 
	board_init();
	USB_Init();
	vTaskStartScheduler();
	for( ;; );
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
