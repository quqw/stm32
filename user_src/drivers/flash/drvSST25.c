#include "includes.h"
#include "halSPI.h"
#include "drvSST25.h"


#define W25_FAST_READ  0x0b
#define W25_PROGRAM      0x02
#define SST_AAI			0xad
#define W25_READ       0x03
#define W25_READ_STATUS  0x05
#define W25_WRITE_STATUS  0x01
#define SST25_GET_ID      0x90
#define W25_GET_ID      0x9f
#define W25_WRITE_DISABLE 0x04
#define W25_WRITE_ENABLE 0x06
#define W25_ERASE_CHIP   0xc7
#define W25_ERASE_SECTOR   0x20
#define W25_ERASE_BLOCK   0xD8

#define W25_BUSY 0x01


static SPIBUS _sstSPI;
 
 
static void _SST25PinConfig(void* p)
{
//    GPIO_InitTypeDef  gpio_init;
//    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
//	gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
//    //spi2:miso
//    gpio_init.GPIO_Pin  = GPIO_Pin_14;
//    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOB, &gpio_init);
//
//    //spi2:nss,clk,mosi
//    gpio_init.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
//    gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB, &gpio_init);
	
//	//cs
//    gpio_init.GPIO_Pin  = GPIO_Pin_12;
//    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOB, &gpio_init);
 }	
	
void drv25Config(void)
{
	//_sstSPI.port=SPI2;
	//_sstSPI.cs.gpio=0;//GPIOB;
	//_sstSPI.cs.pin=GPIO_Pin_12;
	_sstSPI.cs.gpio=GPIOB;
	_sstSPI.cs.pin=GPIO_Pin_12;
	_sstSPI.cs.rcc=RCC_APB2Periph_GPIOB;
	_sstSPI.si.gpio=GPIOB;
	_sstSPI.si.pin=GPIO_Pin_15;
	_sstSPI.cs.rcc=RCC_APB2Periph_GPIOB;
	_sstSPI.so.gpio=GPIOB;
	_sstSPI.so.pin=GPIO_Pin_14;
	_sstSPI.cs.rcc=RCC_APB2Periph_GPIOB;
	_sstSPI.clk.gpio=GPIOB;
	_sstSPI.clk.pin=GPIO_Pin_13;	
	_sstSPI.cs.rcc=RCC_APB2Periph_GPIOB;
	hal_SPIConfig(&_sstSPI,NULL,NULL,NULL);
}
	


void drv25Init(void)
{
	int8u stat;
    
    hal_SPIInit(&_sstSPI,NULL);
    drv25WriteDisable();//WRDI���ڱ�֤SSTоƬ�˳�AAIдģʽ
	stat=drv25ReadStatus();
	
	if(stat&0xbc)
		drv25WriteStatus(stat&0x43);//�Ᵽ��

}
void drv25WriteDisable(void)
{
	while(W25_BUSY & drv25ReadStatus());//��æ
	hal_SPISelect(&_sstSPI);;
	hal_SPIWrite(&_sstSPI,W25_WRITE_DISABLE);
	hal_SPIDeselect(&_sstSPI);;
}

void drv25WriteEnable(void)
{
	while(W25_BUSY & drv25ReadStatus());//��æ
	hal_SPISelect(&_sstSPI);;
	hal_SPIWrite(&_sstSPI,W25_WRITE_ENABLE);
	hal_SPIDeselect(&_sstSPI);;
}


int8u drv25ReadStatus(void)
{
  int8u ret;
  hal_SPISelect(&_sstSPI);;
  hal_SPIWrite(&_sstSPI,W25_READ_STATUS);
  ret= hal_SPIRead(&_sstSPI);
  hal_SPIDeselect(&_sstSPI);;
  return ret;
}

void drv25WriteStatus(int8u state)
{
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
  drv25WriteEnable();
  hal_SPISelect(&_sstSPI);;
  hal_SPIWrite(&_sstSPI,W25_WRITE_STATUS);
  hal_SPIWrite(&_sstSPI,state);
  hal_SPIDeselect(&_sstSPI);;
}

void drv25EraseChip(void)
{
  drv25WriteEnable();
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
  hal_SPISelect(&_sstSPI);;
  hal_SPIWrite(&_sstSPI,W25_ERASE_CHIP);
  hal_SPIDeselect(&_sstSPI);;
}

void drv25EraseSector(int32u sector)
{
	int32u addr=sector*W25_SECTOR_LEN;
  drv25WriteEnable();
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
  hal_SPISelect(&_sstSPI);;
  hal_SPIWrite(&_sstSPI,W25_ERASE_SECTOR);
  hal_SPIWrite(&_sstSPI,(int8u)(addr>>16));
  hal_SPIWrite(&_sstSPI,(int8u)(addr>>8));
  hal_SPIWrite(&_sstSPI,(int8u)addr);
  hal_SPIDeselect(&_sstSPI);;
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����  
}


