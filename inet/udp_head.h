/*
 * udp_head.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 */

#ifndef UDP_HEAD_H_
#define UDP_HEAD_H_

#include "types.h"
#include "ip_header.h"
/*
 * Udp protocol header.
 * Per RFC 768, September, 1981.
 */

#define UDP_MAX_MSG_SIZE  0xFFE3 //(65535-(20+8) = 65507)

typedef struct udp_header {
	uint16_t uh_sport;		/* source port */
	uint16_t uh_dport;		/* destination port */
	uint16_t uh_ulen;		/* udp length */
	uint16_t uh_crc;		/* udp checksum */
}udp_header;

void create_udp_header(udp_header *ptr , in_port_t src , in_port_t dst,uint16_t len,uint16_t crc);
uint16_t udp_checksum(const void *buff, uint32_t len, ip_address *src_addr, ip_address *dest_addr);
uint16_t udp_sum_calc( uint16_t buff[],uint16_t len_udp, uint16_t src_addr[],uint16_t dest_addr[] );
#endif /* UDP_HEAD_H_ */
