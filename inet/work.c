/*
 * work.c
 *
 *  Created on: 30-05-2013
 *      Author: Kangur
 */

#include "ncp.h"
#include "packet.h"
#include "dm9000a.h"

static state current_state = START;


unsigned char data[ETHER_MAX_LEN];
uint32_t data_size = 0;


unsigned char result[ETHER_MAX_LEN];
uint32_t result_size=0;

ncp_header m_ncp;



int manage_ncp(ncp_header * in_ncp)
{
	int i =0;
	
	ip_header * ip;
	udp_header * udp;
	ncp_header * ncp;
	switch (current_state)
	{
	case START:

		ncp_register(&m_ncp,NCP_HEADER_SIZE);
		result_size = create_packiet(result,ETHER_MAX_LEN,&m_ncp,NCP_HEADER_SIZE);
		TransmitPacket(result,result_size);
		current_state = IDLE;
		break;
	case IDLE:
		if(data_size>0)
			get_headers(data,0,&ip,&udp,&ncp);
		else
			return 0;

		if(ncp->ncp_comm == NCP_DATA)
		{
			i = memcmp(&(ncp->ncp_client),get_host_uuid(),sizeof(uuid));
			if(i = 0)
				current_state = PROCESSING ;
		}

		break;
		// wait for processing order
	case PROCESSING:
		//processing funtion
		printf ("PROCESSING \n");
		current_state = RESULT;
		break;

	case RESULT:

		ncp_result(&m_ncp,NCP_HEADER_SIZE,0,0);
		result_size = create_packiet(result,ETHER_MAX_LEN,&m_ncp,NCP_HEADER_SIZE);
		TransmitPacket(result,result_size);
		current_state = IDLE;
		break;
	case MAX_STATE:
		break;
	default:
		printf ("Manage Ncp: unknow state\n");
	};
	return 0;
}

