//===============================================
//				"spiUnit.c"
//	Last update : 26/01/16
//===============================================

#include <avr/io.h>
#include "spiUnit.h"
#include "registers.h"

void TSpi::setup (uint8_t order)
{
	// set ports
	_SCK = 1;
	_MOSI = 1;
	_MISO = 0;
	_SS = 1;
	
	_MSTR = 1;
	if (order==SPI_LSB)
	_DORD = 1;
	else
	_DORD = 0;
	setClock(SPI_CLK_8MHZ);
	// SPI enable
	_SPE = 1;
}

void TSpi::setMode (uint8_t mode)
{
	switch (mode)
	{
		case MODE3:	_CPOL=1; _CPHA=1; break;
		case MODE2:	_CPOL=1; _CPHA=0; break;
		case MODE1:	_CPOL=0; _CPHA=1; break;
		case MODE0:
		default:
		_CPOL=0; _CPHA=0; break;
	}
}

void TSpi::setClock (uint8_t clk)
{
	switch (clk)
	{
		case SPI_CLK_4MHZ:		_SPI2X=0; _SPR1=0; _SPR0=0;	break;
		
		case SPI_CLK_250KHZ:	_SPI2X=0; _SPR1=1; _SPR0=0;	break;
		case SPI_CLK_125KHZ:	_SPI2X=0; _SPR1=1; _SPR0=1;	break;
		
		case SPI_CLK_8MHZ:		_SPI2X=1; _SPR1=0; _SPR0=0;	break;
		case SPI_CLK_2MHZ:		_SPI2X=1; _SPR1=0; _SPR0=1;	break;
		case SPI_CLK_500KHZ:	_SPI2X=1; _SPR1=1; _SPR0=0;	break;
		case SPI_CLK_1MHZ:
		default:		_SPI2X=0; _SPR1=0; _SPR0=1;	break;
	}
}

uint8_t TSpi::transceiver (uint8_t data)
{
	_SPIF = 0;
	SPDR = data;

	while(!_SPIF);

	return (SPDR);
}

void TSpi::end ()
{
	// SPI disable
	_SPE = 0;
}

//===============================================
//			End file of "spiUnit.c"
//===============================================

TSpi spi;