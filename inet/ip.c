/*
 * ethernet.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 */

#include "ip_header.h"
#include "bits_swap.h"

//
////! This is the IP address of this host (expressed in network format).
//static ip_address host_ip = {192,168,10,101};
//static ip_address localhost = {127,0,0,1};
////! This is the IP netmask of this host (expressed in network format).
//static ip_address host_netmask =  {0,0,0,0};
//
//void set_host_ip_net(ip_address ip, ip_address netmask)
//{
//	host_ip = ip;
//	host_netmask = netmask;
//}
//
////! \brief Get the IP address of this host.
////! \return
////!	The IP address expressed in network format.
//ip_address get_host_ip()
//{
//	return( host_ip );
//}
//
////! \brief Get the netmask of this host.
////! \return
////!	The netmask expressed in network format.
//ip_address get_host_netmask()
//{
//	return( host_netmask );
//}
//
////! \brief Get the network address of this host.
////! \return
////!	The network expressed in network format.

void dbg_ip_addres(ip_address * ptr)
{
	if(ptr)
	{
		printf("%d.%d.%d.%d\n",
			ptr->byte1,
			ptr->byte2,
			ptr->byte3,
			ptr->byte4);
	}
	else
	{
		printf("dbg_ip_addres: null pointer\n");
	}

}

void dbg_ip_header(ip_header * ptr)
{
	if(ptr)
	{	
		printf("\n");
		printf("IP HEADER\n");
		printf("Header length:\t %#x\n",ptr->ip_hl);
		//printf("Header version:\t %d \n",ptr->ip_v);
		printf("TypOfService:\t %d \n",ptr->ip_tos);
		printf("Total length:\t %d \n",swap_uint16( ptr->ip_len));
		printf("Identyfication:\t %d \n",swap_uint16(ptr->ip_id));
		printf("Flags:\t %#4x \n",ptr->ip_off);
		printf("TTL:\t\t %d \n",ptr->ip_ttl);
		printf("Protocol:\t %#x \n",ptr->ip_proto);
		printf("Source addres:\t ");
		dbg_ip_addres(&ptr->ip_src);
		printf("Dest addres:\t ");
		dbg_ip_addres(&ptr->ip_dst);
		//printf("Flags + padd:\t %#8x \n",ptr->ip_options);
		printf("\n");
	}
	else
	{
		printf("dbg_ethernet_header: null pointer\n");
	}
	
}

__uint16_t ip_checksum(const void *buf, size_t hdr_len)
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

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return(~sum);
}


void init_ip_header(ip_header * ptr)
{
//ned to swap to net byte order
	ip_address *src = get_host_ip();
	ip_address *dst = get_server_ip();

	ptr->ip_hl	= (IP_HDR_VER << 4) | IP_HDR_IHL  ;
	//ptr->ip_v	= 0x4;
	ptr->ip_tos	= 0x00;
	ptr->ip_len = 181;
	ptr->ip_id	= 0;
	ptr->ip_off =swap_uint16(IP_DF);
	ptr->ip_ttl	= 0x80;  // 128 <-> local  net
	ptr->ip_proto = IPPROTO_UDP;
	ptr->ip_crc = 0; //later
	ptr->ip_src = *src;
	ptr->ip_dst = *dst; //localhost
//	ptr->ip_options = 0x0;
	
	ptr->ip_crc =ip_checksum (ptr,sizeof(ip_header));


}

