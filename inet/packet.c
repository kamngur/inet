/*
 * ethernet.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 */
//#include <dm9000a.h>
#include "packet.h"
#include "bits_swap.h"
#include "types.h"
#include "udp_head.h"
#include "ip_header.h"
#include "ethernet_header.h"
#include "queue.h"
#include "arp.h"
//#include "packet.h"


#ifndef _DEBUG
#define _DEBUG
#endif

//! This is the IP address of this host (expressed in network format).
//static ip_address host_ip = {192,168,10,101};
//static ip_address localhost = {127,0,0,1};
//! This is the IP netmask of this host (expressed in network format).
//static ip_address host_netmask =  {0,0,0,0};

volatile TAILQ_HEAD(free_frames,frame) free_frames;
volatile TAILQ_HEAD(rx_frames,frame) rx_frames;
#ifdef _DEBUG
volatile uint32_t frames_count = 0;
volatile int32_t rx_count = 0;
volatile int32_t free_count = 0;
#endif



void init_lists()
{
	int i = 0;  
	volatile frame * ptr;


	TAILQ_INIT(&free_frames);
	TAILQ_INIT(&rx_frames);
//	LIST_HEAD_INITIALIZER((&free_frames));
//	LIST_HEAD_INITIALIZER((&rx_frames));

	for(i =0; i<65; i++)
	{
		ptr = malloc(sizeof(frame));
		if(ptr != 0)
		{
			ptr->f_maxlen = ETHER_MAX_LEN;
			ptr->f_data = malloc(ETHER_MAX_LEN);
			ptr->f_len =0;
#ifdef _DEBUG
			frames_count++;
			free_count++;

#endif 
			TAILQ_INSERT_HEAD(&free_frames,ptr,f_tail);
			

		}
		else
		{
			printf("%s: Error can't alloc frames\n",__FUNCTION__);
		}
	}
}

void release_frame(frame *ptr)
{
	frame * m_ptr;
	if(ptr == 0)
		return;

	ptr->f_len = 0;
#ifdef _DEBUG
	free_count++;
#endif
	TAILQ_INSERT_TAIL(&free_frames,ptr,f_tail);
}

frame * get_free_frame()
{

    frame * ptr = TAILQ_FIRST(&free_frames);

	if(ptr == 0)
	{
		ptr = malloc(sizeof(frame));
			if(ptr != 0)
			{
				ptr->f_maxlen = ETHER_MAX_LEN;
				ptr->f_data = malloc(ETHER_MAX_LEN);
                if(ptr->f_data == 0)
                    printf("%s: We have problem\n",__FUNCTION__);
				ptr->f_len =0;
#ifdef _DEBUG
				frames_count++;
#endif
				printf("%s: Allocating new frame\n",__FUNCTION__);
			}
			else
			{
				printf("%s: Error can't alloc frames\n",__FUNCTION__);
			}
	}
	else
	{
#ifdef _DEBUG
		free_count--;
#endif
		TAILQ_REMOVE(&free_frames,ptr,f_tail);
	}

	return ptr;
}


/**
 Add frame to rx_frame queue (
*/
frame* get_rx_frame()
{

	volatile frame* ptr = TAILQ_FIRST(&rx_frames);
	if(ptr ==0)
	{
		//printf("%s: Error no rx frames to get\n",__FUNCTION__);
		return 0;
	}
	TAILQ_REMOVE(&rx_frames,ptr,f_tail);
	
#ifdef _DEBUG
	rx_count--;
#endif
	return ptr;
}
/**
 Add frame to rx_frame queue (
*/

void add_rx_frame( frame * ptr)
{
	//TAILQ_REMOVE(ptr,f_tail);
	TAILQ_INSERT_TAIL(&rx_frames,ptr,f_tail);

#ifdef _DEBUG
	rx_count++;
	//printf("%s: added frame \n",__FUNCTION__);
#endif

}





