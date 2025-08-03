/*
 * test_uart.c
 *
 *  Created on: Jun 17, 2025
 *      Author: Roi
 */


#include "peripherals.h"
#include "usart.h"
#include <string.h>


extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart5;



static volatile uint8_t u2_tx_done,  u2_rx_done,  u5_rx_done, u5_tx_done;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2) u2_tx_done = 1;
    else if (huart == &huart5) u5_tx_done = 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart5) u5_rx_done = 1;
    else if (huart == &huart2) u2_rx_done = 1;
}

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


uint8_t test_uart(const test_command_t *cmd){
    uint8_t rxBuf2[MAX_PATTERN_LEN] = {};
    uint8_t rxBuf5[MAX_PATTERN_LEN] = {};

    for(uint8_t i = 0; i < cmd->iterations ; ++i){
        memset(rxBuf2, 0, sizeof(rxBuf2));
        memset(rxBuf5, 0, sizeof(rxBuf5));


        if (HAL_UART_Receive_DMA(&huart5, rxBuf5, cmd->pattern_len) != HAL_OK)
            return TEST_FAILURE;

        if (HAL_UART_Transmit_DMA(&huart2, cmd->pattern, cmd->pattern_len) != HAL_OK)
            return TEST_FAILURE;

        if (wait_flag(&u2_tx_done, TIMEOUT) || wait_flag(&u5_rx_done, TIMEOUT))
            return TEST_FAILURE;


        if (HAL_UART_Receive_DMA(&huart2, rxBuf2, cmd->pattern_len) != HAL_OK)
            return TEST_FAILURE;

        if (HAL_UART_Transmit_DMA(&huart5, rxBuf5, cmd->pattern_len) != HAL_OK)
            return TEST_FAILURE;

        if (wait_flag(&u5_tx_done, TIMEOUT) || wait_flag(&u2_rx_done, TIMEOUT))
            return TEST_FAILURE;

        if (memcmp(cmd->pattern, rxBuf2, cmd->pattern_len) != 0)
            return TEST_FAILURE;
    }

    return TEST_SUCCESS;
}
