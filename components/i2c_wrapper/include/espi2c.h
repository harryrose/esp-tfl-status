/*
 * espi2c.h
 *
 *  Created on: 31 Dec 2019
 *      Author: harry
 */

#ifndef COMPONENTS_I2C_WRAPPER_INCLUDE_ESPI2C_H_
#define COMPONENTS_I2C_WRAPPER_INCLUDE_ESPI2C_H_

#include "i2c.h"
#include "driver/i2c.h"

class ESPI2C;

class ESPI2CCommand : public I2CCommand {
public:
	ESPI2CCommand(const ESPI2CCommand & c);
	~ESPI2CCommand();
	h_err_t Write(unsigned char address, unsigned char * const in, size_t bytes) override;
	h_err_t Read(unsigned char address, unsigned char * out, size_t bytes) override;
	h_err_t Execute() override;

private:
	ESPI2CCommand(ESPI2C *c, i2c_cmd_handle_t handle);
	ESPI2C *iface;
	i2c_cmd_handle_t cmd;

	friend ESPI2C;
};

class ESPI2C : public I2CBus {
public:
	ESPI2C(i2c_port_t i2cNum, gpio_num_t sclPin, gpio_num_t sdaPin, uint32_t frequency);
	~ESPI2C();
	I2CCommand *Begin() override;
	h_err_t Free(I2CCommand *cmd);

private:
	i2c_port_t port;

	friend ESPI2CCommand;
};


#endif /* COMPONENTS_I2C_WRAPPER_INCLUDE_ESPI2C_H_ */
