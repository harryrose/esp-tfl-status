/*
 * error.h
 *
 *  Created on: 30 Dec 2019
 *      Author: harry
 */

#ifndef COMPONENTS_ERROR_INCLUDE_ERROR_H_
#define COMPONENTS_ERROR_INCLUDE_ERROR_H_

#ifdef ESP_PLATFORM
#include "esp_err.h"

typedef esp_err_t h_err_t;

#define HERR_OK 0

#endif


#endif /* COMPONENTS_ERROR_INCLUDE_ERROR_H_ */
