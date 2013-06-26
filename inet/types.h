/*
 * types.h
 *
 *  Created on: 15-01-2013
 *      Author: Kangur
 *      //based on openBSD types
 */


#ifndef TYPES_H_
#define TYPES_H_
#define __BIT_TYPES_DEFINED__
#include <sys\types.h>
#include <stddef.h>
/*
	Funny story ... Visual Studio don't have stdint.h. VS in many ways isn't compatible with C99
	Use http://msinttypes.googlecode.com/svn/trunk/stdint.h
*/
#include <stdint.h>

/*i386 types*/
/* 7.18.1.1 Exact-width integer types */
//typedef	signed char		__int8_t;
//typedef	unsigned char		__uint8_t;
//typedef	short			__int16_t;
//typedef	unsigned short		__uint16_t;
//typedef	int			__int32_t;
//typedef	unsigned int		__uint32_t;
//#ifndef _SIZE_T_DEFINED_
//#define _SIZE_T_DEFINED_
//typedef	unsigned int		size_t;
//#endif
///* LONGLONG */
//typedef	long long		__int64_t;
///* LONGLONG */
//typedef	unsigned long long	__uint64_t;

//#ifndef	__int8_t_defined
//#define	__int8_t_defined
//typedef	__int8_t		int8_t;
//#endif
//
//#ifndef	_UINT8_T_DEFINED_
//#define	_UINT8_T_DEFINED_
//typedef	__uint8_t		uint8_t;
//#endif
//
//#ifndef	_INT16_T_DEFINED_
//#define	_INT16_T_DEFINED_
//typedef	__int16_t		int16_t;
//#endif
//
//#ifndef	_UINT16_T_DEFINED_
//#define	_UINT16_T_DEFINED_
//typedef	__uint16_t		uint16_t;
//#endif
//
//
//
//#ifndef	_INT64_T_DEFINED_
//#define	_INT64_T_DEFINED_
//typedef	__int64_t		int64_t;
//#endif
//
//#ifndef	_UINT64_T_DEFINED_
//#define	_UINT64_T_DEFINED_
//typedef	__uint64_t		uint64_t;
//#endif
//
///* BSD-style unsigned bits types */
//typedef	__uint8_t	u_int8_t;
//typedef	__uint16_t	u_int16_t;
//typedef	__uint32_t	u_int32_t;
//typedef	__uint64_t	u_int64_t;



#endif /* TYPES_H_ */
