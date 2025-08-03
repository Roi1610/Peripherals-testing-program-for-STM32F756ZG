/*
 * server.h
 *
 *  Created on: Jun 14, 2025
 *      Author: Roi
 */

#ifndef INC_SERVER_H_
#define INC_SERVER_H_

#define PORT 9000

/**
 * @brief - initializes a UDP server according
 * to a predefined IP address in CubeMx LWIP
 * and port that defined above
 */
void udp_server_init(void);

#endif /* INC_SERVER_H_ */
