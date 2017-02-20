#ifndef ASSIST_H
#define ASSIST_H
#include "common_types.h"
#define CRC8_PRESET	0xff

#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define ABS(x)	((x)>0?(x):-1*(x))
#define ATOMIC_SET(varName,varValue)      {__disable_irq(); varName=(varValue);__enable_irq();}
#define ATOMIC_ADD(varName,varValue)      {__disable_irq(); varName+=(varValue);__enable_irq();}
#define ATOMIC_DEC(varName,varValue)      {__disable_irq(); varName-=(varValue);__enable_irq();}
#define ATOMIC_OR(varName,varValue)       {__disable_irq(); varName|=(varValue);__enable_irq();}
#define ATOMIC_XOR(varName,varValue)      {__disable_irq(); varName^=(varValue);__enable_irq();}
#define ATOMIC_AND(varName,varValue)      {__disable_irq(); varName&=(varValue);__enable_irq();}
#define ATOMIC_RUN(x)					{__disable_irq(); do{x}while(0);__enable_irq();}

#define BcdToBin(VAL) (((VAL)>>4)*10+((VAL)&0x0F))
#define BinToBcd(VAL) (((VAL)%10)+(((VAL)/10)<<4))

#define OSLOCK_SET(varName,varValue)	{taskDISABLE_INTERRUPTS(); varName=(varValue);taskENABLE_INTERRUPTS();}
#define OSLOCK_ADD(varName,varValue)	{taskDISABLE_INTERRUPTS(); varName+=(varValue);taskENABLE_INTERRUPTS();}
#define OSLOCK_DEC(varName,varValue)	{taskDISABLE_INTERRUPTS(); varName-=(varValue);taskENABLE_INTERRUPTS();}
#define OSLOCK_OR(varName,varValue)		{taskDISABLE_INTERRUPTS(); varName|=(varValue);taskENABLE_INTERRUPTS();}
#define OSLOCK_XOR(varName,varValue)	{taskDISABLE_INTERRUPTS(); varName^=(varValue);taskENABLE_INTERRUPTS();}
#define OSLOCK_RUN(x)					{taskDISABLE_INTERRUPTS();  do{x}while(0);taskENABLE_INTERRUPTS();}


///////////////////////////////////STM32 Register Map////////////////////////////
//#define PTR_BITOFFSET(ptr)          (((u32)(ptr))&0xFFFFF)
//#define PTR_BITADDR(ptr,bitIndex)   ((vu32*)(SRAM_BB_BASE+PTR_BITOFFSET(ptr)*32+(bitIndex)*4))

//#define DISABLE_ISR(IRQ)    NVIC->ICER[IRQ >> 0x05] =(u32)0x01 << (IRQ & 0x1F);
//#define ENABLE_ISR(IRQ)     NVIC->ISER[IRQ >> 0x05] =(u32)0x01 << (IRQ & 0x1F);

//#define APB1PERIPH_OFFSET       0
//#define APB2PERIPH_OFFSET       0x10000
//#define AHBPERIPH_OFFSET        0x20000

//#define AFIO_OFFSET             (APB2PERIPH_OFFSET + 0x0000)
//#define EXTI_OFFSET             (APB2PERIPH_OFFSET + 0x0400)
//#define GPIOA_OFFSET            (APB2PERIPH_OFFSET + 0x0800)
//#define GPIOB_OFFSET            (APB2PERIPH_OFFSET + 0x0C00)
//#define GPIOC_OFFSET            (APB2PERIPH_OFFSET + 0x1000)
//#define GPIOD_OFFSET            (APB2PERIPH_OFFSET + 0x1400)
//#define GPIOE_OFFSET            (APB2PERIPH_OFFSET + 0x1800)
//#define ADC1_OFFSET             (APB2PERIPH_OFFSET + 0x2400)
//#define ADC2_OFFSET             (APB2PERIPH_OFFSET + 0x2800)
//#define TIM1_OFFSET             (APB2PERIPH_OFFSET + 0x2C00)
//#define SPI1_OFFSET             (APB2PERIPH_OFFSET + 0x3000)
//#define USART1_OFFSET           (APB2PERIPH_OFFSET + 0x3800)

