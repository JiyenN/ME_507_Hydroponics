/*
 * motor_driver.c
 *
 *  Created on: Jun 2, 2026
 *      Author: malaika
 */

#include "motor_driver.h"
#include "stdlib.h"

void motor_chan_enable(motor_t* p_mot){
	  HAL_TIM_PWM_Start_IT(p_mot->htim, p_mot->motor_out_1);
	  HAL_TIM_PWM_Start_IT(p_mot->htim, p_mot->motor_out_2);
}

void motor_set_duty_cycle(motor_t* p_mot, int8_t duty_cycle){
	__HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->motor_out_1, (duty_cycle*p_mot->htim->Instance->ARR)/100);
	__HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->motor_out_2, 0);
}

void motor_stop(motor_t* p_mot){
	__HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->motor_out_1, 0);
	__HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->motor_out_2, 0);
}
