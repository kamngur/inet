/*
 * ethernet_header.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 *      //based on open bsd definitions /sys/netinet/if_ether.h
 */

#ifndef ETHERNET_HEADER_H_
#define ETHERNET_HEADER_H_

#include "types.h"

#define ETHER_PREAMBLE_LEN 8
#define	ETHER_ADDR_LEN	6	/* Ethernet address length		*/
#define ETHER_TYPE_LEN	2	/* Ethernet type field length		*/
#define ETHER_CRC_LEN	4	/* Ethernet CRC length			*/
#define ETHER_HDR_LEN	14

#define ETHER_MIN_LEN	64	/* Minimum frame length, CRC included	*/
#define ETHER_MAX_LEN	1518	/* Maximum frame length, CRC included	*/
#define ETHER_MAX_DIX_LEN	1536	/* Maximum DIX frame length	*/
#define ETHER_HDR_OFF		0		/* Offset of ethernet header*/



//const __uint8_t ethernet_preamble[ETHER_PREAMBLE_LEN]={0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAB};

#define ETHER_TYPE_IPV4 0x0800
#define ETHER_TYPE_ARP	0x0806

typedef struct ether_addr {

	__uint8_t ether_addr_octet[ETHER_ADDR_LEN];

}ether_addr;


typedef struct ethernet_header
{
	ether_addr eth_dest_addr;
	ether_addr eth_src_addr;
	__uint16_t ether_type;
}ethernet_header;

void create_ethernet_header(ethernet_header * , ether_addr * , ether_addr * , __uint16_t  );

/*Functions for debug*/
void dbg_ethernet_addres(ether_addr *);
void dbg_ethernet_header(ethernet_header *);


__uint32_t ethernet_checksum(const void *buf, size_t hdr_len);
#endif /* ETHERNET_HEADER_H_ */
