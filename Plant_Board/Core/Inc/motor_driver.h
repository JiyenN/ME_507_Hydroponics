/*
 * motor_driver.h
 *
 *  Created on: Jun 2, 2026
 *      Author: malaika
 */

#ifndef INC_MOTOR_DRIVER_H_
#define INC_MOTOR_DRIVER_H_
#include "stm32f4xx_hal.h"

typedef struct {

	uint16_t    motor_out_1;
	uint16_t    motor_out_2;
	TIM_HandleTypeDef*	htim;


} motor_t;

void motor_chan_enable(motor_t* p_mot);

void motor_set_duty_cycle(motor_t* p_mot, int8_t duty_cycle);

void motor_stop(motor_t* p_mot);

#endif /* INC_MOTOR_DRIVER_H_ */
