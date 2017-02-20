#ifndef __PLATFORM_AD_H
#define __PLATFORM_AD_H

struct adc_channel
{
	struct gpio *pin;
	volatile int16u* value_address;
	int8u channel;
	int8u index;
};


/*
channel=1,2,3,4....
*/
#define DECLARE_ADC_CHANNEL(name,channel,x) DECLARE_GPIO(adc_##name,x);\
																						struct adc_channel adc_channel_##name={&gpio_adc_##name,0,channel,0}

void adc_init(struct adc_channel *adc[],int count);
int16u adc_get_value(struct adc_channel *adc);






#endif

