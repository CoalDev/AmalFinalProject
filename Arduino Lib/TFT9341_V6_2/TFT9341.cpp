// last update 13/12/17



#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include "TFT9341.h"
#include "glcdfont.h"
#include <SPI.h>
#include "Arduino.h"
#include <string.h>


tft9341::tft9341()
{
	_x = 0;
	_y = 0;
	_width    = WIDTH;
	_height   = HEIGHT;
	_fontSize = 1;
	_rotation  = 0;
	_color = BLACK;
	_background = WHITE;
	numbera = 0;
	numberb = 0;
	number1 = 1;
	number2 = 64;
}



//send data 8-bit to spi
void tft9341::writeData(uint8_t data8)
{
	//set clockDivider to SPI_CLOCK_DIV2 by default which is 8MHz
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	_SPIF=0;
	O_DC = 1;	//DC=1;
	O_SS = 0;	//SS=0;
	
	SPDR = data8;

	while(!_SPIF);

	O_SS = 1;	//SS=1;
}

//write command 8-bit to spi
void tft9341::writeCommand(uint8_t cmd)
{
	//set clockDivider to SPI_CLOCK_DIV2 by default which is 8MHz
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	_SPIF=0;
	O_DC = 0;	//DC=0;
	O_SS = 0;	//CS=0;

	SPDR = cmd;

	while(!_SPIF);

	O_SS = 1;	//SS=1;
}

//write data 16-bit (MSB first)
void tft9341::writeData2bytes(uint8_t dataHigh,uint8_t dataLow)
{
	//set clockDivider to SPI_CLOCK_DIV2 by default which is 8MHz
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	_SPIF=0;
	O_DC = 1;	//DC=1;
	O_SS = 0;	//SS=0;
	
	SPDR = dataHigh;

	while(!_SPIF);
	_SPIF=0;
	SPDR = dataLow;

	while(!_SPIF);
	
	O_SS = 1;	//SS=1;
}


void tft9341::begin()
{
	// sets ports as outputs excepts MISO
	// set ports
	_SCK = 1;
	_MOSI = 1;
	_MISO = 0;
	_SS = 1;
	_RES = 1;
	_DC = 1;

	//spi init
	O_SS = 1;
	_SPE = 1;
	_MSTR = 1;
	_SPI2X = 1;

	//reset LCD
	O_RES = 0; //RES=0
	_delay_ms(10);
	O_RES = 1; //RES=1
	_delay_ms(10);

	writeCommand(0x11);
	_delay_ms(10);
	writeCommand(0xCF);
	writeData(0x00);
	writeData(0xc3);
	writeData(0X30);

	writeCommand(0xED);
	writeData(0x64);
	writeData(0x03);
	writeData(0X12);
	writeData(0X81);

	writeCommand(0xE8);
	writeData(0x85);
	writeData(0x10);
	writeData(0x79);

	writeCommand(0xCB);
	writeData(0x39);
	writeData(0x2C);
	writeData(0x00);
	writeData(0x34);
	writeData(0x02);

	writeCommand(0xF7);
	writeData(0x20);

	writeCommand(0xEA);
	writeData(0x00);
	writeData(0x00);

	writeCommand(0xC0);    //Power control
	writeData(0x22);   //VRH[5:0]

	writeCommand(0xC1);    //Power control
	writeData(0x11);   //SAP[2:0];BT[3:0]

	writeCommand(0xC5);    //VCM control
	writeData(0x3d);
	writeData(0x20);

	writeCommand(0xC7);    //VCM control2
	writeData(0xAA); //0xB0

	writeCommand(0x36);    // Memory Access Control
	writeData(0x68);

	writeCommand(0x3A);
	writeData(0x55);

	writeCommand(0xB1);
	writeData(0x00);
	writeData(0x13);

	writeCommand(0xB6);    // Display Function Control
	writeData(0x0A);
	writeData(0xA2);

	writeCommand(0xF6);
	writeData(0x01);
	writeData(0x30);

	writeCommand(0xF2);    // 3Gamma Function Disable
	writeData(0x00);

	writeCommand(0x26);    //Gamma curve selected
	writeData(0x01);

	writeCommand(0xE0);    //Set Gamma
	writeData(0x0F);
	writeData(0x3F);
	writeData(0x2F);
	writeData(0x0C);
	writeData(0x10);
	writeData(0x0A);
	writeData(0x53);
	writeData(0XD5);
	writeData(0x40);
	writeData(0x0A);
	writeData(0x13);
	writeData(0x03);
	writeData(0x08);
	writeData(0x03);
	writeData(0x00);

	writeCommand(0XE1);    //Set Gamma
	writeData(0x00);
	writeData(0x00);
	writeData(0x10);
	writeData(0x03);
	writeData(0x0F);
	writeData(0x05);
	writeData(0x2C);
	writeData(0xA2);
	writeData(0x3F);
	writeData(0x05);
	writeData(0x0E);
	writeData(0x0C);
	writeData(0x37);
	writeData(0x3C);
	writeData(0x0F);

	writeCommand(0x11);    //Exit Sleep
	_delay_ms(20);
	writeCommand(0x29);    //Display on
	_delay_ms(100);
	clrscr ();
}

