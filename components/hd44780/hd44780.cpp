/*
 * hd44780.cpp
 *
 *  Created on: 31 Dec 2019
 *      Author: harry
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hd44780.h"

// Commands
enum {
	LCD_CMD_CLEAR = 0x01,
	LCD_CMD_HOME  = 0x02,
	LCD_CMD_MODE  = 0x04,
	LCD_CMD_DISPLAY_CONTROL = 0x08,
	LCD_CMD_CURSOR_SHIFT = 0x10,
	LCD_CMD_FUNCTION_SET = 0x20,
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
	: iface(iface), displayControl(0),displayMode(0){
}


#define _HD_DELAY_MS(x)

h_err_t HD44780::Begin(){
	vTaskDelay(50 / portTICK_PERIOD_MS);
	auto err = iface->Begin();
	if(err != HERR_OK){
		return err;
	}
	displayControl = LCD_CTRL_DISPLAY_ON;
	err = writeDisplayControl();
	if(err != HERR_OK){
		return err;
	}

	err = Clear();
	if(err != HERR_OK){
		return err;
	}

	displayMode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
	err = writeDisplayMode();
	if(err != HERR_OK){
		return err;
	}

	return Home();
}

h_err_t HD44780::Clear(){
	auto err = iface->Write(HD_REG_CMD,LCD_CMD_CLEAR);
	vTaskDelay(2 / portTICK_PERIOD_MS);
	return err;
}

h_err_t HD44780::Home(){
	auto err = iface->Write(HD_REG_CMD,LCD_CMD_HOME);
	vTaskDelay(2/portTICK_PERIOD_MS);
	return err;
}

h_err_t HD44780::EnableBacklight(bool en) {
	return iface->SetBacklight(en);
}

h_err_t HD44780::writeDisplayControl(){
	return iface->Write(HD_REG_CMD, 0x20 | displayControl);
}

h_err_t HD44780::writeDisplayMode(){
	return iface->Write(HD_REG_CMD, LCD_CMD_MODE | displayMode);
}

