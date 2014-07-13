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


extern ether_addr host_mac;


void set_config(); //inits data 

const in_port_t  get_host_port();
const in_port_t  get_server_port();

ip_address *get_host_ip();
ip_address *get_server_ip();

ether_addr *get_server_mac();
ether_addr *get_host_mac();

const uuid *get_host_uuid();
const uuid *get_project_uuid();
const uuid *get_task_uuid();

#endif
