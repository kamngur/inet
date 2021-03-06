/*
 * ethernet.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 */

#include "udp_head.h"
#include "ip_header.h"
#include "bits_swap.h"
#include "types.h"




void dbg_udp_packet(udp_header *packet, ip_address ip_src, ip_address ip_dst)
{
	uint16_t len = ntohs(packet->uh_ulen) - sizeof(udp_header);
	int i;

	printf("\nUDP packet received (%u bytes of data)"
		"\nsender  = %u.%u.%u.%u:%u"
		"\nreceiver= %u.%u.%u.%u:%u",
		len,

		(ip_src.byte1), ip_src.byte2,ip_src.byte3,ip_src.byte4,
		ntohs(packet->uh_sport),

		(ip_dst.byte1), ip_dst.byte2,ip_dst.byte3,ip_dst.byte4,
		ntohs(packet->uh_dport)
		);
	printf("\nData:\n");
	for( i = 0; i < len; i++ )
		putchar( (((uint8_t *)packet)+sizeof(udp_header))[i]  );
}

/*
 data - udp header + data
 length - length of udp header + data
*/


void create_udp_header(udp_header *ptr , in_port_t src , in_port_t dst,uint16_t len,uint16_t crc)
{
	ptr->uh_dport	= swap_uint16(dst);
	ptr->uh_sport	= swap_uint16(src);
    
	ptr->uh_ulen	= swap_uint16(len+8);
	ptr->uh_crc = 0x0;

}

uint16_t udp_checksum(const void *buff, uint32_t len, ip_address *src_addr, ip_address *dest_addr)
{
	const uint16_t *buf=buff;
	uint16_t *ip_src=(void *)src_addr, *ip_dst=(void *)dest_addr;
	uint32_t sum;
	uint32_t length=len; //wyrazone w oktetach

	// Calculate the sum						//
	sum = 0;

	// Add the pseudo-header					//
	sum +=swap_uint16(*(ip_src++));
	sum +=swap_uint16(*ip_src);

	sum += swap_uint16(*(ip_dst++));
	sum += swap_uint16(*ip_dst);

	sum += (IPPROTO_UDP);
	sum += (length);

	while (len > 1)
	{
		sum += swap_uint16(*buf++);
		if (sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);
		len -= 2;
	}

	if ( len & 1 )
		// Add the padding if the packet lenght is odd		//
		sum += *((uint8_t *)buf);


	// Add the carries						//
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	// Return the one's complement of sum				//
	return  swap_uint16((uint16_t)(~sum))  ;
}


uint16_t udp_sum_calc( uint16_t buff[],uint16_t len_udp, uint16_t src_addr[],uint16_t dest_addr[] )
{
	uint16_t prot_udp=17;
	uint16_t padd=0;
	uint16_t word16;
	uint32_t sum;	
	unsigned i;

	// Find out if the length of data is even or odd number. If odd,
	// add a padding byte = 0 at the end of packet
	//if (padding&1==1){
	//	padd=1;
	//	buff[len_udp]=0;
	//}

	//initialize sum to zero
	sum=0;

	// make 16 bit words out of every two adjacent 8 bit words and 
	// calculate the sum of all 16 vit words
	for (i=0;i<len_udp+padd;i=i+2){
		word16 =((buff[i]<<8)&0xFF00)+(buff[i+1]&0xFF);
		sum = sum + (unsigned long)word16;
	}	
	// add the UDP pseudo header which contains the IP source and destinationn addresses
	for (i=0;i<4;i=i+2){
		word16 =((src_addr[i]<<8)&0xFF00)+(src_addr[i+1]&0xFF);
		sum=sum+word16;	
	}
	for (i=0;i<4;i=i+2){
		word16 =((dest_addr[i]<<8)&0xFF00)+(dest_addr[i+1]&0xFF);
		sum=sum+word16; 	
	}
	// the protocol number and the length of the UDP packet
	sum = sum + prot_udp + len_udp;

	// keep only the last 16 bits of the 32 bit calculated sum and add the carries
	while (sum>>16)
		sum = (sum & 0xFFFF)+(sum >> 16);

	// Take the one's complement of sum
	sum = ~sum;

	return ((uint16_t) sum);
}
