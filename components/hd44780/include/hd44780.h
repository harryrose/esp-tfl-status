/*
 * hd44780.h
 *
 *  Created on: 31 Dec 2019
 *      Author: harry
 */

#ifndef COMPONENTS_HD44780_INCLUDE_HD44780_H_
#define COMPONENTS_HD44780_INCLUDE_HD44780_H_

#include "error.h"

#define _HD_LCD_4BIT 0x00
#define _HD_LCD_2LINE 0x08
#define _HD_LCD_58Dots 0x00

typedef enum {
	HD_REG_CMD = 0,
	HD_REG_DATA = 0x01
} hd_reg_t;

class Interface {
public:
	virtual ~Interface() {};
	virtual h_err_t Begin()=0;
	virtual h_err_t Write(hd_reg_t reg,unsigned char byt) =0;
	virtual h_err_t SetBacklight(bool on)=0;
	virtual bool GetBacklight() =0;
};

class HD44780 {
public:
	HD44780(Interface *iface);
	h_err_t Begin();
	h_err_t Clear();
	h_err_t Home();
	h_err_t EnableBacklight(bool en);
//	h_err_t SetCursor();
//	h_err_t EnableDisplay(bool en);
//	h_err_t EnableCursor(bool en);
//	h_err_t EnableBlink(bool en);
//
private:
	h_err_t writeDisplayControl();
	h_err_t writeDisplayMode();
	Interface *iface;
	unsigned char displayControl;
	unsigned char displayMode;
};


#endif /* COMPONENTS_HD44780_INCLUDE_HD44780_H_ */
