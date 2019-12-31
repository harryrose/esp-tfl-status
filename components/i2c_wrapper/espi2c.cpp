/*
 * espi2c.cpp
 *
 *  Created on: 31 Dec 2019
 *      Author: harry
 */

#include "espi2c.h"
#include "esp_log.h"

#define _BUS_I2C_TAG "I2C"
#define ACK_CHECK_EN 0x01

ESPI2C::ESPI2C(i2c_port_t i2cNum, gpio_num_t sclPin, gpio_num_t sdaPin, uint32_t frequency)
	: port(i2cNum) {
	 i2c_config_t config = {
	            .mode = I2C_MODE_MASTER,
	            .sda_io_num = sdaPin,
	            .sda_pullup_en = GPIO_PULLUP_ENABLE,
	            .scl_io_num = sclPin,
	            .scl_pullup_en = GPIO_PULLUP_ENABLE
	    };
	config.master.clk_speed = frequency;
	esp_err_t err = i2c_param_config(i2cNum, &config);
	if(err != ESP_OK) {
		ESP_LOGE(_BUS_I2C_TAG, "Unable to configure i2c. %s", esp_err_to_name(err));
		return;
	}

	err = i2c_driver_install(i2cNum, I2C_MODE_MASTER,0,0,0);
	if(err != ESP_OK) {
		ESP_LOGE(_BUS_I2C_TAG, "Unable to install i2c driver. %s", esp_err_to_name(err));
		return;
	}
}

ESPI2C::~ESPI2C(){
	i2c_driver_delete(port);
}

I2CCommand *ESPI2C::begin(void) {
	ESPI2CCommand *cmd = new ESPI2CCommand(this, i2c_cmd_link_create());
	return cmd;
}

h_err_t ESPI2C::free(I2CCommand *c) {
	if(c) {
		delete(c);
	}
	return HERR_OK;
}

ESPI2CCommand::ESPI2CCommand(ESPI2C *c, i2c_cmd_handle_t handle) :
	iface(c), cmd(handle){
}

ESPI2CCommand::ESPI2CCommand(const ESPI2CCommand & c):
		iface(c.iface), cmd(c.cmd) {
}

ESPI2CCommand::~ESPI2CCommand(){

}

h_err_t ESPI2CCommand::write(unsigned char address, unsigned char * const in, size_t bytes) {
	if(bytes == 0) {
		return ESP_OK;
	}
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write(cmd, in, bytes, ACK_CHECK_EN);
	return ESP_OK;
}

h_err_t ESPI2CCommand::read(unsigned char address, unsigned char * out, size_t bytes) {
	if(bytes == 0){
		return ESP_OK;
	}
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
	i2c_master_read(cmd, out, bytes, I2C_MASTER_LAST_NACK);
	return ESP_OK;
}

h_err_t ESPI2CCommand::execute(){
	i2c_master_stop(cmd);
	return i2c_master_cmd_begin(iface->port,cmd, 1000 / portTICK_RATE_MS);
}


