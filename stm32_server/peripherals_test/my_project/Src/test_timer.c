/*
 * test_timer.c
 *
 *  Created on: Jun 24, 2025
 *      Author: Roi
 */

#include "peripherals.h"
#include "tim.h"

extern TIM_HandleTypeDef htim6;

static volatile uint8_t flag = 0;
uint32_t start_time;
uint32_t elapsed;

void  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM6){
		flag =1;
		HAL_TIM_Base_Stop_IT(&htim6);
	}
}


uint8_t test_timer(const test_command_t *cmd){

	for(uint8_t i = 0; i < cmd->iterations ; ++i){
		flag = 0;

		__HAL_TIM_SET_COUNTER(&htim6, 0);
		if(HAL_TIM_Base_Start_IT(&htim6) != HAL_OK)
			return TEST_FAILURE;

		start_time = HAL_GetTick();

		while (!flag){
			if (HAL_GetTick() - start_time > 1010)
				return TEST_FAILURE;
		}

		elapsed = HAL_GetTick() - start_time;
		if (elapsed < 900 || elapsed > 1100)
			return TEST_FAILURE;
	}
	return TEST_SUCCESS;
}
