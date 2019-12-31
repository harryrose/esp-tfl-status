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
#include "esp_log.h"
#include "esp_err.h"

enum {
	BACKLIGHT_OFF = 0x00,
	BACKLIGHT_ON = 0x08
};

enum {
	LCD_4BIT = 0x00,
	LCD_2LINE = 0x08,
	LCD_58Dots = 0x00,
	LCD_CMD_FUNCTION_SET = 0x20
};

I2CInterface::I2CInterface(I2CBus *bus, unsigned char addr)
	: i2c(bus), addr(addr), backlight(BACKLIGHT_ON) {

}

I2CInterface::~I2CInterface() {

}

h_err_t I2CInterface::begin(){
	auto err = write(backlight);
	vTaskDelay(1000 / portTICK_PERIOD_MS);

	// attempt to set the LCD to 4-bit mode a number of times
	for(int i = 0; i < 3 && err == HERR_OK; i++) {
		err = write4Bits(0x03 << 4);
		ets_delay_us(4500);
	}
	if(err != HERR_OK) {
		ESP_LOGE(__FILE__,"error setting 4-bit mode. 0x%x. %s",err,esp_err_to_name(err));
		return err;
	}

	err = write4Bits(0x02 << 4);
	if(err != HERR_OK) {
		ESP_LOGE(__FILE__,"error sending 0x02 mode. 0x%x. %s",err,esp_err_to_name(err));
		return err;
	}

	ets_delay_us(1000);
	return write(HD_REG_CMD, LCD_CMD_FUNCTION_SET | LCD_4BIT | LCD_2LINE | LCD_58Dots);
}

h_err_t I2CInterface::write(hd_reg_t reg, unsigned char byt) {
	auto err = write4Bits((byt & 0xf0)| reg | backlight);
	if(err == HERR_OK){
		err = write4Bits(((byt & 0x0f) << 4) | reg | backlight);
	}
	return err;
}

h_err_t I2CInterface::setBacklight(bool on) {
	if(on) {
		backlight = BACKLIGHT_ON;
	} else {
		backlight = BACKLIGHT_OFF;
	}
	return write(0);
}

bool I2CInterface::getBacklight() {
	if(backlight & BACKLIGHT_ON){
		return true;
	}
	return false;
}

h_err_t I2CInterface::write(unsigned char byt) {
	auto cmd = i2c->begin();
	byt |= backlight;
	cmd->write(addr, &byt, 1);
	auto err = cmd->execute();
	i2c->free(cmd);
	return err;
}

enum {
	HDI2C_EN = 0x04
};

h_err_t I2CInterface::write4Bits(unsigned char nibble) {

	auto err = write(nibble);
	if(err == HERR_OK){
		err = write(nibble | HDI2C_EN);
		ets_delay_us(1);
		err = write(nibble & ~HDI2C_EN);
		ets_delay_us(50);
	}
	return err;
}