void drv25EraseBlock(int32u block)
{
	int32u addr=block*W25_BLOCK_LEN;
  drv25WriteEnable();
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
  hal_SPISelect(&_sstSPI);;
  hal_SPIWrite(&_sstSPI,W25_ERASE_BLOCK);
  hal_SPIWrite(&_sstSPI,(int8u)(addr>>16));
  hal_SPIWrite(&_sstSPI,(int8u)(addr>>8));
  hal_SPIWrite(&_sstSPI,(int8u)addr);
  hal_SPIDeselect(&_sstSPI);;
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����  
}

void drvSST25GetID(int8u* pMId,int16u* pDId)
{
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
  hal_SPISelect(&_sstSPI);;			 
  hal_SPIWrite(&_sstSPI,SST25_GET_ID);
  hal_SPIWrite(&_sstSPI,0);
  hal_SPIWrite(&_sstSPI,0);
  hal_SPIWrite(&_sstSPI,0);
  *pMId=hal_SPIRead(&_sstSPI);
  *((int8u*)pDId+1)=hal_SPIRead(&_sstSPI);
  *pDId|=hal_SPIRead(&_sstSPI);
  hal_SPIDeselect(&_sstSPI);;
}


void drvW25GetID(int8u* pMId,int16u* pDId)
{
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
  hal_SPISelect(&_sstSPI);;			 
  hal_SPIWrite(&_sstSPI,W25_GET_ID);
  *pMId=hal_SPIRead(&_sstSPI);
  *((int8u*)pDId+1)=hal_SPIRead(&_sstSPI);
  *pDId|=hal_SPIRead(&_sstSPI);
  hal_SPIDeselect(&_sstSPI);;
}


void drvW25ReadBytes(int32u addr,void* pDest,int32u len)
{
  int32u i;
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
  hal_SPISelect(&_sstSPI);;
  hal_SPIWrite(&_sstSPI,W25_READ);
  //hal_SPIWrite(&_sstSPI,W25_FAST_READ);
  hal_SPIWrite(&_sstSPI,(int8u)(addr>>16));
  hal_SPIWrite(&_sstSPI,(int8u)(addr>>8));
  hal_SPIWrite(&_sstSPI,(int8u)addr);
  for(i=0;i<len;i++)
    ((int8u*)pDest)[i]=hal_SPIRead(&_sstSPI);
  hal_SPIDeselect(&_sstSPI);;
}

//ÿҳ256�ֽ�
//д�볬��һҳ��ӱ�ҳͷ��ʼѭ��д������Ҫ���⴦��
//д��һҳ��ȴ�busy��־λ��λΪ0�ټ���д����һҳ��
void drvW25WriteBytes(int32u addr,const void* pSrc,int32u len)
{
  int16u i,j=0,page=0,incomplete_page_byte=0,dataIndex=0;
  addr=0xffffff&addr;
  if(addr+len>W25_FLASH_LEN)
  	return;
  if(len==0)
		return;
  if(addr%W25_PAGE_LEN)//��ʼ������ҳ
  {
    incomplete_page_byte=W25_PAGE_LEN-addr%W25_PAGE_LEN;
    j=incomplete_page_byte>len?len:incomplete_page_byte;//��Ҫд��Ĳ�����ҳ���ݳ���
    drv25WriteEnable();
    while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
    hal_SPISelect(&_sstSPI);;
    hal_SPIWrite(&_sstSPI,W25_PROGRAM);
    hal_SPIWrite(&_sstSPI,(int8u)(addr>>16));
    hal_SPIWrite(&_sstSPI,(int8u)(addr>>8));
    hal_SPIWrite(&_sstSPI,(int8u)addr);
    for(i=0;i<j;i++)
      hal_SPIWrite(&_sstSPI,((int8u*)pSrc)[dataIndex++]);
    hal_SPIDeselect(&_sstSPI);;
    if(incomplete_page_byte>=len)//д��
      return;
  }
  addr+=incomplete_page_byte;
  page=(len-incomplete_page_byte)/W25_PAGE_LEN;//������ҳ����
  for(j=0;j<page;j++)//д����ҳ
  {
  	drv25WriteEnable();
    while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
    hal_SPISelect(&_sstSPI);;
    hal_SPIWrite(&_sstSPI,W25_PROGRAM);
    hal_SPIWrite(&_sstSPI,(int8u)(addr>>16));
    hal_SPIWrite(&_sstSPI,(int8u)(addr>>8));
    hal_SPIWrite(&_sstSPI,(int8u)addr);
    for(i=0;i<256;i++)
      hal_SPIWrite(&_sstSPI,((int8u*)pSrc)[dataIndex++]);
    hal_SPIDeselect(&_sstSPI);;
    addr+=256;
  }
  incomplete_page_byte=len-incomplete_page_byte-j*W25_PAGE_LEN;//ʣ��Ĳ�����ҳ
  if(incomplete_page_byte)
  {
  	drv25WriteEnable();
    while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
    hal_SPISelect(&_sstSPI);;
    hal_SPIWrite(&_sstSPI,W25_PROGRAM);
    hal_SPIWrite(&_sstSPI,(int8u)(addr>>16));
    hal_SPIWrite(&_sstSPI,(int8u)(addr>>8));
    hal_SPIWrite(&_sstSPI,(int8u)addr);
    for(i=0;i<incomplete_page_byte;i++)
      hal_SPIWrite(&_sstSPI,((int8u*)pSrc)[dataIndex++]);
    hal_SPIDeselect(&_sstSPI);; 
  }
  while(W25_BUSY & drv25ReadStatus());//���BUSY��־�ȴ�д�����
}


