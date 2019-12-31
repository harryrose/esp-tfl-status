/*
 * hd44780.h
 *
 *  Created on: 31 Dec 2019
 *      Author: harry
 */

#ifndef COMPONENTS_HD44780_INCLUDE_HD44780_H_
#define COMPONENTS_HD44780_INCLUDE_HD44780_H_

#include <iostream>
#include "error.h"

typedef enum {
	HD_REG_CMD = 0,
	HD_REG_DATA = 0x01
} hd_reg_t;

class Interface {
public:
	virtual ~Interface() {};
	virtual h_err_t begin()=0;
	virtual h_err_t write(hd_reg_t reg,unsigned char byt) =0;
	virtual h_err_t setBacklight(bool on)=0;
	virtual bool getBacklight() =0;
};


class HD44780 : public std::ostream, public std::streambuf {
public:
	HD44780(Interface *iface);
	h_err_t begin();
	h_err_t clear();
	h_err_t home();
	h_err_t enableBacklight(bool en);

	// from std::streambuf
	int overflow(int c);
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
