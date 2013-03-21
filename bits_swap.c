#ifndef _BIS_SWAP_H_
#define _BIS_SWAP_H_
//! Byte swap unsigned short
#include "types.h"

__uint16_t swap_uint16( __uint16_t val ) 
{
	return (val << 8) | (val >> 8 );
}

//! Byte swap short
 __int16_t swap_int16(  __int16_t val ) 
{
	return (val << 8) | ((val >> 8) & 0xFF);
}

//! Byte swap unsigned int
__uint32_t swap_uint32( __uint32_t val )
{
	val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
	return (val << 16) | (val >> 16);
}

//! Byte swap int
 __int32_t swap_int32(  __int32_t val )
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
	return (val << 16) | ((val >> 16) & 0xFFFF);
}

 __int64_t swap_int64(  __int64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

__uint64_t swap_uint64( __uint64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}

#endif
