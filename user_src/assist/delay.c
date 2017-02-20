#include "includes.h"
#include "delay.h"

void delay_us(int32u us)
{
    int32u i=us;
	do{
	#if defined ( __CC_ARM   ) 
       	__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); //60
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); 
		__nop(); //68
	#else
    //100MS->102MS
    //100US->104US
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");//16
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");//32
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");//48
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");
	__ASM volatile ("nop");//60
	__ASM volatile ("nop");
    __ASM volatile ("nop");
    __ASM volatile ("nop");
    __ASM volatile ("nop");
    __ASM volatile ("nop");
    __ASM volatile ("nop");
    __ASM volatile ("nop");
    //__ASM volatile ("nop");//68
	#endif
	}while(--i);

}


void delay_ms(int32u us)
{
	int32u i;
	IWDG_ReloadCounter();
	for(i=0;i<us;i++)
		delay_us(1000);
}

