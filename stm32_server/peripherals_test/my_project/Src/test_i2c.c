/*
 * test_i2c.c
 *
 *  Created on: Jun 18, 2025
 *      Author: Roi
 */


#include "peripherals.h"
#include "i2c.h"
#include <string.h>


extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c4;

static volatile uint8_t i2c2_tx_done,  i2c2_rx_done,  i2c4_rx_done, i2c4_tx_done;

static uint8_t wait_flag(volatile uint8_t *flag, uint32_t timeout_ms)
{
    uint32_t t0 = HAL_GetTick();
    while (!(*flag))
    {
        if (HAL_GetTick() - t0 > timeout_ms){
        	return 1;
        }
    }
    *flag =0;
    return 0;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c2){
		i2c2_tx_done = 1;
	}
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c2){
		i2c2_rx_done = 1;
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c4){
		i2c4_rx_done = 1;
	}
}
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c4){
		i2c4_tx_done = 1;
	}
}


uint8_t test_i2c(const test_command_t *cmd){

	uint32_t slave_addr = hi2c4.Init.OwnAddress1;

    uint8_t rxBuf4[MAX_PATTERN_LEN] = {};
    uint8_t rxBuf2[MAX_PATTERN_LEN] = {};
    uint8_t txBuf4[MAX_PATTERN_LEN] = {};
    uint8_t txBuf2[MAX_PATTERN_LEN] = {};

	for(uint8_t i = 0; i < cmd->iterations ; ++i){

        memset(rxBuf4, 0 ,sizeof(rxBuf4));
        memset(rxBuf2, 0 ,sizeof(rxBuf2));
        memset(txBuf4, 0 ,sizeof(txBuf4));
        memset(txBuf2, 0 ,sizeof(txBuf2));

    	i2c2_tx_done = 0;
    	i2c2_rx_done = 0;
    	i2c4_tx_done = 0;
    	i2c4_rx_done = 0;

        memcpy(txBuf2, cmd->pattern, cmd->pattern_len);
        if (memcmp(cmd->pattern, txBuf2, cmd->pattern_len) != 0)
             return TEST_FAILURE;

        while (hi2c4.State != HAL_I2C_STATE_READY) {}
		if(HAL_I2C_Slave_Receive_IT(&hi2c4,rxBuf4,cmd->pattern_len) != HAL_OK)
			return TEST_FAILURE;

		HAL_Delay(1);

		if(HAL_I2C_Master_Transmit_IT(&hi2c2, 4, txBuf2, cmd->pattern_len) != HAL_OK)
			return TEST_FAILURE;

		HAL_Delay(1);

		if (wait_flag(&i2c2_tx_done, TIMEOUT) || wait_flag(&i2c4_rx_done, TIMEOUT))
			return TEST_FAILURE;

		memcpy(txBuf4, rxBuf4, cmd->pattern_len);

		if(HAL_I2C_Master_Receive_IT(&hi2c2, slave_addr, rxBuf2, cmd->pattern_len) != HAL_OK)
			return TEST_FAILURE;

		HAL_Delay(1);


		if(HAL_I2C_Slave_Transmit_IT(&hi2c4, txBuf4, cmd->pattern_len) != HAL_OK)
		 	return TEST_FAILURE;

		HAL_Delay(1);


		if (wait_flag(&i2c4_tx_done, TIMEOUT) || wait_flag(&i2c2_rx_done, TIMEOUT))
		    return TEST_FAILURE;

		if (memcmp(cmd->pattern, rxBuf2, cmd->pattern_len) != 0)
		    return TEST_FAILURE;
	}
	return TEST_SUCCESS;
}
