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

typedef enum {
	HD_DIR_RTL = 0x00,
	HD_DIR_LTR = 0x02
} hd_dir_t;

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
	virtual h_err_t begin();
	virtual h_err_t clear();
	virtual h_err_t home();
	virtual h_err_t enableBacklight(bool en);
	virtual h_err_t setCursor(unsigned char col, unsigned char row);
	virtual h_err_t enableDisplay(bool en);
	virtual h_err_t enableCursor(bool en);
	virtual h_err_t enableBlink(bool en);
	virtual h_err_t setDirection(hd_dir_t dir);
	virtual h_err_t write(unsigned char byt);

	// from std::streambuf
	int overflow(int c);
private:
	h_err_t writeDisplayControl();
	h_err_t writeDisplayMode();
	Interface *iface;
	unsigned char displayControl;
	unsigned char displayMode;
};

class PresentationHD44780 : public HD44780 {
public:
	PresentationHD44780(Interface *iface, unsigned char cols, unsigned char rows);
	h_err_t home() override;
	h_err_t setCursor(unsigned char col, unsigned char row);
	h_err_t write(unsigned char byt) override;
private:
	unsigned char cols;
	unsigned char rows;
	unsigned char cursorCol;
	unsigned char cursorRow;
};

#endif /* COMPONENTS_HD44780_INCLUDE_HD44780_H_ */