//#define TIM2_OFFSET             (APB1PERIPH_OFFSET + 0x0000)
//#define TIM3_OFFSET             (APB1PERIPH_OFFSET + 0x0400)
//#define TIM4_OFFSET             (APB1PERIPH_OFFSET + 0x0800)
//#define RTC_OFFSET              (APB1PERIPH_OFFSET + 0x2800)
//#define WWDG_OFFSET             (APB1PERIPH_OFFSET + 0x2C00)
//#define IWDG_OFFSET             (APB1PERIPH_OFFSET + 0x3000)
//#define SPI2_OFFSET             (APB1PERIPH_OFFSET + 0x3800)
//#define USART2_OFFSET           (APB1PERIPH_OFFSET + 0x4400)
//#define USART3_OFFSET           (APB1PERIPH_OFFSET + 0x4800)
//#define I2C1_OFFSET             (APB1PERIPH_OFFSET + 0x5400)
//#define I2C2_OFFSET             (APB1PERIPH_OFFSET + 0x5800)
//#define CAN_OFFSET              (APB1PERIPH_OFFSET + 0x6400)
//#define BKP_OFFSET              (APB1PERIPH_OFFSET + 0x6C00)
//#define PWR_OFFSET              (APB1PERIPH_OFFSET + 0x7000)

//#define DMA_OFFSET              (AHBPERIPH_OFFSET + 0x0000)
//#define DMA_Channel1_OFFSET     (AHBPERIPH_OFFSET + 0x0008)
//#define DMA_Channel2_OFFSET     (AHBPERIPH_OFFSET + 0x001C)
//#define DMA_Channel3_OFFSET     (AHBPERIPH_OFFSET + 0x0030)
//#define DMA_Channel4_OFFSET     (AHBPERIPH_OFFSET + 0x0044)
//#define DMA_Channel5_OFFSET     (AHBPERIPH_OFFSET + 0x0058)
//#define DMA_Channel6_OFFSET     (AHBPERIPH_OFFSET + 0x006C)
//#define DMA_Channel7_OFFSET     (AHBPERIPH_OFFSET + 0x0080)
//#define RCC_OFFSET              (AHBPERIPH_OFFSET + 0x1000)

//#define  GPIOA_ODR_OFFSET (GPIOA_OFFSET+0x0C)
//#define  GPIOA_IDR_OFFSET (GPIOA_OFFSET+0x08)
//#define  GPIOB_ODR_OFFSET (GPIOB_OFFSET+0x0C)
//#define  GPIOB_IDR_OFFSET (GPIOB_OFFSET+0x08)
//#define  GPIOC_ODR_OFFSET (GPIOC_OFFSET+0x0C)
//#define  GPIOC_IDR_OFFSET (GPIOC_OFFSET+0x08)
//#define  GPIOD_ODR_OFFSET (GPIOD_OFFSET+0x0C)
//#define  GPIOD_IDR_OFFSET (GPIOD_OFFSET+0x08)
//#define  GPIOE_ODR_OFFSET (GPIOE_OFFSET+0x0C)
//#define  GPIOE_IDR_OFFSET (GPIOE_OFFSET+0x08)

//#define GPIOA_ODR_BIT(index)	(*(vu32*)(PERIPH_BB_BASE + (GPIOA_ODR_OFFSET * 32) + (index * 4)))
//#define GPIOB_ODR_BIT(index)	(*(vu32*)(PERIPH_BB_BASE + (GPIOB_ODR_OFFSET * 32) + (index * 4)))
//#define GPIOC_ODR_BIT(index)	(*(vu32*)(PERIPH_BB_BASE + (GPIOC_ODR_OFFSET * 32) + (index * 4)))
//#define GPIOD_ODR_BIT(index)	(*(vu32*)(PERIPH_BB_BASE + (GPIOD_ODR_OFFSET * 32) + (index * 4)))
//#define GPIOE_ODR_BIT(index)	(*(vu32*)(PERIPH_BB_BASE + (GPIOE_ODR_OFFSET * 32) + (index * 4)))

