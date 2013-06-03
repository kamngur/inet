/*
 * ncp.h
 *
 *  Created on: 23-03-2013
 *      Author: Kangur
 */


#include "ncp.h"
#include "config.h"

/*
* Current number of communicate
*/
static __uint32_t comm_number=0;

int init_ncp_data(void *data, __uint32_t data_len ,__uint32_t type, __uint32_t option1, __uint32_t option2, uuid * client, uuid * task)
{
	ncp_header * hdr = (ncp_header *) data;

	if(data_len<sizeof(ncp_header))
		return 1;

	//typedef struct ncp_header {
	//	__uint32_t ncp_comm;		/* communicate */
	//	__uint32_t ncp_numb;		/* comm number */
	//	__uint32_t ncp_option1;		/*field with options*/
	//	__uint32_t ncp_option2;	
	//	uuid	ncp_client;			/* UUID of client*/		//(77d68722-bb20-4223-b34d-4bdf2059a731);	
	//	uuid	ncp_task;			/* UUID of project/task*/ 
	//}ncp_header;


	hdr->ncp_comm = type;
	hdr->ncp_numb = comm_number++;
	hdr->ncp_option1 = option1;
	hdr->ncp_option2 = option2;
	hdr->ncp_client = *client;
	hdr->ncp_project = *task;

	return 0;
}

int ncp_register(void *data, __uint32_t data_len )
{
	return init_ncp_data(data,data_len,NCP_REGISTER,0,0,get_host_uuid(),get_host_uuid());

}

/*
* Creates result message in 
*/
int ncp_result(void *data, __uint32_t data_len, void *result, __uint32_t result_len  )
{

	 int i = init_ncp_data(data,data_len,NCP_RESULT,0,0,get_host_uuid(),get_host_uuid());
	return  i;

}
