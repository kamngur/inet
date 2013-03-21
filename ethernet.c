/*
 * ethernet.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 */

#include "ethernet_header.h"
#include "bits_swap.h"

void dbg_ethernet_addres(ether_addr * ptr)
{
	if(ptr)
	{
		printf("%2X-%2X-%2X-%2X-%2X-%2X\n",
			ptr->ether_addr_octet[0],
			ptr->ether_addr_octet[1],
			ptr->ether_addr_octet[2],
			ptr->ether_addr_octet[3],
			ptr->ether_addr_octet[4],
			ptr->ether_addr_octet[5]);
	}
	else
	{
		printf("dbg_ethernet_addres: null pointer\n");
	}

}

void dbg_ethernet_header(ethernet_header * ptr)
{
	if(ptr)
	{
		printf("\n\n");
		printf("ETHERNET HEADER \n");
		printf("Source MAC: \t");
		dbg_ethernet_addres(&(ptr->eth_src_addr));
		printf("Destination MAC: ");
		dbg_ethernet_addres(&(ptr->eth_dest_addr));
		printf("Ethernet type: \t%#4X\n",swap_uint16(ptr->ether_type));
		printf("\n");
	}
	else
	{
		printf("dbg_ethernet_header: null pointer\n");
	}
	
}


void create_ethernet_header(ethernet_header *ptr , ether_addr *src , ether_addr *dst , __uint16_t type)
{
	ptr->eth_dest_addr	= *dst;
	ptr->eth_src_addr	= *src;
	ptr->ether_type	= swap_uint16(type); //default 0x0800
}


__uint32_t ethernet_checksum(const void *buf, size_t hdr_len)
{
	unsigned long sum = 0;
	const __uint16_t *ip1;

	ip1 = buf;
	while (hdr_len > 1)
	{
		sum += *ip1++;
		if (sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);
		hdr_len -= 2;
	}

//	while (sum >> 16)
//		sum = (sum & 0xFFFF) + (sum >> 16);

	return(~sum);
}