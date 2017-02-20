#include "includes.h"
#include "board.h"
#include "apps/storage.h"
#include "apps/motor-control.h"
#include "gui/widget.h"
void board_rcc_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
}


/*key*/
DECLARE_GPIO_BUS(key_l,0x14,6);//pb4-pb9
void board_key_init(void)
{
	gpio_bus_init(&gpio_bus_key_l,GPIO_Mode_IPU);
	gpio_bus_set_data(&gpio_bus_key_l,0xff);
}

int32u board_key_get(void)
{
	int16u key_val=gpio_bus_read_input(&gpio_bus_key_l);
	return (~key_val)&0x3f;
}

//´®¿Ú
DECLARE_USART(1,0x09,0x0a);

void board_serial_init(void)
{

	uart_init(&uart_1,57600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
}

struct communicate comm_1;
struct transfer trans_1;

struct communicate comm_usb;
struct transfer trans_usb;
void communication_init(void)
{
	trans_1.port=&uart_1;
	trans_1.reset=serial_reset;
	trans_1.tick=serial_tick;
	trans_1.push_data=serial_push_bytes;
	trans_1.send=serial_send;
	trans_1.receive_end_callback=0;
	trans_1.copy_out_data=serial_copy_out;
	
	comm_1.trans=&trans_1;
	comm_1.protocol_parse=comm_protocol_parse;
	comm_1.cmd_parse=comm_cmd_run;
	uart_start_rx(&uart_1);

	
	trans_usb.port=NULL;
	trans_usb.reset=NULL;
	trans_usb.tick=NULL;
	trans_usb.push_data=usb_push_bytes;
	trans_usb.send=usb_send;
	trans_usb.receive_end_callback=0;
	trans_usb.copy_out_data=usb_copy_out;
	
	comm_usb.trans=&trans_usb;
	comm_usb.protocol_parse=comm_protocol_parse;
	comm_usb.cmd_parse=comm_cmd_run;
	
}



//²âÊÔ
DECLARE_GPIO(test,0x43);//pe3
void board_test_init(void)
{
	gpio_init(&gpio_test,GPIO_Mode_Out_PP);
	gpio_reset(&gpio_test);
	gpio_set(&gpio_test);
	gpio_reset(&gpio_test);
}




//led
DELCARE_LED(1,0x45,0);//pe5
DELCARE_LED(2,0x46,0);//pe6
void board_led_init(void)
{
	led_init(&led_1,GPIO_Mode_Out_OD);
	led_off(&led_1);
	led_off(&led_2);
}



extern void board_usb_init(void);
void board_init(void)
{

	/* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	board_rcc_init();
	board_key_init();
	board_led_init();
	board_usb_init();
	board_serial_init();
	communication_init();

}