//#define GPIOA_IDR_BIT(index)	(*(vuc32*)(PERIPH_BB_BASE + (GPIOA_IDR_OFFSET * 32) + (index * 4)))
//#define GPIOB_IDR_BIT(index)	(*(vuc32*)(PERIPH_BB_BASE + (GPIOB_IDR_OFFSET * 32) + (index * 4)))
//#define GPIOC_IDR_BIT(index)	(*(vuc32*)(PERIPH_BB_BASE + (GPIOC_IDR_OFFSET * 32) + (index * 4)))
//#define GPIOD_IDR_BIT(index)	(*(vuc32*)(PERIPH_BB_BASE + (GPIOD_IDR_OFFSET * 32) + (index * 4)))
//#define GPIOE_IDR_BIT(index)	(*(vuc32*)(PERIPH_BB_BASE + (GPIOE_IDR_OFFSET * 32) + (index * 4)))

//#define EXTI_IMR_OFFSET		(EXTI_OFFSET+0x00)
//#define EXTI_EMR_OFFSET		(EXTI_OFFSET+0x04)
//#define EXTI_RTSR_OFFSET	(EXTI_OFFSET+0x08)
//#define EXTI_FTSR_OFFSET	(EXTI_OFFSET+0x0C)
//#define EXTI_SWIER_OFFSET	(EXTI_OFFSET+0x10)
//#define EXTI_PR_OFFSET		(EXTI_OFFSET+0x14)

//#define EXTI_IMR_BIT(index)		(*(vu32*)(PERIPH_BB_BASE + (EXTI_IMR_OFFSET * 32) + ((index) * 4)))
//#define EXTI_EMR_BIT(index)		(*(vu32*)(PERIPH_BB_BASE + (EXTI_EMR_OFFSET * 32) + ((index) * 4)))
//#define EXTI_RTSR_BIT(index)	(*(vu32*)(PERIPH_BB_BASE + (EXTI_RTSR_OFFSET * 32) + ((index) * 4)))
//#define EXTI_FTSR_BIT(index)	(*(vu32*)(PERIPH_BB_BASE + (EXTI_FTSR_OFFSET * 32) + ((index) * 4)))
//#define EXTI_SWIER_BIT(index)	(*(vu32*)(PERIPH_BB_BASE + (EXTI_SWIER_OFFSET * 32) + ((index) * 4)))
//#define EXTI_PR_BIT(index)		(*(vu32*)(PERIPH_BB_BASE + (EXTI_PR_OFFSET * 32) + ((index) * 4)))

//#define TIM1_CR1_OFFSET	(TIM1_OFFSET+0)
//#define TIM2_CR1_OFFSET	(TIM2_OFFSET+0)
//#define TIM3_CR1_OFFSET	(TIM3_OFFSET+0)
//#define TIM4_CR1_OFFSET	(TIM4_OFFSET+0)

//#define TIM1_CR1_BIT(index)		(*(vu32*)(PERIPH_BB_BASE + (TIM1_CR1_OFFSET * 32) + ((index) * 4)))
//#define TIM2_CR1_BIT(index)		(*(vu32*)(PERIPH_BB_BASE + (TIM2_CR1_OFFSET * 32) + ((index) * 4)))
//#define TIM3_CR1_BIT(index)		(*(vu32*)(PERIPH_BB_BASE + (TIM3_CR1_OFFSET * 32) + ((index) * 4)))
//#define TIM4_CR1_BIT(index)		(*(vu32*)(PERIPH_BB_BASE + (TIM4_CR1_OFFSET * 32) + ((index) * 4)))

//#define TIM1_CR1_CEN		TIM1_CR1_BIT(0)
//#define TIM2_CR1_CEN		TIM2_CR1_BIT(0)
//#define TIM3_CR1_CEN		TIM3_CR1_BIT(0)
//#define TIM4_CR1_CEN		TIM4_CR1_BIT(0)


//#define USART1_CR1_OFFSET	(USART1_OFFSET+0x0C)
//#define USART2_CR1_OFFSET	(USART2_OFFSET+0x0C)
//#define USART3_CR1_OFFSET	(USART3_OFFSET+0x0C)

//#define USART1_CR2_OFFSET	(USART1_OFFSET+0x10)
//#define USART2_CR2_OFFSET	(USART2_OFFSET+0x10)
//#define USART3_CR2_OFFSET	(USART3_OFFSET+0x10)

