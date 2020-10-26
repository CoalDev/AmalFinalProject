
 // last update 29/09/17
 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "touch.h"
#include "spiUnit.h"
#include "registers.h"
#include "TFT9341.h"


void touch::InitTypeTouch(int type)
{
	type %= 3;
	TypeTouch = type;
}


void touch::begin()
{
	PDD2 = 0;//irq
	PDD7 = 1;//cs
	T_SS = 1;
	POD2 =1;
	TypeTouch = 2;
	
}

uint16_t touch::getX()
{
	uint8_t data1, data2;
	uint16_t dx;
	T_SS = 0;
	spi.transceiver(0xD0);////
	data1 = spi.transceiver(0x00);
	data2 = spi.transceiver(0x00);
	dx = (((data1<<8)+data2)>>3) &0xFFF;
	T_SS = 1;
	return (dx);
}

uint16_t touch::getY()
{
	uint8_t data1, data2;
	uint16_t dy;
	T_SS = 0;
	spi.transceiver(0x90);/////
	data1 = spi.transceiver(0x00);
	data2 = spi.transceiver(0x00);
	dy = (((data1<<8)+data2)>>3) &0xFFF;
	T_SS = 1;
	return (dy);
}


void touch::readxy()
{
	int temp;
//	PDD2 = 1;//lock irq
	_delay_ms(10);
	spi.setClock(SPI_CLK_1MHZ);



	if (TypeTouch == 1)
	{
		_x = getX(); 
		_x = 240 - (_x - 300) / 14;
		_y = getY(); 
		_y = (_y - 270) / 10.5;
	}
	else if (TypeTouch == 0)
	{
		_x = getY(); 
		_x = (_x - 192) / 15.2;
		_y = getX(); 
		_y = (_y - 192) / 11.4;
	}
	else if (TypeTouch == 2)
	{
		_x = getX(); 
		_x = (_x - 300) / 14;
		_y = getY(); 
		_y = 320 + 20 - (_y - 270) / 10.5;
	}


	temp = _x;
	_rotation = lcd.getRotation();
	_height = lcd.getHeight();
	_width = lcd.getWidth();

	if (_rotation == 3){		
	}
	else if (_rotation == 0){
		_x =  _y;
		_y =240- temp;
	}
	else if (_rotation == 2){
		_x = 320 - _y;
		_y = temp;
	}

	else if (_rotation ==1){
		_x = 240 - _x;
		_y = 320 - _y;
	}


	
	if (_x<0) _x=0;
	
	if (_x>_width) _x = _width;
	
	if (_y<0) _y=0;
	
	if (_y>_height) _y = _height;
	
	spi.setClock(SPI_CLK_8MHZ);
}

touch lcdtouch;
