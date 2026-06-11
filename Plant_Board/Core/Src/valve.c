/*
 * valve.c
 *
 *  Created on: Jun 4, 2026
 *      Author: malaika
 */

#include "valve.h"

void valve_open(valve_t* p_valve){
	HAL_GPIO_WritePin(p_valve->gpio_Relay, p_valve->pin_Relay, GPIO_PIN_SET);
}

void valve_close(valve_t* p_valve){
	HAL_GPIO_WritePin(p_valve->gpio_Relay, p_valve->pin_Relay, GPIO_PIN_RESET);
}