// all display one color
void tft9341::clrscr ()
{
	int16_t i,j;
	_x = 0;
	_y = 0;
	sector(0,_width-1,0,_height-1);//320x240
	for (i=0;i<_height;i++)
	for (j=0;j<_width;j++)
	writeData2bytes(_background>>8,_background);
}

void tft9341::sector(int16_t x0, int16_t x1, int16_t y0, int16_t y1)
{
	writeCommand(CASET); // Column addr set
	writeData(x0 >> 8);
	writeData(x0 & 0xFF);     // XSTART
	writeData(x1 >> 8);
	writeData(x1 & 0xFF);     // XEND

	writeCommand(PASET); // Row addr set
	writeData(y0>>8);
	writeData(y0);     // YSTART
	writeData(y1>>8);
	writeData(y1);     // YEND

	writeCommand(RAMWR); // write to RAM
}


// Draw a character
void tft9341::putChar(char c)
{
	int8_t i,j;
	uint8_t line;
	
	if ((_x >= _width) || (_y >= _height) || ((_x + 6 * _fontSize - 1) < 0) || ((_y + 8 * _fontSize - 1) < 0))
	return;

	for (i=0; i<6; i++ )
	{
		if (i == 5)
		line = 0x0;
		else
		line = pgm_read_byte(font+(c*5)+i);
		for (j = 0; j<8; j++)
		{
			if (line & 0x1)
			{
				if (_fontSize == 1) // default size
				drawPixel(_x+i, _y+j);
				else
				fillRect(_x+(i*_fontSize), _y+(j*_fontSize), _fontSize, _fontSize);
			}
			else if (_background != _color)
			{
				switchColors();
				if (_fontSize == 1) // default size
				drawPixel(_x+i, _y+j);
				else  // big size
				fillRect(_x+i*_fontSize, _y+j*_fontSize, _fontSize, _fontSize);
				switchColors();
			}
			line >>= 1;
		}
	}
	_x += 6 * _fontSize;
}


void tft9341::write (char c)
{
	putChar(c);
}

void tft9341::writeln (char c)
{
	write (c);
	_x = 0;
	_y += 8 * _fontSize +2;
}




void tft9341::print (int16_t num, uint8_t radix)
{
	char str[17];
	itoa (num, str, radix);
	print (str);
}

