/*
 * valve.h
 *
 *  Created on: Jun 4, 2026
 *      Author: malaika
 */

#ifndef INC_VALVE_H_
#define INC_VALVE_H_
#include "stm32f4xx_hal.h"

typedef struct{
	GPIO_TypeDef* gpio_Relay;
	uint16_t pin_Relay;
//	GPIO_TypeDef* Relay2_GPIO_Port;
//	uint16_t Relay2_Pin;
} valve_t;

void valve_open(valve_t* p_valve);

void valve_close(valve_t* p_valve);

#endif /* INC_VALVE_H_ */
