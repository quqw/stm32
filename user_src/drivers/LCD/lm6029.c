#include "includes.h"
#include "platform/delay.h"
#include "platform/io8080.h"
#include "platform/gpio.h"
#include "drivers/led.h"
#include "lm6029.h"

void lm6029_write_cmd_default(struct lcd_hw *lm,int16u cmd)
{
	lm->io_bus->write(lm->io_bus,0,(int8u)cmd);
}

void lm6029_write_data_default(struct lcd_hw *lm,int16u data)
{
	lm->io_bus->write(lm->io_bus,1,(int8u)data);
}

void lm6029_reset(struct lcd_hw *lm)
{
	gpio_set(lm->io_bus->cs);
	gpio_set(lm->io_bus->wr);
	gpio_set(lm->io_bus->rd);
	lm->write_data(lm,0xFF);
	gpio_set(lm->gpio_rst);
	delay_us(20);
	gpio_reset(lm->gpio_rst);
	delay_us(20);
	gpio_set(lm->gpio_rst);
	delay_us(20);
}

void lm6029_set_column(struct lcd_hw *lm,int8u column)
{
	lm->write_cmd(lm,0x10 | ((column)>>4));
	lm->write_cmd(lm,column & 0x0F);
}

void lm6029_set_page(struct lcd_hw *lm,int8u page)
{
	lm->write_cmd(lm,0xB0+((page)&0x0F));
}


void lm6029_clear(struct lcd_hw *lm,int8u Mask){
	int8u i,j;
	for(i=0;i<(LCD_Y_MAX+1)/8;i++){
		lm6029_set_page(lm,i);
		lm6029_set_column(lm,0);
		for (j=0;j<(LCD_X_MAX+1);j++){
			lm->write_data(lm,Mask);
		}
	}
}

void lm6029_init(struct lcd_hw *lm)
{
	gpio_init(lm->gpio_rst,GPIO_Mode_Out_PP);
	gpio_init(lm->gpio_rst,GPIO_Mode_Out_PP);
	lm->io_bus->init(lm->io_bus);
		//LCD复位
	lm6029_reset(lm);
	//设置
	lm->write_cmd(lm,0xA2);
	lm->write_cmd(lm,0xA0);
	lm->write_cmd(lm,0xC8);
	lm->write_cmd(lm,0x40);
	lm->write_cmd(lm,0x26);
	delay_us(50000);
	lm->write_cmd(lm,0x2E);
	delay_us(50000);
	lm->write_cmd(lm,0x2F);
	delay_us(50000);
	lm->write_cmd(lm,0x25);
	lm->write_cmd(lm,0x81);
	lm->write_cmd(lm,0x18);
	
	//清空LCD缓冲区
	lm6029_clear(lm,0x00);
	
	//DispalyON
	lm->write_cmd(lm,0xAF);

	//初始化执行2次才能确保成功
	//LCD复位
	lm6029_reset(lm);
	//设置
	lm->write_cmd(lm,0xA2);
	lm->write_cmd(lm,0xA0);
	lm->write_cmd(lm,0xC8);
	lm->write_cmd(lm,0x40);
	lm->write_cmd(lm,0x26);
	delay_us(50000);
	lm->write_cmd(lm,0x2E);
	delay_us(50000);
	lm->write_cmd(lm,0x2F);
	delay_us(50000);
	lm->write_cmd(lm,0x25);
	lm->write_cmd(lm,0x81);
	lm->write_cmd(lm,0x18);	
		//清空LCD缓冲区
	lm6029_clear(lm,0x00);
	
	//DispalyON
	lm->write_cmd(lm,0xAF);
}

//lm6029很特殊，写入的数据是列数据,但是显存的数据是按照行来写入的。
void  lm6029_flush(struct lcd_hw *lm,void* pBuff){
	int i,j,k,data_index[8];
	const int reg_offset=lm->width*7;
	vu32* data_ptr=PTR_BITADDR(pBuff,0);
	int8u data;
	for(k=0;k<8;k++)
		data_index[k]=lm->width*k;//初始化每个位数据的起始地址
	for(i=0;i<LCD_Y_MAXPAGE+1;i++)
	{
		lm6029_set_page(lm,i);
		lm6029_set_column(lm,0);
		for (j=0;j<lm->width;j++)
		{
			data=data_ptr[data_index[0]]|(data_ptr[data_index[1]]<<1)|data_ptr[data_index[2]]<<2|(data_ptr[data_index[3]]<<3)|
					(data_ptr[data_index[4]]<<4)|(data_ptr[data_index[5]]<<5)|data_ptr[data_index[6]]<<6|(data_ptr[data_index[7]]<<7);		
			data=~data;//取反，因为内存里面0表示黑色，1表示白色
			lm->write_data(lm,data);
			for(k=0;k<8;k++)
				data_index[k]++;
		}
		for(k=0;k<8;k++)
			data_index[k]+=reg_offset;//初始化每个位数据的起始地址
	}
}
