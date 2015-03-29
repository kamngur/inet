/*
 * work.h
 *
 *  Created on: 30-05-2013
 *      Author: Kangur
 */

#ifndef _WORK_H_
#define _WORK_H_


//extern unsigned char data[ETHER_MAX_LEN];
//extern uint32_t data_size = 0;






int manage_ncp(void * packiet_data,uint32_t packiet_len );
int manage_ncp_loop(void * packiet_data,uint32_t packiet_len );

#endif
