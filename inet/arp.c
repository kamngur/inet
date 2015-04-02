#ifndef _BIS_SWAP_H_
#define _BIS_SWAP_H_
//! Byte swap unsigned short
#include "packet.h"
#include "arp.h"


arp_header *get_arp_header(frame* req)
{
	arp_header * header = req->f_data+ETHER_HDR_LEN;

	return header;
}

int should_arp_res(arp_header *req)
{
	ip_address *ip = get_host_ip();

	if(memcmp(*ip, req->arp_target_ip,sizeof(ip_address)))
		return 0;
	else
		return 1;
}

void create_arp_res(frame *request,frame *response)
{
	ethernet_header *eth_req = request->f_data;
	ethernet_header *eth_res = response->f_data;
	arp_header *req = (char*)request->f_data + ETHERNET_HEADER_SIZE;
	arp_header *res = (char*)response->f_data + ETHERNET_HEADER_SIZE;
	ether_addr *arp_sender_eth = get_host_mac();
	ip_address *arp_sender_ip = get_host_ip();

	memcpy(&eth_res->eth_dest_addr,&eth_req->eth_src_addr,sizeof(ether_addr));
	memcpy(&eth_res->eth_src_addr,get_host_mac(),sizeof(ether_addr));

	eth_res->ether_type = 0x0608;

	res->arp_hard_type = req->arp_hard_type;
	res->arp_proto_type = req->arp_proto_type;
	res->arp_hard_size = req->arp_hard_size;
	res->arp_proto_size = req->arp_proto_size;
	res->arp_opcode = 0x0200;
	res->arp_sender_eth = *arp_sender_eth;
	res->arp_sender_ip = *arp_sender_ip;
	res->arp_target_addr = req->arp_sender_eth;
	res->arp_target_ip = req->arp_sender_ip;

	response->f_len = sizeof(ethernet_header) + sizeof(arp_header) + ETHER_CRC_LEN;

}

void create_arp_req(frame *fr)
{
	ethernet_header * eth = fr->f_data ;
	arp_header * req = 	fr->f_data + ETHER_HDR_LEN;

	memset(&eth->eth_dest_addr,0xff,sizeof(ether_addr));
	memcpy(&eth->eth_src_addr,get_host_mac(),sizeof(ether_addr));
	eth->ether_type = 0x0608;
	req->arp_hard_type 	= 0x0100 ;
	req->arp_proto_type = 0x0008 ;
	req->arp_hard_size 	= 0x06;
	req->arp_proto_size = 0x04;
	req->arp_opcode =  0x0100;
	memcpy(&req->arp_sender_eth,get_host_mac(),sizeof(ether_addr));
	req->arp_sender_ip = *(get_host_ip());
	memset(&req->arp_target_addr,0,sizeof(ether_addr));
	memcpy(&req->arp_target_ip,get_server_ip(),sizeof(ip_address));

	fr->f_len = sizeof(ethernet_header) + sizeof(arp_header) + ETHER_CRC_LEN;

}

#endif
