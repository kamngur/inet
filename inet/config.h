/*
 * config.h
 *
 *  Created on: 15-03-2013
 *      Author: Kangur
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_


#include "build_time.h"
#include "ethernet_header.h"
#include "ip_header.h"
#include "ncp.h"





void set_config(); //inits data 

ip_address *get_host_ip();
ip_address *get_server_ip();

ether_addr *get_host_mac();
uuid *get_host_uuid();

#endif