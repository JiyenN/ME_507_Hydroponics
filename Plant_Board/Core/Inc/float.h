/*
 * float.h
 *
 *  Created on: Jun 9, 2026
 *      Author: malaika
 */

#ifndef INC_FLOAT_H_
#define INC_FLOAT_H_
#include "stm32f4xx_hal.h"

typedef struct{
	GPIO_TypeDef* gpio_float;
	uint16_t pin_float;

} float_t;

GPIO_PinState read_float(float_t* p_float);


#endif /* INC_FLOAT_H_ */
