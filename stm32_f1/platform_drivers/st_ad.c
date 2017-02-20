#include "includes.h"
#include "st_ad.h"
//#include "platform_drivers/gpio.h"
static volatile int16u _ad_value[16];
static volatile int8u _current_channel=0;
#define ADC_DR_ADDRESS ((int32u)0x4001244C)

void adc_init(struct adc_channel *adc[],int count)
{
	int i;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	  /* DMA1 channel1 configuration ----------------------------------------------*/
	//Ҫ֧�ֶ��ͨ��ת�������뿪��dma���������ݻᶪʧ
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_ad_value;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = count;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = count>1?DMA_MemoryInc_Enable:DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
	  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = count;
  ADC_Init(ADC1, &ADC_InitStructure);
	ADC_DMACmd(ADC1, ENABLE);
	
  /* ADC1 regular channel14 configuration */ 
	for(i=0;i<count;i++)
	{
		adc[i]->index=i;
		adc[i]->value_address=_ad_value+i;
		ADC_RegularChannelConfig(ADC1, adc[i]->channel, i+1, ADC_SampleTime_55Cycles5);
		gpio_init(adc[i]->pin,GPIO_Mode_AIN);
	}

	ADC_Cmd(ADC1, ENABLE);
	  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



int16u adc_get_value(struct adc_channel *adc)
{
	return *(adc->value_address);
}


// void ADC1_2_IRQHandler(void)
// {
// 	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
// 	
// 	_ad_value[_current_channel]=ADC1->DR;
// 	_current_channel++;
// 	_current_channel&=0x0f;
// 	_ad_value[_current_channel]=ADC1->DR;
// 		_current_channel++;
// 	_current_channel&=0x0f;
// }


