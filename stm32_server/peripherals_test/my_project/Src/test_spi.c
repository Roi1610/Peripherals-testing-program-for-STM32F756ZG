/*
 * test_spi.c
 *
 *  Created on: Jun 17, 2025
 *      Author: Roi
 */
#include "peripherals.h"
#include "spi.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

static volatile uint8_t spi1_tx_done,  spi1_rx_done,  spi2_rx_done, spi2_tx_done;

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

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
    if (hspi == &hspi1){
    	spi1_tx_done = 1;
    }
    else if (hspi == &hspi2){
    	spi2_tx_done = 1;
    }
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){
	if (hspi == &hspi1){
		spi1_rx_done = 1;
	}
	else if (hspi == &hspi2){
		spi2_rx_done = 1;
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi == &hspi1) {
        spi1_tx_done = 1;
        spi1_rx_done = 1;
    }
	else if (hspi == &hspi2){
		spi2_tx_done = 1;
		spi2_rx_done = 1;
	}
}

uint8_t test_spi(const test_command_t *cmd){
    uint8_t rxBuf1[MAX_PATTERN_LEN] = {};
    uint8_t rxBuf2[MAX_PATTERN_LEN] = {};
    uint8_t txBuf1[MAX_PATTERN_LEN] = {};
    uint8_t txBuf2[MAX_PATTERN_LEN] = {};


    for(uint8_t i = 0; i < cmd->iterations ; ++i){

        memset(rxBuf1, 0 ,sizeof(rxBuf1));
        memset(rxBuf2, 0 ,sizeof(rxBuf2));
        memset(txBuf1, 0 ,sizeof(txBuf1));
        memset(txBuf2, 0 ,sizeof(txBuf2));
    	spi1_tx_done = 0;
    	spi1_rx_done = 0;
    	spi2_tx_done = 0;
    	spi2_rx_done = 0;

        memcpy(txBuf1, cmd->pattern, cmd->pattern_len);
        if (memcmp(cmd->pattern, txBuf1, cmd->pattern_len) != 0)
             return TEST_FAILURE;

    	while (hspi2.State != HAL_SPI_STATE_READY) {}
        if (HAL_SPI_Receive_DMA(&hspi2, rxBuf2, cmd->pattern_len) != HAL_OK)
             return TEST_FAILURE;

        HAL_Delay(1);

        if (HAL_SPI_Transmit_DMA(&hspi1, txBuf1, cmd->pattern_len) != HAL_OK)
             return TEST_FAILURE;

        HAL_Delay(1);

        memcpy(txBuf2, rxBuf2, cmd->pattern_len);

        if (wait_flag(&spi1_tx_done, TIMEOUT) || wait_flag(&spi2_rx_done, TIMEOUT))
             return TEST_FAILURE;

        if (HAL_SPI_TransmitReceive_DMA(&hspi2, txBuf2,rxBuf2, cmd->pattern_len) != HAL_OK)
             return TEST_FAILURE;

        HAL_Delay(1);

        if (HAL_SPI_TransmitReceive_DMA(&hspi1, txBuf1,rxBuf1, cmd->pattern_len) != HAL_OK)
             return TEST_FAILURE;

        HAL_Delay(1);

        if (wait_flag(&spi2_tx_done, TIMEOUT) || wait_flag(&spi1_rx_done, TIMEOUT))
             return TEST_FAILURE;
        if (memcmp(cmd->pattern, rxBuf1, cmd->pattern_len) != 0)
             return TEST_FAILURE;
    }
    return TEST_SUCCESS;
}