/**********************************
  SST25ByteProgram  дһ���ֽ�*ע���ڴ�ǰҪ����ȡ��д����,ʵ��дӦʹ��AAI,�˺�����AAI�е��ã�����д�������ֽ�
**********************************/
void SST25ByteProgram(int32u addr,int8u byte)
{
	int8u ad[3]={0,0,0};
	ad[0]=(addr>>16)&0xff;
	ad[1]=(addr>>8)&0xff;
	ad[2]=addr&0xff;
	drv25WriteEnable();
	while(W25_BUSY & drv25ReadStatus());//��æ
	hal_SPISelect(&_sstSPI);;
	hal_SPIWrite(&_sstSPI,0x02);
	hal_SPIWrite(&_sstSPI,ad[0]);
	hal_SPIWrite(&_sstSPI,ad[1]);
	hal_SPIWrite(&_sstSPI,ad[2]);
	hal_SPIWrite(&_sstSPI,byte);
	hal_SPIDeselect(&_sstSPI);;
}
/***********************************
  SST25Write д����ֽ�
***********************************/
void drvSST25WriteBytes(int32u addr,const void* p_data,int32u len){
	int8u ad[3]={0,0,0};
	int8u* p=(int8u*)p_data;
	int32u cnt=len;
	int32u address=addr;
	if(cnt==0)
		return;
	if(address+cnt>W25_FLASH_LEN)
  		return;	
	if(address%2==1)//aaiֻ�����ż��ַ��ʼд
	{
		SST25ByteProgram(addr,*p++);
		address++;
		cnt--;
	}
	//����У�дʣ�ಿ��
	if(cnt==1)	//cnt<2��Ӧʹ����ͨ���ֽڷ�ʽ
	{
		SST25ByteProgram(addr,*p);
		return;
	}	
	else if(cnt>1)
	{
		//aaiд��ͷ2�ֽ�
		drv25WriteEnable();
		while(W25_BUSY & drv25ReadStatus());//��æ
		ad[2]=(address>>16)&0xff;
		ad[1]=(address>>8)&0xff;
		ad[0]=address&0xff;
		hal_SPISelect(&_sstSPI);;
		hal_SPIWrite(&_sstSPI,0xad);
		hal_SPIWrite(&_sstSPI,ad[2]);
		hal_SPIWrite(&_sstSPI,ad[1]);
		hal_SPIWrite(&_sstSPI,ad[0]);
		hal_SPIWrite(&_sstSPI,*p++);
		hal_SPIWrite(&_sstSPI,*p++);
		hal_SPIDeselect(&_sstSPI);;
		cnt-=2;
		
		while(W25_BUSY & drv25ReadStatus());//��æ	
		//�м��˫�ֽ�д
		for(;cnt>1;cnt-=2)
		{
			hal_SPISelect(&_sstSPI);;
			hal_SPIWrite(&_sstSPI,0xad);
			hal_SPIWrite(&_sstSPI,*p++);
			hal_SPIWrite(&_sstSPI,*p++);
			hal_SPIDeselect(&_sstSPI);;
			while(W25_BUSY & drv25ReadStatus());//��æ
		}
		drv25WriteDisable();//WRDI�����˳�AAIдģʽ	
		//��������һ���ֽ�
		if(cnt==1)
		{
			drv25WriteEnable();
			SST25ByteProgram(addr+len-1,*p);
		}	
	}
	while(W25_BUSY & drv25ReadStatus());//��æ
}

