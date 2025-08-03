
# Peripherals testing program for STM32F756ZG

The program is testing the following peripherals:
• TIMER
• UART (using DMA)
• SPI (using DMA)
• I2C 
• ADC (using DMA)
• Eth (using DMA)

The user enters a number of iterations to test and a word pattern if required. 
The information is sent to the server and at the end of the test, a result is sent to the client. 
On the client side, a log file will be recorded with the following details: 
Test ID, date and time the test was sent, how long the test took and the test result (success or failure).

## Installation

Client side:
Use a Debian-based Linux machine.
Download the folder - linux_client
open a terminal from the folder and run Make.


Server side:
Download the folder - stm32_server
Inside the folder you will find another folder called peripherals_test, open it using cubemxIDE and burn the project to the STM32F756ZG card.

STM32F756ZG Pinout:
I2C2<->I2C4: PB10<->PF14, PB11<->PF15
SPI1<->SPI2: PA5<->PD3, PA6<->PC2, PB5<->PC3
UART3<->UART5: PD8<->PD2, PD9<->PC12
ADC: PA0<->3V