void tft9341::println (int16_t num, uint8_t radix)
{
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341::println ()
{
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341::print (uint16_t num, uint8_t radix)
{
	char str[16];

	itoa (num, str, radix);
	print (str);
}

void tft9341::println (uint16_t num, uint8_t radix)
{
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341::print (uint8_t num, uint8_t radix)
{
	char str[8];

	itoa (num, str, radix);
	print (str);
}

void tft9341::println (uint8_t num, uint8_t radix)
{
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341::print (int8_t num, uint8_t radix)
{
	char str[8];

	itoa (num, str, radix);
	print (str);
}

void tft9341::println (int8_t num, uint8_t radix)
{
	print (num, radix);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void tft9341::print (char *str)
{
	uint8_t i;
	for (i=0;str[i];i++)
	write(str[i]);
}

void tft9341::println (char *str)
{
	print (str);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void tft9341::print (String txtMsg)
{
	int8_t i,temp;

	for(i=0; i<txtMsg.length(); i++){
		temp=txtMsg[i];
		if(temp <0xb0) write(temp);
	}
}


void tft9341::println (String txtMsg)
{
	print (txtMsg);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void tft9341::print (double fnum,uint8_t s)
{
	int16_t num1,num2,e;
	double fnum1;
	if (s>5) s=5;
	e = pow (10,s);
	num1 = trunc(fnum);
	fnum1 = fnum - num1;
	num2 = abs(round(fnum1*e));
	
	print (num1);
	write ('.');
	print (num2);
}

void tft9341::println (double fnum, uint8_t s)
{
	print (fnum,s);
	_x = 0;
	_y += 8 * _fontSize +2;
}


void tft9341::print (uint16_t num)
{
	char str[8];

	ltoa (num, str, DEC);
	print (str);
}


void tft9341::println (uint16_t num)
{
	print (num);
	_x = 0;
	_y += 8 * _fontSize +2;
}

void tft9341::drawPixel (int16_t x, int16_t y)
{
	sector(x,x,y,y);
	writeData2bytes(_color>>8,_color);
}

void tft9341::drawHLine (int16_t x, int16_t y, int16_t w)
{
	sector(x,x+w-1,y,y);
	while (w--)
	writeData2bytes(_color>>8,_color);
}

void tft9341::drawVLine (int16_t x, int16_t y, int16_t h)
{
	sector(x,x,y,y+h-1);
	while (h--)
	writeData2bytes(_color>>8,_color);
}

void tft9341::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	int16_t steep = ABS(y1 - y0) > ABS(x1 - x0);
	if (steep) {
		SWAP(x0, y0);
		SWAP(x1, y1);
	}

	if (x0 > x1) {
		SWAP(x0, x1);
		SWAP(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = ABS(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
		} else {
		ystep = -1;
	}

	for (; x0<=x1; x0++) {
		if (steep) {
			drawPixel(y0, x0);
			} else {
			drawPixel(x0, y0);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void tft9341::drawLinePolar (int16_t x0, int16_t y0, int16_t r, int16_t angle){
	float x,y;
	uint16_t x1;
	uint16_t y1;
	
	x = x0 + r*cos (angle*M_PI/180);
	y = y0 + r*sin (angle*M_PI/180);
	
	x1 = round (x);
	y1 = round (y);
	drawLine (x0,y0,x1,y1);
}

void tft9341::drawRect ( int16_t x, int16_t y, int16_t w, int16_t h) {
	drawHLine(x,y,w);
	drawHLine(x,y+h,w);
	drawVLine(x,y,h);
	drawVLine(x+w,y,h);
}

// Draw a triangle
void tft9341::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	drawLine(x0, y0, x1, y1);
	drawLine(x1, y1, x2, y2);
	drawLine(x2, y2, x0, y0);
}

void tft9341::drawCircle(int16_t x0, int16_t y0, int16_t r) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0  , y0+r);
	drawPixel(x0  , y0-r);
	drawPixel(x0+r, y0);
	drawPixel(x0-r, y0);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		
		drawPixel(x0 + x, y0 + y);
		drawPixel(x0 - x, y0 + y);
		drawPixel(x0 + x, y0 - y);
		drawPixel(x0 - x, y0 - y);
		drawPixel(x0 + y, y0 + x);
		drawPixel(x0 - y, y0 + x);
		drawPixel(x0 + y, y0 - x);
		drawPixel(x0 - y, y0 - x);
	}
}

void tft9341::drawCircleHelper( int16_t x0, int16_t y0, int16_t r, int8_t cornername) {
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y);
			drawPixel(x0 + y, y0 + x);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y);
			drawPixel(x0 + y, y0 - x);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x);
			drawPixel(x0 - x, y0 + y);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x);
			drawPixel(x0 - x, y0 - y);
		}
	}
}

// Draw a rounded rectangle
void tft9341::drawRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t r) {
	// smarter version
	drawHLine(x+r  , y    , w-2*r); // Top
	drawHLine(x+r  , y+h-1, w-2*r); // Bottom
	drawVLine(x    , y+r  , h-2*r); // Left
	drawVLine(x+w-1, y+r  , h-2*r); // Right
	// draw four corners
	drawCircleHelper(x+r    , y+r    , r, 1);
	drawCircleHelper(x+w-r-1, y+r    , r, 2);
	drawCircleHelper(x+w-r-1, y+h-r-1, r, 4);
	drawCircleHelper(x+r    , y+h-r-1, r, 8);
}

void tft9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
	sector(x,x+w-1,y,y+h-1);
	for(y=h; y>0; y--)
	for(x=w; x>0; x--)
	writeData2bytes(_color>>8,_color);
}


