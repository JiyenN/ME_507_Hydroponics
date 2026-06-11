/*
 * probe.c
 *
 *  Created on: Jun 11, 2026
 *      Author: malai
 */


#include "probe.h"
#include <stdio.h>

void tds_init(tds_t *tds, ADC_HandleTypeDef *hadc, UART_HandleTypeDef *huart, float vref)
{
    tds->hadc = hadc;
    tds->huart = huart;
    tds->adc_value = 0;
    tds->voltage = 0.0f;
    tds->ppm = 0.0f;
    tds->temperature_c = 25.0f;
    tds->vref = vref;
}

float tds_read_ppm(tds_t *tds)
{
    HAL_ADC_Start(tds->hadc);

    if (HAL_ADC_PollForConversion(tds->hadc, 100) == HAL_OK) {
        tds->adc_value = HAL_ADC_GetValue(tds->hadc);
    }

    HAL_ADC_Stop(tds->hadc);

    tds->voltage = ((float)tds->adc_value * tds->vref) / 4095.0f;

    float compensation = 1.0f + 0.02f * (tds->temperature_c - 25.0f);
    float compensated_voltage = tds->voltage / compensation;

    tds->ppm =
        (133.42f * compensated_voltage * compensated_voltage * compensated_voltage
        -255.86f * compensated_voltage * compensated_voltage
        +857.39f * compensated_voltage) * 0.5f;

    return tds->ppm;
}

void tds_uart_print(tds_t *tds)
{
    char msg[100];

    uint32_t voltage_mv = (uint32_t)(tds->voltage * 1000.0f);
    uint32_t ppm_int = (uint32_t)(tds->ppm);

    int len = snprintf(msg, sizeof(msg),
                       "ADC: %lu | Voltage: %lu mV | TDS: %lu ppm\r\n",
                       tds->adc_value,
                       voltage_mv,
                       ppm_int);

    HAL_UART_Transmit(tds->huart, (uint8_t *)msg, len, 100);
}
