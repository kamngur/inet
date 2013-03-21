/*
 * udp_head.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 */

#ifndef _PACK_H_
#define _PACK_H_
#ifdef _MSC_VER
#  define PACKED_STRUCT(name) \
	__pragma(pack(push, 1)) struct name __pragma(pack(pop))
#else
#  define PACKED_STRUCT(name) struct __attribute__((packed)) name
#endif
#endif /* UDP_HEAD_H_ */
