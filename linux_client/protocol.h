#ifndef PROTOCOL_H
#define PROTOCOL_H  

#include <stdint.h>

#define MAX_PATTERN_LEN 256

#define PERIPHERAL_TIMER 0x01
#define PERIPHERAL_UART 0x02
#define PERIPHERAL_SPI 0x04
#define PERIPHERAL_I2C 0x08
#define PERIPHERAL_ADC 0x10
#define PING 0x20

#define TEST_SUCCESS 0x01
#define TEST_FAILURE 0xFF

/**
 * @brief struct that contains the information
 * that is passed to UUT
 * @param test_id - identifies the test
 * @param peripheral - the tested periphery is represented in bitfield
 * @param iterations - the number of iterations that the test will run
 * @param pattern_len - the size of bit pattern string sent to the UUT
 * @param pattern - the string of characters that sent to  the UUT
 */
C {
    uint32_t test_id;
    uint8_t peripheral;
    uint8_t iterations;
    uint8_t pattern_len;
    uint8_t pattern[MAX_PATTERN_LEN];
} test_command_t;

/**
 * @brief struct that contains the information
 * that received from the UUT
 * @param test_id - identifies the test
 * @param result - success or failure
 */
typedef struct __attribute__((packed)) {
    uint32_t test_id;
    uint8_t result;
} test_result_t;

#endif