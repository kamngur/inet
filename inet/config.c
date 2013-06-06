/*
 * config.c
 *
 *  Created on: 15-03-2013
 *      Author: Kangur
 */
#include "build_time.h"
#include "ethernet_header.h"
#include "ip_header.h"
#include "ncp.h"

static ip_address host_ip = {192,168,1,2};
static ip_address server_ip = {192,168,1,1};

static in_port_t host_port = 5556;
static in_port_t server_port = 5555;

static ether_addr host_mac = { 0x01, 0x60, 0x6E, 0x11, 0x01, 0x1F  };
static ether_addr server_mac = { 0x5c, 0xf9, 0xdd, 0x53, 0x07, 0x34  };
static ip_address localhost = {127,0,0,1};
//! This is the IP netmask of this host (expressed in network format).
static ip_address host_netmask =  {255,255,0,0};

// {AA3E630E-6600-424d-87FE-04F2D9F78E45}
static uuid host_uuid ={ 0xaa3e630e, 0xDE02, 0xDE02, 0xDE02, { 0x4, 0xf2, 0xd9, 0xf7, 0x8e, 0x45 } };
static uuid project_uuid ={ 0xab3a641e, 0xDE02, 0xDE02, 0xDE02, { 0x4, 0xf2, 0xd9, 0xf7, 0x8e, 0x45 } };
static uuid task_uuid ={ 0xab1a642a, 0xDE02, 0xDE02, 0xDE02, { 0x4, 0xf2, 0xd9, 0xf7, 0x8e, 0x45 } };
// {AA3E630E-6600-424d-87FE-04F2D9F78E45}

in_port_t get_host_port()
{
	return host_port;
}

in_port_t get_server_port()
{
	return server_port;
}

void set_config()
{
//set mac
#ifndef _STATIC_
 host_mac.ether_addr_octet[0]=getCompileYear()&0xFF00;
 host_mac.ether_addr_octet[1]=getCompileYear()&0x00FF;
 host_mac.ether_addr_octet[2]=getCompileDay();
 host_mac.ether_addr_octet[3]=getCompileHour();
 host_mac.ether_addr_octet[4]=getCompileMinutes();
 host_mac.ether_addr_octet[5]=getCompileSeconds();
 //set ip
 host_ip.byte3= getCompileHour()+getCompileSeconds();
 host_ip.byte4= getCompileMinutes()+getCompileSeconds();

// set uuid
 host_uuid.node[0]=host_ip.byte1;
 host_uuid.node[1]=host_ip.byte2;
 host_uuid.node[2]=host_ip.byte3;
 host_uuid.node[3]=host_ip.byte4;
#endif /* _STATIC_*/
}

ip_address *get_host_ip()
{
	return &host_ip;
};
ip_address *get_server_ip()
{
	return &server_ip;
};

ether_addr *get_host_mac()
{
	return &host_mac;
};

ether_addr *get_server_mac()
{
	return &server_mac;
};

const uuid *get_host_uuid()
{
	return &host_uuid;
};
const uuid *get_project_uuid()
{
	return &project_uuid;
};

const uuid *get_task_uuid()
{
	return &task_uuid;
};
