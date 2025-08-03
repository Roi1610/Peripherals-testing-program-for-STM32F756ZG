/*
 * peripherals.h
 *
 *  Created on: Jun 14, 2025
 *      Author: Roi
 */

#ifndef INC_PERIPHERALS_H_
#define INC_PERIPHERALS_H_

#include "protocol.h"

#define TIMEOUT 1000


/**
 * @brief - function to test uart communication.
 * receives a pointer to a struct test_command_t
 * Transfers data to uart5 from uart2 via DMA. Then returns the data to uart2 from uart5 via DMA
 * and finally compares the data in the uart2 buffer with the data received from the client.
 */
uint8_t test_uart(const test_command_t *cmd);

/**
 * @brief - function to test spi communication.
 * receives a pointer to a struct test_command_t
 * Transfers data to spi2(slave) from spi1(master) via DMA. Then returns the data to spi1 from spi2 via DMA
 * and finally compares the data in the spi1 buffer with the data received from the client.
 */
uint8_t test_spi(const test_command_t *cmd);

/**
 * @brief - function to test i2c communication.
 * receives a pointer to a struct test_command_t
 * Transfers data to i2c4(slave) from i2c2(master). Then returns the data to i2c2 from i2c4
 * and finally compares the data in the uart2 buffer with the data received from the client.
 */
uint8_t test_i2c(const test_command_t *cmd);

/**
 * @brief - function to test adc.
 * adc pin connected to 3V
 * A measurement of a value on the pin is performed using DMA.
 * It is checked if we subtract the measured value from the expected value and get a deviation of less than 50
 */
uint8_t test_adc(const test_command_t *cmd);

/**
 * @brief - function to test timer.
 *  the timer is set to generate a clock every one second and change a flag.
 *  the clock is measured at a starting point and as long as the flag has not changed and we have not exceeded one second, the program waits.
 *  as soon as the flag changes, the clock is measured again and a check is made to see if the difference between the measurements took one second
 *  (with a precision of 100 milliseconds).
 *  This means that each iteration will take one second
 */
uint8_t test_timer(const test_command_t *cmd);

#endif /* INC_PERIPHERALS_H_ */
