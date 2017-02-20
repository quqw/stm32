//#include "includes.h"
#include "platform_drivers/st_gpio.h"
#include "platform_drivers/st_uart.h"
#include "platform_drivers/st_timer.h"
#include "board.h"
#include "communication/comm_types.h"
#include "communication/serial_transfer.h"
#include "communication/comm_app.h"
void board_rcc_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
}


//´®¿Ú
DELCARE_ST_UART(main,USART1);
void serial_init(void)
{
	USART_InitTypeDef init;
	init.USART_BaudRate=115200;
	init.USART_WordLength=USART_WordLength_8b;
	init.USART_StopBits=USART_StopBits_1;
	init.USART_Parity=USART_Parity_No;
	init.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	init.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	st_uart_init(&st_uart_main,&init);
	st_uart_start_rx((comm_port_t*)&st_uart_main);
}

struct communicate comm_1;
struct transfer trans_serial;
void communication_init(void)
{
	trans_serial.port=(comm_port_t*)&st_uart_main;
	trans_serial.reset=serial_reset;
	trans_serial.tick=serial_tick;
	trans_serial.push_data=serial_push_bytes;
	trans_serial.send=serial_send;
	trans_serial.receive_end_callback=0;
	trans_serial.copy_out_data=serial_copy_out;
	
	comm_1.trans=&trans_serial;
	comm_1.protocol_parse=comm_protocol_parse;
	comm_1.cmd_parse=comm_cmd_run;
}

//led
DECLARE_ST_GPIO(led,GPIOB,GPIO_Pin_8);
void board_led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	st_gpio_init(&st_gpio_led,&GPIO_InitStructure);
}




extern void board_usb_init(void);
void board_init(void)
{
	
	if (SysTick_Config(SystemCoreClock / 100)) //10ms
	{ 
	/* Capture error */ 
	while (1);
	}
	  /* 2 bit for pre-emption priority, 2 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//board_timer_init();
	board_rcc_init();
	board_led_init();
	//board_key_init();
	board_usb_init();
	communication_init();
	serial_init();

}




