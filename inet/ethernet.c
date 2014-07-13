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


void create_ethernet_header(ethernet_header *ptr , ether_addr *src , ether_addr *dst , uint16_t type)
{
	ptr->eth_dest_addr	= *dst;
	ptr->eth_src_addr	= *src;
	ptr->ether_type	= swap_uint16(type); //default 0x0800
}





// Generating polynomial:
const uint32_t ethernet_polynomial_le = 0xedb88320U;

//bit-oriented implementation: processes a byte array.
unsigned ether_crc_le(int length, char *data, int foxes)
{
    unsigned int crc = (foxes) ? 0xffffffff: 0;	/* Initial value. */
    while(--length >= 0) 
    {
        unsigned char current_octet = *data++;
        int bit;
        // printf("%02X, %08X,  inv %08X\n", current_octet, crc, ~crc);

        for (bit = 8; --bit >= 0; current_octet >>= 1) {
            if ((crc ^ current_octet) & 1) {
                crc >>= 1;
                crc ^= ethernet_polynomial_le;
            } else
                crc >>= 1;
        }
    }
    // printf("crc final %x\n", crc);
    return crc;
}

uint32_t ethernet_checksum(const void *buf, size_t hdr_len)
{
	unsigned long sum = 0xffffffff;
	const uint16_t *ip1;

	ip1 = buf;
	while (hdr_len > 1)
	{
		sum += *ip1++;
		if (sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);
		hdr_len -= 2;
	}

	//while (sum >> 16)
	//	sum = (sum & 0xFFFF) + (sum >> 16);

	return(~sum);
}


uint32_t 
ether_crc32_le_update(const uint8_t *buf, size_t len)
{
      uint32_t crc = 0xffffffff;
    static const uint32_t crctab[] = {
        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
        0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
        0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
    };
    size_t i;

    for (i = 0; i < len; i++) {
        crc ^= buf[i];
        crc = (crc >> 4) ^ crctab[crc & 0xf];
        crc = (crc >> 4) ^ crctab[crc & 0xf];
    }

    return (crc);
}

//uint32_t __pure
//ether_crc32_be_update(const u_int8_t *buf, size_t len)
//{
//    u_int32_t crc = 0xffffffff;
//    static const u_int8_t rev[] = {
//        0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
//        0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
//    };
//    static const u_int32_t crctab[] = {
//        0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
//        0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
//        0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
//        0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd
//    };
//    size_t i;
//    u_int8_t data;
//
//    for (i = 0; i < len; i++) {
//        data = buf[i];
//        crc = (crc << 4) ^ crctab[(crc >> 28) ^ rev[data & 0xf]];
//        crc = (crc << 4) ^ crctab[(crc >> 28) ^ rev[data >> 4]];
//    }
//
//    return (crc);
//}