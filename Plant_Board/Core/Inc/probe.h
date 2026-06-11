/*
 * probe.h
 *
 *  Created on: Jun 11, 2026
 *      Author: malai
 */

#ifndef INC_PROBE_H_
#define INC_PROBE_H_
#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef struct {
    ADC_HandleTypeDef *hadc;
    UART_HandleTypeDef *huart;

    uint32_t adc_value;
    float voltage;
    float ppm;
    float temperature_c;
    float vref;
} tds_t;

void tds_init(tds_t *tds, ADC_HandleTypeDef *hadc, UART_HandleTypeDef *huart, float vref);
float tds_read_ppm(tds_t *tds);
void tds_uart_print(tds_t *tds);


#endif /* INC_PROBE_H_ */
