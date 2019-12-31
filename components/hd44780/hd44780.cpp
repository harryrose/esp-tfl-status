/*
 * hd44780.cpp
 *
 *  Created on: 31 Dec 2019
 *      Author: harry
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hd44780.h"
#include "esp_log.h"

// Commands
enum {
	LCD_CMD_CLEAR = 0x01,
	LCD_CMD_HOME  = 0x02,
	LCD_CMD_MODE  = 0x04,
	LCD_CMD_DISPLAY_CONTROL = 0x08,
	LCD_CMD_CURSOR_SHIFT = 0x10,

	LCD_CMD_SET_CG_RAM_ADDR = 0x40,
	LCD_CMD_SET_DD_RAM_ADDR = 0x80
};

enum {
	LCD_ENTRY_RIGHT = 0x00,
	LCD_ENTRY_LEFT  = 0x02,
	LCD_ENTRY_SHIFT_INCREMENT = 0x01,
	LCD_ENTRY_SHIFT_DECREMENT = 0x00
};

enum {
	LCD_CTRL_DISPLAY_ON = 0x04,
	LCD_CTRL_CURSOR_ON  = 0x02,
	LCD_CTRL_BLINK_ON   = 0x01
};

enum {
	LCD_BACKLIGHT_ON = 0x08
};

HD44780::HD44780(Interface *iface)
	: std::ostream(this), iface(iface), displayControl(0),displayMode(0){
}

int HD44780::overflow(int c) {
	ESP_LOGI(__FILE__, "writing character 0x%x",c);
	return iface->write(HD_REG_DATA,(unsigned char) c);
}

h_err_t HD44780::begin(){
	vTaskDelay(50 / portTICK_PERIOD_MS);
	auto err = iface->begin();
	if(err != HERR_OK){
		ESP_LOGE(__FILE__,"error beginning interface. 0x%x. %s",err,esp_err_to_name(err));
		return err;
	}
	displayControl = LCD_CTRL_DISPLAY_ON;
	err = writeDisplayControl();
	if(err != HERR_OK){
		ESP_LOGE(__FILE__,"error writing display control. 0x%x. %s",err,esp_err_to_name(err));
		return err;
	}

	err = clear();
	if(err != HERR_OK){
		ESP_LOGE(__FILE__,"error clearing display. 0x%x. %s",err,esp_err_to_name(err));
		return err;
	}

	displayMode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
	err = writeDisplayMode();
	if(err != HERR_OK){
		ESP_LOGE(__FILE__,"error writing display mode. 0x%x. %s",err,esp_err_to_name(err));
		return err;
	}

	return home();
}

h_err_t HD44780::clear(){
	auto err = iface->write(HD_REG_CMD,LCD_CMD_CLEAR);
	ets_delay_us(2000);
	return err;
}

h_err_t HD44780::home(){
	auto err = iface->write(HD_REG_CMD,LCD_CMD_HOME);
	ets_delay_us(2000);
	return err;
}

h_err_t HD44780::enableBacklight(bool en) {
	return iface->setBacklight(en);
}

h_err_t HD44780::enableDisplay(bool en) {
	if(en){
		displayControl |= LCD_CTRL_DISPLAY_ON;
	} else {
		displayControl &= ~LCD_CTRL_DISPLAY_ON;
	}
	return writeDisplayControl();
}

h_err_t HD44780::enableCursor(bool en) {
	if(en) {
		displayControl |= LCD_CTRL_CURSOR_ON;
	} else {
		displayControl &= ~LCD_CTRL_CURSOR_ON;
	}
	return writeDisplayControl();
}

h_err_t HD44780::enableBlink(bool en) {
	if(en){
		displayControl |= LCD_CTRL_BLINK_ON;
	} else {
		displayControl &= ~LCD_CTRL_BLINK_ON;
	}
	return writeDisplayControl();
}

h_err_t HD44780::writeDisplayControl(){
	return iface->write(HD_REG_CMD, LCD_CMD_DISPLAY_CONTROL | displayControl);
}

h_err_t HD44780::writeDisplayMode(){
	return iface->write(HD_REG_CMD, LCD_CMD_MODE | displayMode);
}

h_err_t HD44780::setCursor(unsigned char col, unsigned char row) {
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 }; // stolen straight from the lcd i2c library
	return iface->write(HD_REG_CMD, LCD_CMD_SET_DD_RAM_ADDR | (col + row_offsets[row & 0x03]));
}

h_err_t HD44780::setDirection(hd_dir_t dir) {
	if(dir == HD_DIR_LTR) {
		displayMode |= HD_DIR_LTR;
	} else {
		displayMode &= ~HD_DIR_LTR;
	}
	return writeDisplayMode();
}