int create_packiet(void *packet_data,uint32_t pack_len,void * data,uint32_t data_len)
{
	char* ptr= (char *)packet_data;
	ethernet_header * eth;
	ip_header * ip ;
	udp_header * udp ;


	unsigned int ip_hdr_size = IP_HEADER_SIZE;
	unsigned int ip_hdr_size2 = 0;
	uint32_t m_data_len= pack_len - ETHER_HDR_LEN - IP_HEADER_SIZE - UDP_HEADER_SIZE;
	

	//unsigned int sss = sizeof(ip_header);
	//uint8_t *my_ptr=0;
	//uint16_t udp_crc=0;
	//uint32_t eth_crc=0;
	

	//ether_addr src={0x00,0x01,0x00,0x01,0x00,0x01};
	//ether_addr src = {0x00,0x00,0x00,0x00,0x00,0x00};
	//ether_addr dst = {0x00,0x01,0x00,0x01,0x00,0x01};



	if(data_len > pack_len - ETHER_HDR_LEN-IP_HEADER_SIZE-UDP_HEADER_SIZE )
	{
		m_data_len = pack_len - ETHER_HDR_LEN-IP_HEADER_SIZE-UDP_HEADER_SIZE ;
	}
	else
	{
		//m_data_len = data_len-UDP_HEADER_SIZE;
		m_data_len = data_len;
	}

	get_headers((char * )packet_data, &eth,&ip , &udp, &ptr);
	
	create_ethernet_header(eth,get_host_mac(),get_server_mac(),ETHER_TYPE_IPV4);
	init_ip_header(ip);
	 ip_hdr_size2 = ip->ip_hl& 0xF;
	
	 ip->ip_len = swap_int16(IP_HEADER_SIZE + UDP_HEADER_SIZE + m_data_len ); // why ??
	 ip->ip_crc =ip_checksum (ip,sizeof(ip_header));

	create_udp_header(udp, (in_port_t) get_host_port(),(in_port_t) get_server_port(),data_len,0);

	memcpy(ptr,data,(size_t)m_data_len);

	udp->uh_crc = udp_checksum((void *)udp,data_len+UDP_HEADER_SIZE,&ip->ip_src,&ip->ip_dst);//xxx




	return m_data_len + ETHER_HDR_LEN + IP_HEADER_SIZE + UDP_HEADER_SIZE + ETHER_CRC_LEN;
	


	
}

/*
* Filter all packet that don't check 
*	- must be ethernet 
*	- must be IPv4	(or arp ??)
*	- must be from server
*	- must be this MAC
*	
*	- must be to this IP adress
*	- must be UDP (fragmented)
*
*/
int filter_packiets(char* packet_data,uint32_t pack_len)
{

	
	//uint32_t payload_len = 0;
	//uint32_t offset = 0;
	//uint32_t packet_len =ETHER_MAX_LEN;
	//uint16_t flags =0;
	uint16_t port = 0;
    uint16_t port_s = 0 ;

	int u = IP_HDR_VER;
	void * data_ptr =0;


	
	//ncp_datagram* ptr= &packet;
	ethernet_header * eth;
	arp_header * arp;
	ip_header * ip;
	udp_header *udp;
	ip_address * addres  = get_host_ip();

	int i;

	get_headers(packet_data, &eth, &ip, &udp ,0);

	if(eth->ether_type == 0x0608 ) //ARP
	{
		arp = ip;
		if (memcmp(&arp->arp_target_ip ,get_host_ip(),sizeof(ip_address)) == 0)
		{
			return 10;
		};
		return -1 ;//if arp its ok
	}

	if (eth->ether_type != 0x0008 ) //ethernet //swap(ETHER_TYPE) 
	{

		return 1;
	}

	if( (ip->ip_hl&0xF0) != (IP_HDR_VER<<4))
	{
		return 2;
	}
	if(ip->ip_proto != IPPROTO_UDP)
	{
		return 3;
	}

	//i = memcmp(&(ip->ip_dst),addres,sizeof(ip_address));
	//if(i != 0)
	//{
	//	return 4;
	//}
	port = swap_uint16(udp ->uh_dport);
    port_s = get_host_port();

	if( port != port_s)
	{
		return 5;
	}
	printf("get intresting data\n");

	//udp_header * udp = (udp_header * )ip + ip->ip_hl*4;
	//ncp_header * ncp = (ncp_header *) udp + UDP_HEADER_SIZE;



	return 0;
}

