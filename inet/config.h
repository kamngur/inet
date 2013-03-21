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

ip_address *getHostIp();
ip_address *getServerIp();

ether_addr *getHostMac();
uuid *getHostUuid();

#endif