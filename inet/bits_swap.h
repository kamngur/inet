#ifndef _BIS_SWAP_H_
#define _BIS_SWAP_H_
//! Byte swap unsigned short


 //#if BYTE_ORDER==__LITTLE_ENDIAN__
         //! Network to host conversion for a word.
         #define ntohs(n) ( (((n) & 0xFF00) >> 8) | (((n) & 0x00FF) << 8) )
         //! Host to network conversion for a word.
         #define htons(n) ( (((n) & 0xFF00) >> 8) | (((n) & 0x00FF) << 8) )
         //! Network to host conversion for a double word.
         #define ntohl(n) ( (((n) & 0xFF000000) >> 24) | (((n) & 0x00FF0000) >> 8) \
                 | (((n) & 0x0000FF00) << 8) | (((n) & 0x000000FF) << 24) )

         //! Host to network conversion for a double word.
         #define htonl(n) ( (((n) & 0xFF000000) >> 24) | (((n) & 0x00FF0000) >> 8) \
                 | (((n) & 0x0000FF00) << 8) | (((n) & 0x000000FF) << 24) )
 //#else
         ////! Network to host conversion for a word.
         //#define ntohs(n) (n)
         ////! Host to network conversion for a word.
         //#define htons(n) (n)
         ////! Network to host conversion for a double word.
         //#define ntohl(n) (n)
         ////! Host to network conversion for a double word.
         //#define htonl(n) (n)
 //#endif

uint16_t swap_uint16( uint16_t   ); 


//! Byte swap short
 int16_t swap_int16(  int16_t   ); 

//! Byte swap unsigned int
uint32_t swap_uint32( uint32_t   );


//! Byte swap int
 int32_t swap_int32(  int32_t   );

 int64_t swap_int64(  int64_t   );


uint64_t swap_uint64( uint64_t   );


#endif
