#ifndef _PACKET_H_
#define _PACKET_H_


#include "queue.h"
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


typedef struct frame
{
	TAILQ_ENTRY(frame) f_tail;
	uint16_t f_len;
	uint16_t f_maxlen;
	void * f_data;
}frame;

//PACKED_STRUCT(ncp_datagram);

/*
* Inits queues (free and rx queue) and adds 70 element to free queue
*/
void init_lists();
/*
* Removes frame from current queu and adds to free frames queue
*/
void release_frame(frame *ptr);
/*
* Gets first element form free queue
*/
frame * get_free_frame();
/*
* gets firs recived frame
*/
frame* get_rx_frame();
/*
* Adds frame at queue end (tail)
*/
void add_rx_frame( frame * ptr);


/*
* Simple function to test creation of ethernet/ip/upd/ncp packiet
*/

int create_packiet(void *packet_data,uint32_t pack_len,void * data,uint32_t data_len);
/*
*	Function to test fragmentation in ip/udp
*/
int send_big_data(void * data,uint16_t data_len);
/*
* Function to get pointers to each header
*/
void get_headers(char * data, ethernet_header ** eth, ip_header ** ip, udp_header ** udp, ncp_header ** ncp);


/*
* Function to filter off packet that aren't IPv4/UPD 
*/
int filter_packiets(char *packet_data,uint32_t pack_len);



#endif
