/*
 * ip_header.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 */

#ifndef IP_HEADER_H_
#define IP_HEADER_H_

#include "types.h"
#include "pack.h"

typedef __uint8_t	in_addr_t;	/* base type for internet address */
typedef __uint8_t	in_port_t;	/* IP port type */

#define IP_HDR_OFFSET 14
#define IP_HDR_LENGHT_PSEUDOHEADER 16
#define IP_HDR_LENGHT 20
#define IP_HDR_VER	0x4
#define IP_HDR_IHL	0x5
/*
 * Protocols
 */
#define	IPPROTO_IP		0		/* dummy for IP */
#define IPPROTO_HOPOPTS		IPPROTO_IP	/* Hop-by-hop option header */
#define	IPPROTO_ICMP		1		/* control message protocol */
#define	IPPROTO_IGMP		2		/* group mgmt protocol */
#define	IPPROTO_GGP		3		/* gateway^2 (deprecated) */
#define	IPPROTO_IPIP		4		/* IP inside IP */
#define	IPPROTO_IPV4		IPPROTO_IPIP	/* IP inside IP */
#define	IPPROTO_TCP		6		/* tcp */
#define	IPPROTO_EGP		8		/* exterior gateway protocol */
#define	IPPROTO_PUP		12		/* pup */
#define	IPPROTO_UDP		17		/* user datagram protocol */
#define	IPPROTO_IDP		22		/* xns idp */
#define	IPPROTO_TP		29 		/* tp-4 w/ class negotiation */
#define IPPROTO_IPV6		41		/* IPv6 in IPv6 */
#define IPPROTO_ROUTING		43		/* Routing header */
#define IPPROTO_FRAGMENT	44		/* Fragmentation/reassembly header */
#define IPPROTO_RSVP		46		/* resource reservation */
#define	IPPROTO_GRE		47		/* GRE encap, RFCs 1701/1702 */
#define	IPPROTO_ESP		50		/* Encap. Security Payload */
#define	IPPROTO_AH		51		/* Authentication header */
#define	IPPROTO_MOBILE		55		/* IP Mobility, RFC 2004 */
#define IPPROTO_ICMPV6		58		/* ICMP for IPv6 */
#define IPPROTO_NONE		59		/* No next header */
#define IPPROTO_DSTOPTS		60		/* Destination options header */
#define	IPPROTO_EON		80		/* ISO cnlp */
#define IPPROTO_ETHERIP		97		/* Ethernet in IPv4 */
#define	IPPROTO_ENCAP		98		/* encapsulation header */
#define IPPROTO_PIM		103		/* Protocol indep. multicast */
#define IPPROTO_IPCOMP		108		/* IP Payload Comp. Protocol */
#define	IPPROTO_CARP		112		/* CARP */
#define	IPPROTO_MPLS		137		/* unicast MPLS packet */
#define	IPPROTO_PFSYNC		240		/* PFSYNC */
#define	IPPROTO_RAW		255		/* raw IP packet */

#define	IPPROTO_MAX		256

typedef struct ip_address
{
	__uint8_t byte1;
	__uint8_t byte2;
	__uint8_t byte3;
	__uint8_t byte4;
} ip_address;


typedef struct ip_header {

	__uint8_t     ip_hl;		/* header length */		/* version */
	__uint8_t  ip_tos;		/* type of service */
	__uint16_t ip_len;		/* total length */
	__uint16_t ip_id;		/* identification */
	__uint16_t ip_off;		/* flags+ fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* don't fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	__uint8_t  ip_ttl;		/* time to live */
	__uint8_t  ip_proto;			/* protocol */
	__uint16_t ip_crc;		/* checksum */
	ip_address ip_src;
	ip_address ip_dst; /* source and dest address */
	//__uint32_t ip_options;    /*ip options + padding*/ /*IHL >5*/  /*optional in udp it takes diffrent structure */
}ip_header;


void dbg_ip_header(ip_header * );
void dbg_ip_addres(ip_address *);

void set_host_ip_net(ip_address ip, ip_address netmask);
ip_address get_host_ip();
ip_address get_host_netmask();
ip_address get_host_net();

__uint16_t ip_checksum(const void *buf, size_t hdr_len);
void init_ip_header(ip_header * ptr);
//void create_packiet(void *packiet,__uint32_t pack_len,void * data,__uint32_t data_len);

#endif /* IP_HEADER_H_ */
