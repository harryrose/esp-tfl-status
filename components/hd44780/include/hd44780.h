/*
 *	Based on the Arduino library found here: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
 *
 *  This file implements an ESP32-compatible driver for the HD44780 LCD driver (
 *  the specs of which can be found here:
 *  	https://www.openhacks.com/uploadsproductos/eone-1602a1.pdf
 *  ).
 *
 *  Note that the actual interface itself has been abstracted away by the `Interface` virtual class.
 *  This should allow users with the parallel interface to implement an interface suitable for their needs.
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

// HD44780 is the driver for the screen.  Note that it implements ostream
// and so can be written to using the `<<` operator.
//
// i.e.
//	Interface *someInterface ...
//  HD44780 display(someInteface);
//  display << "Hello world!"
//
//  will result in `Hello world` being written to the display.
class HD44780 : public std::ostream, public std::streambuf {
public:
	HD44780(Interface *iface);
	// begin prepares the screen for use. it must be called before any other class methods.
	virtual h_err_t begin();
	// clear resets the data in the memory of the screen
	virtual h_err_t clear();
	// home sets the cursor to the column 0, row 0
	virtual h_err_t home();
	// enableBacklight turns the backlight on or off
	virtual h_err_t enableBacklight(bool en);
	// setCursor moves the write cursor to a particular column/row
	virtual h_err_t setCursor(unsigned char col, unsigned char row);
	// enableDisplay turns the characters on the display on or off without affecting the data in memory.
	virtual h_err_t enableDisplay(bool en);
	// enableCursor shows/hides the cursor
	virtual h_err_t enableCursor(bool en);
	// enableBlink enables/disables the blinking of the cursor, if it is visible
	virtual h_err_t enableBlink(bool en);
	// setDirection configures the direction in which text appears on the screen
	virtual h_err_t setDirection(hd_dir_t dir);
	// write writes a character to the screen.
	virtual h_err_t write(unsigned char byt);

	// overflow is from std::streambuf. it writes the character represented by the integer to the screen
	int overflow(int c);
private:
	// writeDisplayControl writes the displayControl value to the screen
	h_err_t writeDisplayControl();
	// writeDisplayMode writes the displayMode value to the screen
	h_err_t writeDisplayMode();

	Interface *iface;
	unsigned char displayControl;
	unsigned char displayMode;
};

// PresentationHD44780 is an implementation of the HD44780 with the aim of making
// rendering text onto the screen more 'user' friendly.
//
// With the base HD44780 class, if text wraps or runs off the edge of the screen,
// it does not necessarily immediately appear on the next line -- it may continue
// writing off the edge of the screen, or it may skip a line due to the arrangement
// of memory on the device.
//
// This class keeps track of the position of the cursor, and wraps text appropriately
// at the edge of the screen.
//
// It also treats `\n` and `\r` characters as newline and carriage return respectively.
// (Note in this case, `\n` will also return the cursor to the beginning of the next line).
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
