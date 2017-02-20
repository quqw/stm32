#include "includes.h"
#include "platform/io8080.h"
#include "platform/gpio_bus.h"

/*模拟IO口*/
void simulate_i8080_bus_init(struct io8080_bus *io)
{
	gpio_bus_init(io->data_bus,GPIO_Mode_Out_PP);
	gpio_init(io->rd,GPIO_Mode_Out_PP);
	gpio_init(io->wr,GPIO_Mode_Out_PP);
	gpio_init(io->cs,GPIO_Mode_Out_PP);
	gpio_init(io->address0,GPIO_Mode_Out_PP);
	gpio_set(io->cs);
	gpio_set(io->wr);
	gpio_set(io->rd);
}

void simulate_i8080_bus_write(struct io8080_bus *io,int32u address,int16u dat)
{
  address?gpio_set(io->address0):gpio_reset(io->address0);
	gpio_set(io->rd);
	gpio_reset(io->cs);
	gpio_reset(io->wr);
  gpio_bus_set_data(io->data_bus,dat);
	gpio_set(io->wr);
	gpio_set(io->cs);
}

int16u simulate_i8080_bus_read(struct io8080_bus *io,int32u address)
{
	return 0;
}

/*FSMC*/
void fsmc_i8080_bus_init(struct io8080_bus *bus)
{
	  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
    GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_AFIO, ENABLE);

	//PD.11(A16 (RS)) 需要根据实际修改
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  /* Set PD.04(NOE), PD.05(NWE)  PD.7(NE1 (LCD/CS)) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//需要根据偏移修改
  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*-- FSMC Configuration ------------------------------------------------------*/
  /* FSMC_Bank1_NORSRAM1 timing configuration */
  p.FSMC_AddressSetupTime = 1;//1;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 5;
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_B;
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;//FSMC_MemoryType_NOR;//FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  

  /* Enable FSMC_Bank1_NORSRAM1 */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

// void fsmc_i8080_bus_init(struct io8080_bus *bus)
    // {
      // FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  // FSMC_NORSRAMTimingInitTypeDef  p;
        // GPIO_InitTypeDef GPIO_InitStructure;

  // /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  // RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         // RCC_APB2Periph_AFIO, ENABLE);

  // /* Set PD.04(NOE), PD.05(NWE) PD.11(A16 (RS)) PD.7(NE1 (LCD/CS)) as alternate function push pull */
  // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11 ;
  // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  // GPIO_Init(GPIOD, &GPIO_InitStructure);

  // /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     // PE.14(D11) as alternate function push pull */
  // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                // GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 ;
  // GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    // /*-- FSMC Configuration ------------------------------------------------------*/
  // /* FSMC_Bank1_NORSRAM1 timing configuration */
  // p.FSMC_AddressSetupTime = 1;//1;
  // p.FSMC_AddressHoldTime = 0;
  // p.FSMC_DataSetupTime = 5;
  // p.FSMC_BusTurnAroundDuration = 0;
  // p.FSMC_CLKDivision = 0;
  // p.FSMC_DataLatency = 0;
  // p.FSMC_AccessMode = FSMC_AccessMode_B;
  // FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  // FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//FSMC_DataAddressMux_Disable;
  // FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;//FSMC_MemoryType_NOR;//FSMC_MemoryType_SRAM;
  // FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  // FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  // FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  // FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  // FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  // FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  // FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  // FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  // FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  // FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  // FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  // FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  

  // /* Enable FSMC_Bank1_NORSRAM1 */
  // FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
    // }
		
//16位模式只使用8根数据线，数据线低8位没有使能FSMC，所以写16bits实际上只操作8bits
void fsmc_i8080_bus_write(struct io8080_bus *bus,int32u address,int16u dat)//地址只有0和1两种
{
	int32u mac_addr;
	if(bus->data_width==16)
		mac_addr=0x60000000+(address<<(bus->addr_x+1));//因为是16位模式，第一个位是无效的，要让地址线有动作地址就要多偏移一位
	else
		mac_addr=0x60000000+(address<<bus->addr_x);
    *(volatile int16u*)mac_addr=(dat<<bus->data_offset);
    // if(address==0)
        // (*(volatile int16u*)(0x60000000))=(int16u)(dat<<4);
    // else
        // (*(volatile int16u*)(0x60020000))=dat<<4;
	
}
int16u fsmc_i8080_bus_read(struct io8080_bus *bus,int32u address)
{
		int32u mac_addr;
	if(bus->data_width==16)
		mac_addr=0x60000000+(address<<(bus->addr_x+1));//因为是16位模式，第一个位是无效的
	else
		mac_addr=0x60000000+(address<<bus->addr_x);
	return (*(volatile int16u*)mac_addr)>>(bus->data_offset);
}
