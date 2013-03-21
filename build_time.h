/*
 * build_time.h
 *
 *  Created on: 15-03-2013
 *      Author: Kangur
 */

#ifndef _BUILD_TIME_H_
#define _BUILD_TIME_H_




#define BUILD_YEAR ((__DATE__[7] - '0') * 1000 +  (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + __DATE__[10] - '0')

#define BUILD_DATE ((__DATE__[4] - '0') * 10 + __DATE__[5] - '0')


//#if 1
//#if ((__DATE__[0]) == 'J' && (__DATE__[1]) == 'a' && (__DATE__[2]) == 'n')
//#define BUILD_MONTH  1
//#elif (__DATE__[0] == 'F' && __DATE__[1] == 'e' && __DATE__[2] == 'b')
//#define BUILD_MONTH  2
//#elif (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
//#define BUILD_MONTH  3
//#elif (__DATE__[0] == 'A' && __DATE__[1] == 'p' && __DATE__[2] == 'r')
//#define BUILD_MONTH  4
//#elif (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
//#define BUILD_MONTH  5
//#elif (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
//#define BUILD_MONTH  6
//#elif (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
//#define BUILD_MONTH  7
//#elif (__DATE__[0] == 'A' && __DATE__[1] == 'u' && __DATE__[2] == 'g')
//#define BUILD_MONTH  8
//#elif (__DATE__[0] == 'S' && __DATE__[1] == 'e' && __DATE__[2] == 'p')
//#define BUILD_MONTH  9
//#elif (__DATE__[0] == 'O' && __DATE__[1] == 'c' && __DATE__[2] == 't')
//#define BUILD_MONTH 10
//#elif (__DATE__[0] == 'N' && __DATE__[1] == 'o' && __DATE__[2] == 'v')
//#define BUILD_MONTH 11
//#elif (__DATE__[0] == 'D' && __DATE__[1] == 'e' && __DATE__[2] == 'c')
//#define BUILD_MONTH 12
//#else
//#error "Could not figure out month"
//#endif
//#endif






int getCompileHour();
int getCompileMinutes();
int getCompileSeconds();
int getCompileYear();
int getCompileDay();



extern const char compile_time[];
extern const char compile_date[];


#endif /* _BUILD_TIME_H_ */
