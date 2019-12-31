/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "error.h"
#include "espi2c.h"
#include "hd44780.h"
#include "i2c-interface.h"
#include "esp_log.h"

extern "C"{
	void app_main();
}

void app_main()
{
    printf("Hello world!\n");
    ESPI2C *i2c = new ESPI2C(I2C_NUM_0, gpio_num_t(22), gpio_num_t(21), 200000);
    I2CInterface *i2ciface = new I2CInterface(i2c,0x27);
	HD44780 *disp = new HD44780(i2ciface);
	auto err = disp->Begin();
	ESP_LOGI(__FILE__,"Result: %x",err);
	err = disp->EnableBacklight(true);
	ESP_LOGI(__FILE__,"Result: %x",err);
    fflush(stdout);
}
