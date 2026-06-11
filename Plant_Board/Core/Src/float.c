/*
 * float.c
 *
 *  Created on: Jun 9, 2026
 *      Author: malaika
 */


#include "float.h"

GPIO_PinState read_float(float_t* p_float){
	    return HAL_GPIO_ReadPin(p_float->gpio_float, p_float->pin_float);
}


