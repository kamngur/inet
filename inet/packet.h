#ifndef _PACKET_H_
#define _PACKET_H_
#include "types.h"

#include "ethernet_header.h"
#include "ip_header.h"
#include "udp_head.h"
#include "ncp.h"

#define ETHERNET_HEADER_SIZE sizeof(ethernet_header)

#define IP_HEADER_SIZE sizeof(ip_header)

#define UDP_HEADER_SIZE sizeof(udp_header)

#define NCP_HEADER_SIZE sizeof(ncp_header)


//typedef struct ncp_datagram {
//	ethernet_header ethernet;
//	ip_header ip;
//	udp_header udp;
//	ncp_header ncp;
//	void* ncp_data;
//}ncp_datagram;

PACKED_STRUCT(ncp_datagram);

void create_packiet(void *packiet,__uint32_t pack_len,void * data,__uint32_t data_len);
int send_big_data(void * data,__uint16_t data_len);

int filter_packiets(char *packet_data,__uint32_t pack_len);
#endif
