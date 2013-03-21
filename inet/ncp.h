/*
 * ncp.h
 *
 *  Created on: 12-01-2013
 *      Author: Kangur
 *
 *	Description: File contains definitions implementation of Network Computing Protocol. 
 */

#ifndef _NCP_H_
#define _NCP_H_

#include "types.h"
#include "ip_header.h"

typedef struct uuid {
	__uint32_t	time_low;
	__uint16_t	time_mid;
	__uint16_t	time_hi_and_version;
	__uint16_t	sequence;
	__uint8_t	node[6]; 
}uuid;

#define NCP_REGISTER	0x52475354 //RGST /*register*/
#define NCP_DATA		0x44415441 //DATA //data
#define NCP_RESULT		0x52534c54 //RSLT //result
#define NCP_PROGGRES	0x50424752 //PRGR //proggress

typedef struct ncp_header {
	__uint32_t ncp_comm;		/* communicate */
	__uint32_t ncp_numb;		/* comm number */
	__uint32_t ncp_option1;		/*field with options*/
	__uint32_t ncp_option2;	
	uuid	ncp_client;			/* UUID of client*/		//(77d68722-bb20-4223-b34d-4bdf2059a731);	
	uuid	ncp_task;			/* UUID of project/task*/ 
}ncp_header;


typedef struct task{
	uuid task_uuid;

};

enum state
{IDLE,PROCESSING,MAX_STATE };


/*typedef struct npc_client_state{
	uuid	client_uuid;
	state	client_state;
	task*	client_task;
};*/

//typedef struct task{
//	uuid task_uuid;
//	//void *data;
//	//__uint16_t data_len;
//	// alloc_data_function_ptr
//	// dealloc_data_funtion_ptr
//	// process_funtion_ptr
//	// send_data_function_ptr
//};
#endif /* NPC_H */
