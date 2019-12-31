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
    ESPI2C *i2c = new ESPI2C(I2C_NUM_0, gpio_num_t(22), gpio_num_t(21), 100000);
    I2CInterface *i2ciface = new I2CInterface(i2c,0x27);
	HD44780 *disp = new PresentationHD44780(i2ciface,20,4);
	auto err = disp->begin();
	ESP_LOGI(__FILE__,"Result: %x",err);
	err = disp->enableBacklight(true);
	ESP_LOGI(__FILE__,"Result: %x",err);
	(*disp) << "This is an example of a long\nstring to see what happens.";

    fflush(stdout);

}
