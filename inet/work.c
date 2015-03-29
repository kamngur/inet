/*
 * work.c
 *
 *  Created on: 30-05-2013
 *      Author: Kangur
 */

#include "ncp.h"
#include "packet.h"
#include "../dm9000a.h"

static state current_state = IDLE; //skip register it's done
static state next_state = IDLE;

char input[ETHER_MAX_LEN];
uint16_t input_size = 0;


char output_packiet[ETHER_MAX_LEN];
uint16_t output_size=0;



const uint16_t MAX_NCP_LEN = ETHER_MAX_LEN - ETHER_HDR_LEN - IP_HEADER_SIZE - UDP_HEADER_SIZE ;

char outcome_ncp[ETHER_MAX_LEN];
uint16_t outcome_size=NCP_HEADER_SIZE;

ncp_header *m_ncp = outcome_ncp;

static uuid LOOPBACK_TASK ={ 0x00, 0x00, 0x00, 0x00, { 0x00, 0x00, 0x0, 0x00, 0x00, 0x00 } };


/*!
* @method		: process_ncp
* @class		: process_ncp
* @description 	: process given task from server. We work witch ncp_header + data. For now data mus be  from one packiet.
*
* @details		:
* Access		: public 
* Qualifier:	:
*
* @returns    	: (int) - length of returned data
* @param		: (ncp_header * input_data) -input ncp header
* @param		: (uint32_t input_len) - len of ncp_header + ncp data. (udp data len)
* @param		: (ncp_header * output_data) -output ncp header
* @param		: (uint32_t output_len) - len of ncp_header + ncp data. (udp data len)
**/
int process_ncp(ncp_header * input_data,uint16_t input_len, ncp_header *output_data, uint16_t output_len )
{
    if ( memcmp(input_data->ncp_task,&LOOPBACK_TASK,sizeof(uuid)) == 0 )
    {
        memcpy(output_data,input_data,input_len);
        output_len = input_len;
    }
    return output_len;
}


int manage_ncp(void * packiet_data,uint16_t packiet_len )
{
	int i =0;
	ethernet_header* eh;
	ip_header * ip;
	udp_header * udp;
	ncp_header * ncp;
    uint16_t data_size;
    
   get_headers(packiet_data,&eh,&ip,&udp,&ncp);
    data_size = udp->uh_ulen;
   

	switch (current_state)
	{
	case START:

		ncp_register(&m_ncp,NCP_HEADER_SIZE);
		output_size = create_packiet(output_packiet,ETHER_MAX_LEN,m_ncp,NCP_HEADER_SIZE);
		TransmitPacket(output_packiet,output_size);
		next_state = IDLE;
		break;
	case IDLE:
		if(packiet_len=0)
            break;
			

		if(ncp->ncp_comm == NCP_DATA)
		{
			i = memcmp(&(ncp->ncp_client),get_host_uuid(),sizeof(uuid));
			if(i == 0)
				next_state = PROCESSING ;
		}

		break;
		// wait for processing order
	case PROCESSING:
		outcome_size = process_ncp(ncp,data_size,outcome_ncp,ETHER_MAX_LEN);
		printf("PROCESSING \n");
		next_state = RESULT;
		break;

	case RESULT:

		ncp_result(&m_ncp,NCP_HEADER_SIZE,0,0);
		output_size = create_packiet(output_packiet,ETHER_MAX_LEN,m_ncp,outcome_size);
		TransmitPacket(output_packiet,output_size);
		next_state = IDLE;
		break;
	case MAX_STATE:
        printf ("Manage Ncp: unpredicted state\n");
		break;
	default:
		printf ("Manage Ncp: unknow state\n");
	};
    current_state = next_state;
	return 0;
}


int manage_ncp_loop(void * packiet_data,uint16_t packiet_len )
{
    int i =0;
    ethernet_header*eh;
    ip_header * ip;
    udp_header * udp;
    ncp_header * ncp;
    get_headers(packiet_data,&eh,&ip,&udp,&ncp);
    input_size = swap_uint16(udp->uh_ulen);
    
    do 
    {
        current_state = next_state;
        switch (current_state)
        {
        case START:

            ncp_register(&m_ncp,NCP_HEADER_SIZE);
            output_size = create_packiet(output_packiet,ETHER_MAX_LEN,m_ncp,NCP_HEADER_SIZE);
            TransmitPacket(output_packiet,output_size);
            next_state = IDLE;
            break;
        case IDLE:
            if(packiet_len == 0)
                break;

            if(ncp->ncp_comm == NCP_DATA)
            {
                i = memcmp(&(ncp->ncp_client),get_host_uuid(),sizeof(uuid));
                if(i == 0)
                    next_state = PROCESSING ;
            }

            break;
            // wait for processing order
        case PROCESSING:
            output_size = process_ncp(ncp,input_size,m_ncp,NCP_HEADER_SIZE);
            printf ("PROCESSING \n");
            next_state = RESULT;
            break;

        case RESULT:

            ncp_result(m_ncp,NCP_HEADER_SIZE,0,0);
            output_size = create_packiet(output_packiet,ETHER_MAX_LEN,m_ncp,output_size);
            TransmitPacket(output_packiet,output_size);
            packiet_len = 0;
            next_state = IDLE;
            break;
        case MAX_STATE:
            printf ("Manage Ncp: unpredicted state\n");
            break;
        default:
            printf ("Manage Ncp: unknow state\n");
        };


    } while (current_state != next_state);
    
    return 0;
}
