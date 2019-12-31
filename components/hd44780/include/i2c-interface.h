/*
 * i2c-interface.h
 *
 *  Created on: 31 Dec 2019
 *      Author: harry
 */

#ifndef COMPONENTS_HD44780_INCLUDE_I2C_INTERFACE_H_
#define COMPONENTS_HD44780_INCLUDE_I2C_INTERFACE_H_

#include "hd44780.h"
#include "i2c.h"


class I2CInterface : public Interface {
public:
	I2CInterface(I2CBus *i2c, unsigned char addr);
	~I2CInterface();
	h_err_t begin();
	h_err_t write(hd_reg_t reg, unsigned char byt) override;
	h_err_t setBacklight(bool on) override;
	bool getBacklight() override;

private:
	h_err_t write(unsigned char byt);
	h_err_t write4Bits(unsigned char nibble);
	I2CBus *i2c;
	unsigned char addr;
	unsigned char backlight;
};



#endif /* COMPONENTS_HD44780_INCLUDE_I2C_INTERFACE_H_ */
