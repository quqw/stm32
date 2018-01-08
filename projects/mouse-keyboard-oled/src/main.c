

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_pwr.h"
#include "board/board.h"
#include "usb_func.h"
#include "communication/comm_app.h"
#include "platform_drivers/st_uart.h"
#include "assist/delay.h"
extern struct uart uart_1;
extern struct communicate comm_1;

void func_mouse_click(void)
{
    hid_send_mouse(1,0,0);
    delay_us(1000);
    hid_send_mouse(0,0,0);
    delay_us(1000);
}
int main(void)
{
	int8u serial_out,key[8];
    BOOL click_start=FALSE;
  board_init();
  USB_Init();
    memset(key,0,8);
  while (1)
    {	
//        switch(board_key_get())
//        {
//        case KEY_LEFT:
//            //hid_send_mouse(0,-2,0);
// 						hid_send_mouse(1,0,0);
//            break;
//        case KEY_RIGHT:
//            //hid_send_mouse(0,2,0);
// 						hid_send_mouse(2,0,0);
//            break;
//        case KEY_UP:
//            //hid_send_mouse(0,0,-2);
// 						hid_send_mouse(4,0,0);
//            break; 
//        case KEY_DOWN:
//            hid_send_mouse(0,0,2);
//            break;
//        case KEY_ENTER:
//            click_start=click_start?FALSE:TRUE;
//            delay_ms(500);
//            break;          
//        }
//        if(click_start)
//            func_mouse_click();
		if(st_uart_read_byte((comm_port_t*)&uart_1,&serial_out)==0)
			continue;
		comm_get_data_callback(&comm_1,&serial_out,1);
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
