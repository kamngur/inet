// win_client.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"


#include "pcap.h"
#pragma pack(push,1)
extern "C"
{
#include <WinNT.h>
#include <WinDef.h>
#include <WinBase.h>

#include "ethernet_header.h"
#include "ip_header.h"
#include "udp_head.h"
#include "packed.h"
#include "packet.h"
#include "build_time.h"
#include "config.h"
};
#pragma pop


#ifdef _MSC_VER
/*
 * we do not want the warnings about the old deprecated and unsecure CRT functions
 * since these examples can be compiled under *nix as well
 */
#define _CRT_SECURE_NO_WARNINGS
#endif

pcap_t *adhandle;



uint32_t tmp =1;



void add_crc(unsigned char *ptr,unsigned int tx_len)
{
	
	uint32_t m_tmp;
	uint32_t ethernet_len;
	uint8_t *my_ptr=0;
	uint16_t udp_crc=0;
	uint16_t udp_crc2=0;
	uint16_t udp_crc3=0;
	uint16_t udp_crc4=0;
	uint32_t eth_crc=0;
	uint32_t m_data_len= tx_len - ETHER_HDR_LEN - IP_HEADER_SIZE - UDP_HEADER_SIZE;
	ethernet_header * eth ;
	ip_header * ip;
	udp_header * udp ;
	get_headers((char *) ptr, &eth, &ip, &udp, 0 );
    ip->ip_crc = 0;
	ip->ip_crc =ip_checksum (ip,sizeof(ip_header));
	m_data_len = swap_uint16(udp->uh_ulen);
//	ethernet_len = ETHER_HDR_LEN + IP_HEADER_SIZE +  UDP_HEADER_SIZE + m_data_len + 4;
    ethernet_len = ETHER_HDR_LEN + IP_HEADER_SIZE +  UDP_HEADER_SIZE + m_data_len  ;
	
	//udp_crc = udp_sum_calc((uint16_t*)udp,m_data_len+8,(uint16_t*)&(ip->ip_src),(uint16_t*)&(ip->ip_dst));
	//udp_crc2 = udp_checksum(udp,m_data_len+8,&(ip->ip_src),&(ip->ip_dst));
	//udp_crc3 = udp_sum_calc((uint16_t*)udp,m_data_len,(uint16_t*)&(ip->ip_src),(uint16_t*)&(ip->ip_dst));
	//udp_crc4 = udp_checksum(udp,m_data_len,&(ip->ip_src),&(ip->ip_dst));
	//udp->uh_crc = swap_uint16(udp_crc);
	m_tmp = ethernet_len;
   // eth_crc = ether_crc_le(m_tmp,(char*)ptr,1);
	eth_crc = ~ether_crc32_le_update(ptr,m_tmp);
	my_ptr = ptr + ethernet_len ;
    *my_ptr++=(uint8_t)((eth_crc & 0x000000ff));
    *my_ptr++=(uint8_t)((eth_crc & 0x0000ff00)>>8);
    *my_ptr++=(uint8_t)((eth_crc & 0x00ff0000)>>16);
	*my_ptr++=(uint8_t)((eth_crc & 0xff000000)>>24);
	
	
	

}

extern "C" int TransmitPacket (unsigned char *data_ptr,unsigned int tx_len)
{
	add_crc(data_ptr,tx_len);
	if (pcap_sendpacket(adhandle,	// Adapter
		data_ptr,				// buffer with the packet
		(int)tx_len				// size
		) != 0)
	{
		fprintf(stderr,"\nError sending the packet: %s\n", pcap_geterr(adhandle));
		//return 3;
	}
	return 0;
}


/* prototype of the packet handler */
/* Callback function invoked by libpcap for every incoming packet */