// Fill a triangle
void tft9341::fillTriangle ( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{

	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		SWAP(y0, y1); SWAP(x0, x1);
	}
	if (y1 > y2) {
		SWAP(y2, y1); SWAP(x2, x1);
	}
	if (y0 > y1) {
		SWAP(y0, y1); SWAP(x0, x1);
	}

	if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if(x1 < a)      a = x1;
		else if(x1 > b) b = x1;
		if(x2 < a)      a = x2;
		else if(x2 > b) b = x2;
		drawHLine(a, y0, b-a+1);
		return;
	}

	int16_t
	dx01 = x1 - x0,
	dy01 = y1 - y0,
	dx02 = x2 - x0,
	dy02 = y2 - y0,
	dx12 = x2 - x1,
	dy12 = y2 - y1,
	sa   = 0,
	sb   = 0;


	if(y1 == y2) last = y1;   // Include y1 scanline
	else         last = y1-1; // Skip it

	for(y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if(a > b) SWAP(a,b);
		drawHLine(a, y, b-a+1);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if(a > b) SWAP(a,b);
		drawHLine(a, y, b-a+1);
	}
}

void tft9341::fillCircle(int16_t x0, int16_t y0, int16_t r)
{
	drawVLine(x0, y0-r, 2*r+1);
	fillCircleHelper(x0, y0, r, 3, 0);
}

void tft9341::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,uint8_t cornername, int16_t delta)
{

	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			drawVLine(x0+x, y0-y, 2*y+1+delta);
			drawVLine(x0+y, y0-x, 2*x+1+delta);
		}
		if (cornername & 0x2) {
			drawVLine(x0-x, y0-y, 2*y+1+delta);
			drawVLine(x0-y, y0-x, 2*x+1+delta);
		}
	}
}

// Fill a rounded rectangle
void tft9341::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r) {
	// smarter version
	fillRect(x+r, y, w-2*r, h);

	// draw four corners
	fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1);
	fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1);
}

void tft9341::invertDisplay(uint8_t i) {
	writeCommand(i ? INVON : INVOFF);
}


void tft9341::setRotation(uint8_t m) {

	writeCommand(MADCTL);
	_rotation = m%4;
	switch (_rotation) {
		case 0:
		writeData(right2left);
		_height = HEIGHT;
		_width = WIDTH;
		_x = 0;
		_y = 0;
		break;
		case 1:
		writeData(down2up);
		_height = WIDTH;
		_width = HEIGHT;
		_x = 0;
		_y = 0;
		break;
		case 2:
		writeData(left2right);
		_height = HEIGHT;
		_width = WIDTH;
		_x = 0;
		_y = 0;
		break;
		case 3:
		writeData(up2down);
		_height = WIDTH;
		_width = HEIGHT;
		_x = 0;
		_y = 0;
		break;
	}
}

uint16_t tft9341::color565(uint32_t color)
{
	uint8_t r,g,b;
	
	b = color & 0xFF;
	g = (color >> 8) & 0xFF;
	r = (color >> 16) & 0xFF;
	
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void tft9341::putRgbColorPixel(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t colorPixel;
	colorPixel = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	
	writeData2bytes(colorPixel>>8,colorPixel);
}

void tft9341::startScrollingRolling()
{
	writeCommand(0x33);
	writeData(0x00);
	writeData(0x00);
	writeData(0x01);
	writeData(0x2c);
	writeData(0x00);
	writeData(0x14);
}

void tft9341::endScrollingRolling (uint8_t Direction)
{
	if( Direction == 2)
	{
		writeCommand(0x37);
		writeData(numbera);
		writeData(numberb);

		if (numbera == 0)
		{
			if (numberb < 255 )
			numberb++;
			else
			{
				numbera = 1;
				numberb = 0;
			}
		}
		else
		{
			if	(numberb < 64)
			numberb++;
			else
			{
				numberb=0;
				numbera=0;
			}
		}
	}
	
	if( Direction == 1)
	{
		writeCommand(0x37);
		writeData(number1);
		writeData(number2);

		if(number1 == 1)
		{
			if(number2 > 0)
			number2--;
			else
			{
				number2 = 255;
				number1 = 0;
			}
		}

		else if (number2> 0)
		number2--;
		else
		{
			number2 = 64 ;
			number1 = 1;
		}
	}
}

tft9341 lcd;
