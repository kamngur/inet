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

int init_ncp_data(void *data, __uint32_t data_len ,__uint32_t type, __uint32_t option1, __uint32_t option2, uuid * client,uuid * project, uuid * task)
{
	ncp_header * hdr = (ncp_header *) data;

	if(data_len<sizeof(ncp_header))
		return 1;




	hdr->ncp_comm = type;
	hdr->ncp_numb = comm_number++;
	hdr->ncp_option1 = option1;
	hdr->ncp_option2 = option2;
	hdr->ncp_client = *client;
	hdr->ncp_project = *project;
	hdr->ncp_task = *task;

	return 0;
}

int ncp_register(void *data, __uint32_t data_len )
{
	return init_ncp_data(data,data_len,NCP_REGISTER,0,0,get_host_uuid(),get_project_uuid(),get_task_uuid());

}

/*
* Creates result message in 
*/
int ncp_result(void *data, __uint32_t data_len, void *result, __uint32_t result_len  )
{

	 int i = init_ncp_data(data,data_len,NCP_RESULT,0,0,get_host_uuid(),get_project_uuid(),get_task_uuid());
	return  i;

}

int ncp_progress(void *data, __uint32_t data_len )
{

	int i = init_ncp_data(data,data_len,NCP_PROGGRES,1,2,get_host_uuid(),get_project_uuid(),get_task_uuid());
	return  i;

}
