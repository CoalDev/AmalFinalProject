//===============================================
//			"spiUnit.h"
//	Last update : 30/01/16
//	User: Yaron Dori
//===============================================
#ifndef	_SPI_UNIT_H_
#define	_SPI_UNIT_H_

#include <avr/pgmspace.h>
#include "registers.h"

#ifndef _SPI_
#define _SPI_ 1
#define _SCK	PDB5
#define _MISO	PDB4
#define _MOSI	PDB3
#define _SS		PDB2
#endif


#define SPI_LSB 0
#define SPI_MSB 1

#define MODE0	0
#define MODE1	1
#define MODE2	2
#define MODE3	3


#define SPI_CLK_8MHZ	0
#define SPI_CLK_4MHZ	1
#define SPI_CLK_2MHZ	2
#define SPI_CLK_1MHZ	3
#define SPI_CLK_500KHZ	4
#define SPI_CLK_250KHZ	5
#define SPI_CLK_125KHZ	6

class TSpi{
	public:
	void setup (uint8_t order);
	void setClock (uint8_t clk);
	void setMode (uint8_t mode);
	uint8_t transceiver (uint8_t data);
	void end ();
};

extern TSpi spi;

#endif
//===============================================
//			End file of "spiUnit.h"
//===============================================
