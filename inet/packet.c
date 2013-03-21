/*
 * ethernet.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 */

#include "packet.h"
#include "bits_swap.h"
#include "types.h"
#include "udp_head.h"
#include "ip_header.h"
#include "ethernet_header.h"

//! This is the IP address of this host (expressed in network format).
//static ip_address host_ip = {192,168,10,101};
//static ip_address localhost = {127,0,0,1};
//! This is the IP netmask of this host (expressed in network format).
//static ip_address host_netmask =  {0,0,0,0};




void create_packiet(void *packet,__uint32_t pack_len,void * data,__uint32_t data_len)
{
	__uint8_t* ptr= packet;
	ethernet_header * eth = ptr;
	ip_header * ip = (ip_header *)(ptr+ETHER_HDR_LEN); 
	udp_header * udp = (ptr+ETHER_HDR_LEN+ sizeof(ip_header));
	unsigned int sss = sizeof(ip_header);
	__uint16_t udp_crc=0;
	__uint32_t eth_crc=0;
	__uint32_t m_data_len= pack_len - ETHER_HDR_LEN - 20 - 8;

	//ether_addr src={0x00,0x01,0x00,0x01,0x00,0x01};
	ether_addr src={0x00,0x01,0x00,0x01,0x00,0x01};
	ether_addr dst={0x00,0x00,0x00,0x00,0x00,0x00};
	//init_data();
	create_ethernet_header(eth,&src,&dst,ETHER_TYPE_IPV4);
	init_ip_header(ip);
	
	create_udp_header(udp, 8081,8081,m_data_len,0);
	udp_crc=udp_checksum(udp,data_len+8,ip->ip_src,ip->ip_dst);
	udp->uh_crc=udp_crc;
	if(data_len>pack_len-14-20-8-4) //TODO wtf are those values
		memcpy((udp+8),data,pack_len-14-20-8);
	else
		memcpy(((char*)udp+8),data,data_len);

	eth_crc = ethernet_checksum(ptr,pack_len);
	ptr = ptr + pack_len -4;
	*ptr++=(__uint8_t)((eth_crc&0xff000000)>>24);
	*ptr++=(__uint8_t)((eth_crc&0x00ff0000)>>16);
	*ptr++=(__uint8_t)((eth_crc&0x0000ff00)>>8);
	*ptr++=(__uint8_t)((eth_crc&0x000000ff));
	


	
}
