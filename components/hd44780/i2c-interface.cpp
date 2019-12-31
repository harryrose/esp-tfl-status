/*
 * i2c-interface.cpp
 *
 * Based on the LiquidCrystal_I2C Library for arduino.
 *
 *  Created on: 31 Dec 2019
 *      Author: harry
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c-interface.h"
#include "esp32/rom/ets_sys.h"

enum {
	BACKLIGHT_OFF = 0x00,
	BACKLIGHT_ON = 0x08
};

I2CInterface::I2CInterface(I2CBus *bus, unsigned char addr)
	: i2c(bus), addr(addr), backlight(BACKLIGHT_OFF) {

}

I2CInterface::~I2CInterface() {

}

h_err_t I2CInterface::Begin(){
	auto err = write(0x00);
	vTaskDelay(1000 / portTICK_PERIOD_MS);

	// attempt to set the LCD to 4-bit mode a number of times
	for(int i = 0; i < 3 && err == HERR_OK; i++) {
		err = write4Bits(0x03);
		vTaskDelay(5 / portTICK_PERIOD_MS);
	}
	if(err != HERR_OK) {
		return err;
	}

	err = write4Bits(0x02);
	if(err != HERR_OK) {
		return err;
	}

	vTaskDelay(1 / portTICK_PERIOD_MS);
	return Write(HD_REG_CMD, _HD_LCD_4BIT | _HD_LCD_2LINE | _HD_LCD_58Dots);
}

h_err_t I2CInterface::Write(hd_reg_t reg, unsigned char byt) {
	auto err = write((byt & 0xf0) | reg | backlight);
	if(err == HERR_OK){
		err = write(((byt & 0x0f) << 4) | reg | backlight);
	}
	return err;
}

h_err_t I2CInterface::SetBacklight(bool on) {
	if(on) {
		backlight = BACKLIGHT_ON;
	} else {
		backlight = BACKLIGHT_OFF;
	}
	return Write(HD_REG_CMD,0);
}

bool I2CInterface::GetBacklight() {
	if(backlight & BACKLIGHT_ON){
		return true;
	}
	return false;
}

h_err_t I2CInterface::write(unsigned char byt) {
	auto cmd = i2c->Begin();
	cmd->Write(addr, &byt, 1);
	auto err = cmd->Execute();
	i2c->Free(cmd);
	return err;
}

#define _HDI2C_EN 0x04

h_err_t I2CInterface::write4Bits(unsigned char nibble) {
	nibble <<= 4;
	auto err = write(nibble);
	if(err == HERR_OK){
		err = write(nibble | _HDI2C_EN);
		ets_delay_us(1);
		err = (nibble & ~_HDI2C_EN);
		ets_delay_us(50);
	}
	return err;
}