//#define USART1_CR1_UE		(*(vu32*)(PERIPH_BB_BASE + (USART1_CR1_OFFSET * 32) + ((13) * 4)))
//#define USART1_CR1_PS		(*(vu32*)(PERIPH_BB_BASE + (USART1_CR1_OFFSET * 32) + ((9) * 4)))
//#define USART1_CR1_TXEIE	(*(vu32*)(PERIPH_BB_BASE + (USART1_CR1_OFFSET * 32) + ((7) * 4)))
//#define USART1_CR1_TCIE		(*(vu32*)(PERIPH_BB_BASE + (USART1_CR1_OFFSET * 32) + ((6) * 4)))
//#define USART1_CR1_TE		(*(vu32*)(PERIPH_BB_BASE + (USART1_CR1_OFFSET * 32) + ((3) * 4)))
//#define USART1_CR1_RE		(*(vu32*)(PERIPH_BB_BASE + (USART1_CR1_OFFSET * 32) + ((2) * 4)))
//#define USART1_CR1_SBK		(*(vu32*)(PERIPH_BB_BASE + (USART1_CR1_OFFSET * 32) + ((0) * 4)))

//#define USART2_CR1_UE		(*(vu32*)(PERIPH_BB_BASE + (USART2_CR1_OFFSET * 32) + ((13) * 4)))
//#define USART2_CR1_PS		(*(vu32*)(PERIPH_BB_BASE + (USART2_CR1_OFFSET * 32) + ((9) * 4)))
//#define USART2_CR1_TXEIE	(*(vu32*)(PERIPH_BB_BASE + (USART2_CR1_OFFSET * 32) + ((7) * 4)))
//#define USART2_CR1_TCIE		(*(vu32*)(PERIPH_BB_BASE + (USART2_CR1_OFFSET * 32) + ((6) * 4)))
//#define USART2_CR1_TE		(*(vu32*)(PERIPH_BB_BASE + (USART2_CR1_OFFSET * 32) + ((3) * 4)))
//#define USART2_CR1_RE		(*(vu32*)(PERIPH_BB_BASE + (USART2_CR1_OFFSET * 32) + ((2) * 4)))
//#define USART2_CR1_SBK		(*(vu32*)(PERIPH_BB_BASE + (USART2_CR1_OFFSET * 32) + ((0) * 4)))

//#define USART3_CR1_UE		(*(vu32*)(PERIPH_BB_BASE + (USART3_CR1_OFFSET * 32) + ((13) * 4)))
//#define USART3_CR1_PS		(*(vu32*)(PERIPH_BB_BASE + (USART3_CR1_OFFSET * 32) + ((9) * 4)))
//#define USART3_CR1_TXEIE	(*(vu32*)(PERIPH_BB_BASE + (USART3_CR1_OFFSET * 32) + ((7) * 4)))
//#define USART3_CR1_TCIE		(*(vu32*)(PERIPH_BB_BASE + (USART3_CR1_OFFSET * 32) + ((6) * 4)))
//#define USART3_CR1_TE		(*(vu32*)(PERIPH_BB_BASE + (USART3_CR1_OFFSET * 32) + ((3) * 4)))
//#define USART3_CR1_RE		(*(vu32*)(PERIPH_BB_BASE + (USART3_CR1_OFFSET * 32) + ((2) * 4)))
//#define USART3_CR1_SBK		(*(vu32*)(PERIPH_BB_BASE + (USART3_CR1_OFFSET * 32) + ((0) * 4)))



int16u crc16_ibm(void* data, int len);
int16u crc16_ccitt(void *pBuff, int32u len) ;
int16u crc16_ccitt_l(int8u *pBuff, int32u len) ;
int16u crc16_typea(int8u* pBuff, int32u dwLen);
void bin2hex(int8u* pBuff,int32u nCount,char* pTagNo);
int8u hex2byte(const char* pCh);
int32u hex2bin(const char* pTagNo,int8u* pBuff,int32u nMaxCount);
int8u reverse_bits(int8u bytIn);
int8u crc8_ccitt(int8u preset,void * pBuff,int32u nCount);
int16u mb_crc16( int8u * data, int16u len );
BOOL parse_ip_string(string ip_str,int8u* ip);
int32u  parse_u32(String pdata,int8u size);
#endif

