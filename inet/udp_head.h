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
	__uint16_t uh_sport;		/* source port */
	__uint16_t uh_dport;		/* destination port */
	__uint16_t uh_ulen;		/* udp length */
	__uint16_t uh_crc;		/* udp checksum */
}udp_header;

void create_udp_header(udp_header *ptr , in_port_t src , in_port_t dst,__uint16_t len,__uint16_t crc);
__uint16_t udp_checksum(const void *buff, __uint32_t len, ip_address *src_addr, ip_address *dest_addr);
__uint16_t udp_sum_calc( __uint16_t buff[],__uint16_t len_udp, __uint16_t src_addr[],__uint16_t dest_addr[] );
#endif /* UDP_HEAD_H_ */
