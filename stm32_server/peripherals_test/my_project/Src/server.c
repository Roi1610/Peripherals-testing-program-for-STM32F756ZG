/*
 * server.c
 *
 *  Created on: Jun 14, 2025
 *      Author: Roi
 */

#include "lwip.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "stdio.h"
#include "server.h"
#include "protocol.h"
#include "peripherals.h"
#include <string.h>

extern ip4_addr_t ipaddr;

void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p,
    const ip_addr_t *addr, u16_t port){
	if (!p || !p ->payload){
		if (p) pbuf_free(p);
		return;
	}

	test_command_t *cmd = (test_command_t *)p->payload;
	    test_result_t result = {
	        .test_id = cmd->test_id,
	        .result = TEST_FAILURE
	};

	if (cmd->peripheral & PERIPHERAL_TIMER){
			result.result = test_timer(cmd);
	}
	if (cmd->peripheral & PERIPHERAL_UART){
		result.result = test_uart(cmd);
	}
	if (cmd->peripheral & PERIPHERAL_SPI){
			result.result = test_spi(cmd);
	}
	if (cmd->peripheral & PERIPHERAL_I2C){
			result.result = test_i2c(cmd);
	}
	if (cmd->peripheral & PERIPHERAL_ADC){
			result.result = test_adc(cmd);
	}
	if (cmd->peripheral & PING){
			result.result = TEST_SUCCESS;
	}


	struct pbuf *resp = pbuf_alloc(PBUF_TRANSPORT, sizeof(result), PBUF_RAM);
	if (resp) {
		memcpy(resp->payload, &result, sizeof(result));
	    udp_sendto(pcb, resp, addr, port);
	    pbuf_free(resp);
	}

	pbuf_free(p);
}

void udp_server_init(void){
	struct udp_pcb *pcb = udp_new();
	err_t bind;
	bind = udp_bind(pcb, &ipaddr, PORT);

	if (bind == ERR_OK)
	{
		udp_recv(pcb, udp_receive_callback, NULL);
	}
	else
	{
		udp_remove(pcb);
	}


}
