/*
 * touch.h
 *
 * Created: 23/01/2016 19:58:46
 * Last Update 29/09/17
 *  Author: Yaron Dori
 */ 


#ifndef _TOUCH_H_
#define _TOUCH_H_

#define T_SS POD7
#define T_IRQ PID2

class touch{
	int16_t TypeTouch;
	uint8_t _rotation;
	int16_t _x, _y, _width, _height;
	uint16_t getX();
	uint16_t getY();
	public:
	void InitTypeTouch(int type);
	void begin();
	void readxy();
	uint16_t readx() {return (_x);};
	uint16_t ready() {return (_y);};
};

extern touch lcdtouch;

#endif /* TOUCH_H_ */