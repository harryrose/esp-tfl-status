/*
 * i2c.h
 *
 *  Created on: 30 Dec 2019
 *      Author: harry
 */

#ifndef COMPONENTS_I2C_WRAPPER_INCLUDE_I2C_H_
#define COMPONENTS_I2C_WRAPPER_INCLUDE_I2C_H_

#include "error.h"
#include <stdio.h>

class I2CCommand {
public:
	virtual ~I2CCommand() {}
	virtual h_err_t write(unsigned char address, unsigned char * const in, size_t bytes) =0;
	virtual h_err_t read(unsigned char address, unsigned char * out, size_t bytes) =0;
	virtual h_err_t execute() =0;
};

class I2CBus {
public:
	virtual ~I2CBus() {}
	virtual I2CCommand *begin() =0;
	virtual h_err_t free(I2CCommand *) =0;
};


#endif /* COMPONENTS_I2C_WRAPPER_INCLUDE_I2C_H_ */
