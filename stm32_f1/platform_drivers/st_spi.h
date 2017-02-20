#ifndef __PLATFORM_SPI_H
#define __PLATFORM_SPI_H
#include "platform/gpio.h"

struct spi_adapter
{
	struct gpio clk;
	struct gpio cs;
	struct gpio si;
	struct gpio so;
};




#endif