void send_packiet()
{
	char packiet[1502];
	u_char * ptr=(u_char *)&packiet;
	uint32_t data_len=1502;
	char data[100]="abcdefgh\0" ;
	int s= sizeof(ip_header);
	ncp_register(data,NCP_HEADER_SIZE);
	s = create_packiet((void*)ptr, data_len,(void*) data,NCP_HEADER_SIZE);
	add_crc(ptr,data_len);
	/* Send down the packet */
	if (pcap_sendpacket(adhandle,	// Adapter
		ptr,				// buffer with the packet
		s				// size
		) != 0)
	{
		fprintf(stderr,"\nError sending the packet: %s\n", pcap_geterr(adhandle));
		//return 3;
	}
}

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	struct tm *ltime;
	char timestr[16];
	ethernet_header *eth;
	ip_header *ih;
	udp_header *uh;
	u_int ip_len;
	u_short sport,dport;
	time_t local_tv_sec;
	int val = 0;
	frame * m_frame = 0;

	/*
	 * unused parameter
	 */
	(VOID)(param);

	/* convert the timestamp to readable format */
	local_tv_sec = header->ts.tv_sec;
	ltime=localtime(&local_tv_sec);
	strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

	/* print timestamp and length of the packet */
	printf("%s.%.6d len:%d \n", timestr, header->ts.tv_usec, header->len);

	val = filter_packiets((char *)pkt_data, header->len);

	if (val != 0)
	{
		
		return ;
	}
	else
	{
		m_frame = get_free_frame();
		if (m_frame == 0)
			return;

		m_frame->f_len = header->len;
		memcpy(m_frame->f_data,pkt_data,header->len); //XXX

		add_rx_frame(m_frame);
		m_frame = 0;
	}
	get_headers((char *)pkt_data,&eth,&ih,&uh,0);

	
	dbg_ethernet_header(eth);
	/* retrieve the position of the ip header */

	dbg_ip_header(ih);
	/* retrieve the position of the udp header */

	
	/* convert from network byte order to host byte order */
	sport = ntohs( uh->uh_sport );
	dport = ntohs( uh->uh_dport );
	//sport =  uh->uh_sport;
	//dport = uh->uh_dport ;

	/* print ip addresses and udp ports */
    printf("%d.%d.%d.%d:%d -> %d.%d.%d.%d:%d\n",
		ih->ip_src.byte1,
		ih->ip_src.byte2,
		ih->ip_src.byte3,
		ih->ip_src.byte4,
		sport,
		ih->ip_dst.byte1,
		ih->ip_dst.byte2,
		ih->ip_dst.byte3,
		ih->ip_dst.byte4,
		dport);
	send_packiet();

}

int _tmain2(int argc, _TCHAR* argv[])
{
    return 0;

}

int _tmain(int argc, _TCHAR* argv[])
{
	const char compile_time[]= __TIME__;
	const char compile_date[]= __DATE__;
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i=0;
	
	//set_config();
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	char packet_filter[] = "ip and udp";
	//char packet_filter[] = "";
	struct bpf_program fcode;
	init_lists();
	/* Retrieve the device list */
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}
	
	/* Print the list */
	for(d=alldevs; d; d=d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if(i==0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}
	
	printf("Enter the interface number (1-%d):",i);
   // inum = 1;
	scanf("%d", &inum);
	
	/* Check if the user specified a valid adapter */
	if(inum < 1 || inum > i)
	{
		printf("\nAdapter number out of range.\n");
		
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
	
	/* Open the adapter */
	if ((adhandle= pcap_open_live(d->name,	// name of the device
							 65536,			// portion of the packet to capture. 
											// 65536 grants that the whole packet will be captured on all the MACs.
							 1,				// promiscuous mode (nonzero means promiscuous)
							 1000,			// read timeout
							 errbuf			// error buffer
							 )) == NULL)
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	/* Check the link layer. We support only Ethernet for simplicity. */
	if(pcap_datalink(adhandle) != DLT_EN10MB)
	{
		fprintf(stderr,"\nThis program works only on Ethernet networks.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	if(d->addresses != NULL)
		/* Retrieve the mask of the first address of the interface */
		netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask=0xffffff; 


	//compile the filter
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) <0 )
	{
		fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	//set the filter
	if (pcap_setfilter(adhandle, &fcode)<0)
	{
		fprintf(stderr,"\nError setting the filter.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	printf("\nlistening on %s...\n", d->description);
	
	/* At this point, we don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);
	
	/* start the capture */
	pcap_loop(adhandle, 0, packet_handler, NULL);
	/*do more in new thread*/


   	printf("\n Test 22122222222222222\n");


	return 0;
}
