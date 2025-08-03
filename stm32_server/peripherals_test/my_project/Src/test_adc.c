/*
 * test_adc.c
 *
 *  Created on: Jun 22, 2025
 *      Author: Roi
 */

#include "peripherals.h"
#include "adc.h"

#define MAX_DEVIATION 50

extern ADC_HandleTypeDef hadc1;

static volatile uint8_t flag = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc == &hadc1){
		HAL_ADC_Stop_DMA(&hadc1);
		flag = 1;
	}
}


uint8_t test_adc(const test_command_t *cmd){

	uint32_t expected_value = 4095;
	uint32_t adc_value = 0;

	for(uint8_t i = 0; i < cmd->iterations ; ++i){
		flag = 0;
		if(HAL_ADC_Start_DMA(&hadc1, &adc_value, 1) != HAL_OK)
			 return TEST_FAILURE;

		uint32_t t0 = HAL_GetTick();
		while (!flag){
			if (HAL_GetTick() - t0 > TIMEOUT){
				return TEST_FAILURE;
			}
		}

		if( expected_value - adc_value > MAX_DEVIATION)
			return TEST_FAILURE;
	}
	return TEST_SUCCESS;
}
