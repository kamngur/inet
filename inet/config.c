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

//#define VMWARE

#ifdef VMWARE

static ether_addr host_mac   = {0x00, 0x50 , 0x56 , 0xc0,0x00, 0x08};
static ip_address host_ip    = {192,168,229,1};
static ether_addr server_mac = {0x00, 0x0c, 0x29 , 0x58,0xB8, 0x30};
static ip_address server_ip  = {192,168,229,135};
#else 
    #ifdef LOCALHOST
    static ip_address host_ip = {192,168,0,1};
    static ip_address server_ip = {192,168,0,7};
  //  static ether_addr host_mac= {0x5c, 0xf9 , 0xdd , 0x53,0x07, 0x34}; //REALTEC ETHERNET
  //  static ether_addr server_mac= {0x5c, 0xf9 , 0xdd , 0x53,0x07, 0x34};
    static ether_addr host_mac= {0x00, 0x0b , 0x00 , 0x00,0xad, 0xd0}; //WIFI
    static ether_addr server_mac= {0xac, 0x7b , 0xa1 , 0x3c,0x6b, 0x9e};
    #endif

#endif

static ether_addr host_mac= { 0x00, 0x0c, 0x29, 0x58, 0xf8, 0x3b }; //REALTEC ETHERNET
static ip_address host_ip = {192,168,1,2};
static ip_address server_ip = {192,168,1,1};
static ether_addr server_mac= {0x5c, 0xf9 , 0xdd , 0x53,0x07, 0x34};
//static ip_address host_ip = {192,168,229,1};
//static ip_address server_ip = {192,168,229,130};

const in_port_t host_port = 5556;
//static in_port_t server_port = 45100;
const in_port_t server_port = 45000;
//static ether_addr host_mac = { 0x00, 0x0c, 0x29, 0x58, 0xf8, 0x3b  };
//static ether_addr server_mac = { 0x00, 0x0c, 0x29, 0x58, 0xb8, 0x30  };
//static ether_addr host_mac= {0x84, 0xA6 , 0xC8 , 0xF5,0xB1, 0x7B};
//static ether_addr server_mac= {0x84, 0xA6 , 0xC8 , 0xF5,0xB1, 0x7B};
static ip_address localhost = {127,0,0,1};
//! This is the IP netmask of this host (expressed in network format).
static ip_address host_netmask =  {255,255,0,0};

// {AA3E630E-6600-424d-87FE-04F2D9F78E45}

static uuid host_uuid ={ 0xaa3e630e, 0xDE02, 0xDE02, 0xDE02, { 0x04, 0xf2, 0xd9, 0xf7, 0x8e, 0x45 } };
static uuid project_uuid ={0xeecf2c8e, 0x8a17, 0x8c40, 0x94bf, { 0xb1, 0x93, 0x5a, 0xda, 0xb9, 0x85 } };
static uuid task_uuid ={ 0xab1a642a, 0xDE02, 0xDE02, 0xDE02, { 0x4, 0xf2, 0xd9, 0xf7, 0x8e, 0x45 } };


// {AA3E630E-6600-424d-87FE-04F2D9F78E45}



 const in_port_t get_host_port()
{
	return host_port;
}

 const in_port_t get_server_port()
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
