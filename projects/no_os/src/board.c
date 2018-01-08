#include "includes.h"
#include "board.h"
void board_rcc_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
}

//射频115200
void board_rf_config(void)
{
	GPIO_InitTypeDef gpio_init;
	NVIC_InitTypeDef   nvic_init;
	USART_InitTypeDef uart_init;
	//power 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpio_init.GPIO_Pin = GPIO_Pin_15;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	board_rf_power_off();
  GPIO_Init(GPIOB, &gpio_init);
	delay_ms(1000);
	//uart
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  uart_init.USART_BaudRate=115200;
  uart_init.USART_WordLength=USART_WordLength_8b;
  uart_init.USART_StopBits=USART_StopBits_1;
  uart_init.USART_Parity=USART_Parity_No;
  uart_init.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
  uart_init.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART3, &uart_init);
	
	//uart 3 tx~pb10 rx~b11
	gpio_init.GPIO_Pin = GPIO_Pin_10;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &gpio_init);
	
	gpio_init.GPIO_Pin = GPIO_Pin_11;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &gpio_init); 
	
	nvic_init.NVIC_IRQChannel                   = USART3_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init.NVIC_IRQChannelSubPriority        = 0;
	nvic_init.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic_init); 
				
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  USART_Cmd(USART3, ENABLE);
}
void board_rf_power_on(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}	
void board_rf_power_off(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}	

void board_serial_config(void)
{
	GPIO_InitTypeDef gpio_init;
	NVIC_InitTypeDef   nvic_init;
	USART_InitTypeDef uart_init;
		//uart1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  uart_init.USART_BaudRate=57600;
  uart_init.USART_WordLength=USART_WordLength_8b;
  uart_init.USART_StopBits=USART_StopBits_1;
  uart_init.USART_Parity=USART_Parity_No;
  uart_init.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
  uart_init.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART1, &uart_init);
	
	//485 ctrl
	gpio_init.GPIO_Pin = GPIO_Pin_8;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &gpio_init);
	
	//uart 1 tx~pa9 rx~a10
	gpio_init.GPIO_Pin = GPIO_Pin_9;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &gpio_init);
	
	gpio_init.GPIO_Pin = GPIO_Pin_10;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &gpio_init); 
	
	nvic_init.NVIC_IRQChannel                   = USART1_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init.NVIC_IRQChannelSubPriority        = 0;
	nvic_init.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic_init); 

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  USART_Cmd(USART1, ENABLE);
	board_485_rx();		
}
void board_485_rx(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}
void board_485_tx(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

void uart_send_byte_sync(USART_TypeDef *uart,int8u b)
{
	while((uart->SR & USART_FLAG_TXE)==0);
	uart->DR=b;
	//多读一下SR，为了让TC清零。不然第一个字节发不出去,因为TC初始值为1。
	while((uart->SR & USART_FLAG_TXE)==0 || (uart->SR & USART_FLAG_TC) == 0);
}


//led
void board_led_config(void)
{
	GPIO_InitTypeDef gpio_init;
	//power 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpio_init.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &gpio_init);
	board_led_off(LED1);
	board_led_off(LED2);
}
static int led1_on=0;
static int led2_on=0;
void led1_tick(int ticks)
{
	if(led1_on==0)
		return;
	if(led1_on>ticks)
		led1_on-=ticks;
	else
	{
		led1_on=0;
		board_led_off(LED1);
	}
}
void led2_tick(int ticks)
{
	if(led2_on==0)
		return;
	if(led2_on>ticks)
		led2_on-=ticks;
	else
	{
		led2_on=0;
		board_led_off(LED2);
	}
}

void board_led_on(int led,int time)
{
	if(led&LED1)
	{
		led1_on=time;
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
	if(led&LED2)
	{
		led2_on=time;
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
		
}	
void board_led_off(int led)
{
	if(led&LED1)
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
	if(led&LED2)
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
}	
void board_led_toggle(int led)
{
	if(led&LED1)		GPIOA->ODR ^=GPIO_Pin_6;
	if(led&LED2)
		GPIOA->ODR ^=GPIO_Pin_7;
}	
void board_init(void)
{
	if (SysTick_Config(SystemCoreClock / 100)) //10ms
	{ 
	/* Capture error */ 
	while (1);
	}
	  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	board_rcc_init();
	board_led_config();
	board_rf_config();
	board_rf_power_on();
	delay_ms(2000);//等待射频模块
	board_serial_config();
}




