#ifndef _ARP_H_
#define _ARP_H_
//ARP
#include "ethernet_header.h"
#include "ethernet.h"
#include "ip_header.h"
#include "types.h"
#include "config.h"

typedef struct arp_header
{
	uint16_t	arp_hard_type;
	uint16_t	arp_proto_type;
	uint8_t		arp_hard_size;
	uint8_t		arp_proto_size;
	uint16_t	arp_opcode;
	ether_addr arp_sender_eth;
	ip_address arp_sender_ip;
	ether_addr arp_target_addr;
	ip_address arp_target_ip;

}arp_header;


void create_arp_req(frame *fr);

void create_arp_res(frame *request,frame *response);


#endif