/*
int  create_udp_datagram(void *packet_data,uint32_t pack_len,void * payload,uint32_t data_len)
{
	ncp_datagram* ptr= packet_data;
	ethernet_header * eth =  &(ptr->ethernet);
	ip_header * ip =(ip_header*) &(ptr->ip);
	udp_header * udp =(udp_header *) &(ptr->payload);
	unsigned int sss = sizeof(ip_header);
	uint8_t *my_ptr=0;
	uint16_t udp_crc=0;
	uint32_t eth_crc=0;
	uint32_t m_data_len= pack_len - ETHER_HDR_LEN - IP_HEADER_SIZE - UDP_HEADER_SIZE;

	//ether_addr src={0x00,0x01,0x00,0x01,0x00,0x01};
	ether_addr src={0x00,0x01,0x00,0x01,0x00,0x01};
	ether_addr dst={0x00,0x00,0x00,0x00,0x00,0x00};


	create_ethernet_header(eth,&src,&dst,ETHER_TYPE_IPV4);
	init_ip_header(ip);

	create_udp_header(udp, 8081,8081,m_data_len,0);
	udp_crc=udp_checksum(udp,data_len+8,ip->ip_src,ip->ip_dst);
	udp->uh_crc=udp_crc;
	if(data_len>pack_len-ETHER_HDR_LEN-IP_HEADER_SIZE-UDP_HEADER_SIZE-4)
		memcpy(ptr->payload.udp.data,payload,(size_t)m_data_len);
	else
		memcpy(ptr->payload.udp.data,payload,(size_t)data_len);

	eth_crc = ethernet_checksum(ptr,pack_len);
	my_ptr = ptr + pack_len -4;
	*my_ptr++=(uint8_t)((eth_crc & 0xff000000)>>24);
	*my_ptr++=(uint8_t)((eth_crc & 0x00ff0000)>>16);
	*my_ptr++=(uint8_t)((eth_crc & 0x0000ff00)>>8);
	*my_ptr++=(uint8_t)((eth_crc & 0x000000ff));



}
*/


int send_big_data(void * data,uint16_t data_len)
{
	uint32_t payload_len = 0;
	uint32_t offset = 0;
	uint32_t packet_len =ETHER_MAX_LEN;
	uint16_t flags =0;
	void * data_ptr =0;
	char packet[ETHER_MAX_DIX_LEN];
	//ncp_datagram* ptr= &packet;
	ethernet_header * eth ;
	ip_header * ip;
	udp_header * udp ;
	ncp_header * ncp;
	
	uint16_t pack_payload_start;
	ether_addr src = {0x00,0x01,0x00,0x01,0x00,0x01};
	ether_addr dst = {0x00,0x00,0x00,0x00,0x00,0x00};


	get_headers((char*) data, &eth, &ip, &udp, &ncp );
	create_ethernet_header(eth,&src,&dst,ETHER_TYPE_IPV4);
	init_ip_fragment_header(ip,IP_MF,0,data_len);

	for(pack_payload_start = 0 ; pack_payload_start <= data_len ; pack_payload_start += IP_PAYLOAD_MAX)
	{

		if(data_len - pack_payload_start > IP_PAYLOAD_MAX)
		{
			payload_len = IP_PAYLOAD_MAX;
			flags = IP_MF;
		}
		else
		{
			payload_len = data_len - pack_payload_start;
			flags = 0 ;
		}

		if(pack_payload_start  == 0) //add udp header and copy data
		{
			payload_len -= UDP_HEADER_SIZE;
			create_udp_header(udp, 8081,8082,data_len,0);

			data_ptr = ncp + NCP_HEADER_SIZE;
		}
		else	// set IP fragmentation and copy data
		{

			ip->ip_off = swap_uint16((flags && 0xE000)|(offset & 0x1FFF));
			data_ptr =  udp;
		}
	//	memcpy(data_ptr,((uint8_t)offset+offset),payload_len);

		offset += payload_len/8;
	//	TransmitPacket(ptr,ETHER_MAX_LEN); //TODO
	}

	return 0;

}




void get_headers(char * data, ethernet_header ** eth, ip_header ** ip, udp_header ** udp, ncp_header ** ncp )
{
	unsigned int s1 = ETHER_HDR_LEN;
	unsigned int s2 = IP_HEADER_SIZE;
	unsigned int s3 = UDP_HEADER_SIZE;
	if(data == 0)
		return;

	if(eth != 0)
		*eth = data;

	if(ip != 0)
		*ip  = data + ETHER_HDR_LEN;

	if(udp != 0)
		*udp = data + ETHER_HDR_LEN + IP_HEADER_SIZE;
	
	if(ncp != 0)
		*ncp = data + ETHER_HDR_LEN + IP_HEADER_SIZE + UDP_HEADER_SIZE;

}

